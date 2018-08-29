/*
  ==============================================================================

    SimpleVoice.cpp
    Created: 20 Apr 2018 2:08:09am
    Author:  COx2

  ==============================================================================
*/

#include "SimpleVoice.h"

namespace {
	const float HALF_PI = MathConstants<float>::halfPi;
	const float ONE_PI = MathConstants<float>::pi;
	const float TWO_PI = MathConstants<float>::twoPi;
}

SimpleVoice::SimpleVoice(OscillatorParameters* oscParams, LfoParameters* lfoParams, AmpEnvelopePatameters* ampEnvParams, AudioParameterBool* velocitySenseParam)
	: _oscParamsPtr(oscParams)
	, _lfoParamsPtr(lfoParams)
	, _ampEnvParamsPtr(ampEnvParams)
	, _velocitySenseParamPtr(velocitySenseParam)
	, ampEnv(ampEnvParams->Attack->get(), ampEnvParams->Decay->get(), ampEnvParams->Sustain->get(), ampEnvParams->Release->get())
	, currentAngle(0.0f)
	, lfoAngle(0.0f)
	, lastLevel(0.0f)
	, pitchBend(0.0f)
{}

SimpleVoice::~SimpleVoice()
{}

bool SimpleVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<const SimpleSound*> (sound) != nullptr;
}

void SimpleVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)
{
	DBG("startNote");

	// velocity = 0...1 
	if (SimpleSound* soundForPlay = dynamic_cast<SimpleSound*> (sound))
	{
		// 同じボイスでノートONが繰り返された場合はボイススチール処理としてのstopNote(false)が実行されないため、
		// startNote()が実行される。この時、AMP EGはリリース状態ではないため、波形の角度は前回フレームのラジアンを維持しておかないと、
		// 同じボイスから生成する波形の前後フレーム間の変化量が急峻になり、ノイズの発生原因となる。
		if (ampEnv.isHolding())
		{
			//ボイスの発音が初めての場合はラジアンを0としてリセットする
			currentAngle = 0.0f;
			lfoAngle = 0.0f;
		}

		if (_velocitySenseParamPtr->get()) {
			
			if (velocity <= 0.01f) {
				velocity = 0.01f;
			}

			level = velocity * 0.4f; // * 0.4をしておかないと、加算合成されるので破綻する;
		}
		else 
		{
			level = 0.8f;
		}

		pitchBend = ((float)currentPitchWheelPosition - 8192.0f) / 8192.0f;

		float cyclesPerSecond = (float)MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		float cyclesPerSample = (float)cyclesPerSecond / (float)getSampleRate();

		// startNoteではじめてangelDeltaが確定する
		angleDelta = cyclesPerSample * TWO_PI;

		// この時にサンプルレートは確定している
		ampEnv.attackStart();
		//ampEnv.cycle((float)getSampleRate());

		levelDiff = lastLevel - level;

	}
}

void SimpleVoice::stopNote(float velocity, bool allowTailOff)
{
	DBG("stopNote : " + juce::String((int)allowTailOff));

	// シンセサイザークラスから呼ばれるとき、キーリリースだとallowTailOff == true、キーリリース直後のボイススチールではallowTailOff == false
	lastLevel = level;

	if (allowTailOff)
	{
		ampEnv.releaseStart();

	}
	else
	{
		// キーホールド中(ADSのいずれか)であればangleDeltaをリリース状態に移行
		// ボイススチールを受けて直ぐに音量を0にしてしまうと、急峻な変化となりノイズの発生を引き起こすため、それを予防する処理。
		if (ampEnv.isHolding())
		{
			ampEnv.releaseStart();
		}
		// リリース状態に入っていたときはangleDeltaの値をリセット
		else if(ampEnv.isReleasing())
		{
			angleDelta = 0.0f;		
		}

		// ボイススチール処理の過程で現在のノート再生状態をクリアする
		clearCurrentNote();
	}
}

void SimpleVoice::pitchWheelMoved(int newPitchWheelValue)
{
	// シンセサイザークラスから呼ばれるとき、キーリリースだとallowTailOff == true、キーリリース直後のボイススチールではallowTailOff == false
	pitchBend = ((float)newPitchWheelValue - 8192.0f) / 8192.0f;

}

void SimpleVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void SimpleVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	ampEnv.setParameters(_ampEnvParamsPtr->Attack->get(), _ampEnvParamsPtr->Decay->get(), _ampEnvParamsPtr->Sustain->get(), _ampEnvParamsPtr->Release->get());

	// clearCurrentNote()実行後ではここは通れない
	if (SimpleSound* playingSound = static_cast<SimpleSound*>(getCurrentlyPlayingSound().get()))
	{
		if (angleDelta != 0.0f)
		{
			while (--numSamples >= 0) // 通常のキーON時はここを通る
			{
				levelDiff *= 0.99f;

				// Modulation factor
				float modulationFactor = calcModulationFactor(lfoAngle);

				// OSC MIX
				float currentSample = 0.0f;
				{
					currentSample += waveForms.sine(currentAngle) * _oscParamsPtr->SineWaveLevel->get();

					currentSample += waveForms.saw(currentAngle) * _oscParamsPtr->SawWaveLevel->get();

					currentSample += waveForms.triangle(currentAngle) * _oscParamsPtr->TriWaveLevel->get();

					currentSample += waveForms.square(currentAngle) * _oscParamsPtr->SquareWaveLevel->get();

					currentSample += waveForms.noise() * _oscParamsPtr->NoiseLevel->get();
				}

				// LFO
				if (_lfoParamsPtr->LfoTarget->getCurrentChoiceName() == "WaveLevel")
				{
					currentSample *= modulationFactor;
				}

				// Level and ADSR
				{
					currentSample *= level + levelDiff;
					currentSample *= ampEnv.getValue();
				}

				//バッファに対して加算処理を行う。1VOICE内でのMIXと複数VOICE間でのMIXなので加算
				for (int channelNum = outputBuffer.getNumChannels(); --channelNum >= 0;) 
				{
					outputBuffer.addSample(channelNum, startSample, currentSample);
				}

				if (_lfoParamsPtr->LfoTarget->getCurrentChoiceName() == "WaveAngle")
				{
					currentAngle += angleDelta * pow(2.0f, pitchBend) * modulationFactor;
				}
				else
				{
					currentAngle += angleDelta * pow(2.0f, pitchBend);
				}
				lfoAngle += (_lfoParamsPtr->LfoSpeed->get() / (float)getSampleRate()) * TWO_PI;

				ampEnv.cycle((float)getSampleRate());

				if (ampEnv.isReleasing())
				{
					if (ampEnv.getValue() <= 0.005f) //リリースが十分に小さければ
					{
						ampEnv.releaseEnd();

						// ボイススチール処理の過程で現在のノート再生状態をクリアする
						clearCurrentNote();
						angleDelta = 0.0f;
						currentAngle = 0.0f;
						break;
					}
				}

				if (abs(levelDiff) < 0.0001f)
				{
					levelDiff = 0.0f;
				}

				if (currentAngle > TWO_PI)
				{
					currentAngle -= TWO_PI;
				}

				if (lfoAngle > TWO_PI)
				{
					lfoAngle -= TWO_PI;
				}

				++startSample;
			}
		}
	}
}

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
	factor *= _lfoParamsPtr->LfoAmount->get();
	factor /= 2.0f;
	factor += 0.5f;

	return factor;
}
