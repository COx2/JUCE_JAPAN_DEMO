/*
  ==============================================================================

    SimpleVoice.cpp
    Created: 20 Apr 2018 2:08:09am
    Author:  COx2

  ==============================================================================
*/

// ヘッダファイルをインクルードする。
#include "SimpleVoice.h"

// ①無名名前空間に定数宣言を記述する。
namespace {
	const float HALF_PI = MathConstants<float>::halfPi;
	const float ONE_PI = MathConstants<float>::pi;
	const float TWO_PI = MathConstants<float>::twoPi;
}

// ②引数付きコンストラクタ
SimpleVoice::SimpleVoice(OscillatorParameters* oscParams, LfoParameters* lfoParams, AmpEnvelopePatameters* ampEnvParams, AudioParameterBool* velocitySenseParam)
	: _oscParamsPtr(oscParams)
	, _lfoParamsPtr(lfoParams)
	, _ampEnvParamsPtr(ampEnvParams)
	, _velocitySenseParamPtr(velocitySenseParam)
	, ampEnv(ampEnvParams->Attack->get(), ampEnvParams->Decay->get(), ampEnvParams->Sustain->get(), ampEnvParams->Release->get())
	, currentAngle(0.0f), lfoAngle(0.0f), angleDelta(0.0f)
	, level(0.0f), lastLevel(0.0f), levelDiff(0.0f)
	, pitchBend(0.0f)
{}

// デストラクタ
SimpleVoice::~SimpleVoice()
{}

// ③当ボイスクラスに対応するサウンドクラスが登録されているかどうかを判定する関数。
//   Synthesiserクラスから呼び出されるインターフェース。
bool SimpleVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<const SimpleSound*> (sound) != nullptr;
}

// ④Synthesiserクラスから呼び出されるノートONのトリガー。
//   引数で渡されたノート番号、ベロシティなどの値に対応して波形を生成する前準備を行う。
void SimpleVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)
{
	// 引数として受け取ったノート番号の値とベロシティの値をログとして出力する。
	DBG("[StartNote] NoteNumber: " + juce::String(midiNoteNumber) + ", Velocity: " + juce::String(velocity));

	// ノートON対象のサウンドクラスが当ボイスクラスに対応したものであるかどうかを判定する。
	if (SimpleSound* soundForPlay = dynamic_cast<SimpleSound*> (sound))
	{
		// ベロシティ有効/無効のフラグに応じて音量レベルを決定する。有効...ベロシティの値から算出する。 無効...固定値を使用する。
		if (_velocitySenseParamPtr->get()) 
		{
			if (velocity <= 0.01f) {
				velocity = 0.01f;
			}
			level = velocity * 0.4f;
		}
		else 
		{
			level = 0.8f;
		}

		// ノイズ対策...今回と前回の音量レベルの差分を算出して保持する。
		levelDiff = level - lastLevel;
		
		// ピッチベンド・メッセージの入力を-1.0f～1.0fの値に正規化して保持する。
		pitchBend = ((float)currentPitchWheelPosition - 8192.0f) / 8192.0f;

		// レンダリングする波形のピッチに相当するサンプルデータ間の角度⊿θ[rad]の値を決定する。
		float cyclesPerSecond = (float)MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		float cyclesPerSample = (float)cyclesPerSecond / (float)getSampleRate();
		angleDelta = cyclesPerSample * TWO_PI;

		// AMP EG: Attack状態に移行する。
		ampEnv.attackStart();
	}
}

// ⑤Synthesiserクラスから呼び出されるノートOFFのトリガー。
//   引数で渡される値に対応して波形生成を終了する前準備を行う。
//   キーリリースだとallowTailOff == true、キーリリース直後のボイススチールではallowTailOff == false
void SimpleVoice::stopNote(float velocity, bool allowTailOff)
{
	DBG("[stopNote] AllowTailOff: " + juce::String((int)allowTailOff));

	// ノイズ対策...現在のノートのレベルを保持しておく
	lastLevel = level;

	// キーリリース時...allowTailOff = true、ボイススチール時...allowTailOff = false
	if (allowTailOff)
	{
		ampEnv.releaseStart();
	}
	else
	{
		// ノイズ対策...ボイススチール直後のAttack状態への移行の時にエンベロープがRelease状態の値から繋がるようにしておく。
		if (ampEnv.isHolding()) {
			ampEnv.releaseStart();
		}

		// ボイスがレンダリングしているノート情報をクリアする
		clearCurrentNote();
	}
}

// ⑥ピッチホイール操作をトリガーとしてSynthesiserクラスから呼び出される。ピッチベンドの値を保持する変数を更新する。
void SimpleVoice::pitchWheelMoved(int newPitchWheelValue)
{
	pitchBend = ((float)newPitchWheelValue - 8192.0f) / 8192.0f;
}

// ⑦MIDI CC受信をトリガーとしてSynthesiserクラスから呼び出される関数。
void SimpleVoice::controllerMoved(int controllerNumber, int newControllerValue)
{}

// ⑧LFOのモジュレーション波形を算出する関数。
float SimpleVoice::calcModulationFactor(float angle)
{
	float factor = 0.0f;
	juce::String waveTypeName = _lfoParamsPtr->LfoWaveType->getCurrentChoiceName();
	if (waveTypeName == "Sine")
	{
		factor = waveForms.sine(angle);
	}
	else if (waveTypeName == "Saw")
	{
		factor = waveForms.saw(angle);
	}
	else if (waveTypeName == "Tri")
	{
		factor = waveForms.triangle(angle);
	}
	else if (waveTypeName == "Square")
	{
		factor = waveForms.square(angle);
	}
	else if (waveTypeName == "Noise")
	{
		factor = waveForms.noise();
	}
	// factorの値が0.5を中心とした0.0～1.0の値となるように調整する。
	factor = ((factor * _lfoParamsPtr->LfoAmount->get()) / 2.0f) + 0.5f;
	return factor;
}

// ⑨オーディオバッファに波形をレンダリングする関数。Synthesiserクラスから呼び出される
void SimpleVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	// レンダリング対象のサウンドクラスが当ボイスクラスに対応したものであるかどうかを判定する。
	if (SimpleSound* playingSound = dynamic_cast<SimpleSound*>(getCurrentlyPlayingSound().get()))
	{
		if (angleDelta != 0.0f)
		{
			while (--numSamples >= 0)
			{
				// LFO:モジュレーション波形のサンプルデータを生成する。
				float modulationFactor = calcModulationFactor(lfoAngle);

				// OSC MIX: 
				float currentSample = 0.0f;
				{
					currentSample += waveForms.sine(currentAngle) * _oscParamsPtr->SineWaveLevel->get();
					currentSample += waveForms.saw(currentAngle) * _oscParamsPtr->SawWaveLevel->get();
					currentSample += waveForms.triangle(currentAngle) * _oscParamsPtr->TriWaveLevel->get();
					currentSample += waveForms.square(currentAngle) * _oscParamsPtr->SquareWaveLevel->get();
					currentSample += waveForms.noise() * _oscParamsPtr->NoiseLevel->get();
				}

				// LFO: モジュレーション対象がWaveLevelの時のモジュレーション処理
				if (_lfoParamsPtr->LfoTarget->getCurrentChoiceName() == "WaveLevel") {
					currentSample *= modulationFactor;
				}

				// OSC MIX: 
				levelDiff *= 0.99f;						// 前回のベロシティとの差異によるノイズ発生を防ぐ。
				if (fabsf(levelDiff) <= 0.005f) {
					levelDiff = 0.0f;
				}
				currentSample *= level - levelDiff;

				// AMP EG: エンベロープの値をサンプルデータに反映する。
				currentSample *= ampEnv.getValue();

				// バッファに対して加算処理を行う。ポリフォニックでは、各ボイスの音を加算処理する必要がある。
				for (int channelNum = outputBuffer.getNumChannels(); --channelNum >= 0;) {
					outputBuffer.addSample(channelNum, startSample, currentSample);
				}

				// エンベロープがリリース状態の場合の処理
				if (ampEnv.isReleasing())
				{
					if (ampEnv.getValue() <= 0.005f) //エンベロープの値が十分に小さければ
					{
						ampEnv.releaseEnd();		 // エンベロープをWait状態に移行する。
						clearCurrentNote();			 // このボイスが生成するノート情報をクリアする。
						angleDelta = 0.0f;			 // 変数を初期値に戻す。
						currentAngle = 0.0f;		 // 変数を初期値に戻す。
						lfoAngle = 0.0f;			 // 変数を初期値に戻す。
						levelDiff = 0.0f;			 // 変数を初期値に戻す。
						break;
					}
				}

				//============ 次のサンプルデータを生成するための準備 ============

				// LFO:モジュレーション対象がWaveAngleの時のモジュレーション処理
				// ピッチベンド:MIDIキーボードのピッチベンド入力を反映する処理
				if (_lfoParamsPtr->LfoTarget->getCurrentChoiceName() == "WaveAngle")
				{
					currentAngle += angleDelta * pow(2.0f, pitchBend) * modulationFactor;
				}
				else
				{
					currentAngle += angleDelta * pow(2.0f, pitchBend);
				}
				lfoAngle += (_lfoParamsPtr->LfoSpeed->get() / (float)getSampleRate()) * TWO_PI;

				if (abs(levelDiff) < 0.0001f) {
					levelDiff = 0.0f;
				}

				if (currentAngle > TWO_PI) {
					currentAngle -= TWO_PI;
				}

				if (lfoAngle > TWO_PI) {
					lfoAngle -= TWO_PI;
				}

				// AMP EG: エンベロープの各パラメータを最新の値に変更する。
				ampEnv.setParameters(_ampEnvParamsPtr->Attack->get(), _ampEnvParamsPtr->Decay->get(), _ampEnvParamsPtr->Sustain->get(), _ampEnvParamsPtr->Release->get());

				// AMP EG: エンベロープを1サンプル分進めておく。
				ampEnv.cycle((float)getSampleRate());

				// 書き込み先のオーディオバッファのサンプルインデックス値をインクリメントする。
				++startSample;
			}
		}
	}
}
