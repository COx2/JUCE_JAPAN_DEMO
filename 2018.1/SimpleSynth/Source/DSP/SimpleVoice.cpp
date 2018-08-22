/*
  ==============================================================================

    SimpleVoice.cpp
    Created: 20 Apr 2018 2:08:09am
    Author:  COx2

  ==============================================================================
*/

#include "SimpleVoice.h"

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
	// velocity = 0...1 
	if (SimpleSound* soundForPlay = dynamic_cast<SimpleSound*> (sound))
	{
		// 異なるノートの場合はstopNoteが実行されずリリース状態ではないので、現在のラジアンを維持
		if (ampEnv.getState() != AmpEnvelope::AMPENV_STATE::RELEASE
			&& ampEnv.getState() != AmpEnvelope::AMPENV_STATE::WAIT)
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
		// MathConstants<double>::piは記事で説明
		angleDelta = cyclesPerSample * MathConstants<float>::twoPi;

		// この時にサンプルレートは確定している
		ampEnv.attackStart((float)getSampleRate());
		ampEnv.cycle();

		levelDiff = lastLevel - level;

	}
}

void SimpleVoice::stopNote(float velocity, bool allowTailOff)
{
	// シンセサイザークラスから呼ばれるとき、キーリリースだとallowTailOff == true、キーリリース直後のボイススチールではallowTailOff == false
	lastLevel = level;

	if (allowTailOff)
	{
		ampEnv.releaseStart();

	}
	else
	{
		// リリース状態でないときはangleDeltaをリセットせずにリリース状態に移行
		if (ampEnv.getState() != AmpEnvelope::AMPENV_STATE::RELEASE
			&& ampEnv.getState() != AmpEnvelope::AMPENV_STATE::WAIT)
		{
			ampEnv.releaseStart();
		}
		else if(ampEnv.getState() == AmpEnvelope::AMPENV_STATE::RELEASE)
		{
			// リリース状態に入っていたときはangleDeltaをリセットする
			angleDelta = 0.0f;		
		}

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

				float modulationFactor = 0.0f;

				if (_lfoParamsPtr->LfoWaveType->getCurrentChoiceName() == "Sine")
				{
					modulationFactor = waveForms.sineWave(lfoAngle);
				}
				else if (_lfoParamsPtr->LfoWaveType->getCurrentChoiceName() == "Saw")
				{
					modulationFactor = waveForms.sawWave(lfoAngle);
				}
				else if (_lfoParamsPtr->LfoWaveType->getCurrentChoiceName() == "Tri")
				{
					modulationFactor = waveForms.triangleWave(lfoAngle);
				}
				else if (_lfoParamsPtr->LfoWaveType->getCurrentChoiceName() == "Square")
				{
					modulationFactor = waveForms.squareWave(lfoAngle);
				}
				else if (_lfoParamsPtr->LfoWaveType->getCurrentChoiceName() == "Noise")
				{
					modulationFactor = waveForms.whiteNoise();
				}
				modulationFactor *= _lfoParamsPtr->LfoAmount->get();
				modulationFactor /= 2.0f;
				modulationFactor += 0.5f;

				float currentSample = 0.0f;

				if (_lfoParamsPtr->LfoTarget->getCurrentChoiceName() == "WaveLevel")
				{
					currentSample += waveForms.sineWave(currentAngle) * _oscParamsPtr->SineWaveLevel->get() * modulationFactor;

					currentSample += waveForms.sawWave(currentAngle) * _oscParamsPtr->SawWaveLevel->get() * modulationFactor;

					currentSample += waveForms.triangleWave(currentAngle) * _oscParamsPtr->TriWaveLevel->get() * modulationFactor;

					currentSample += waveForms.squareWave(currentAngle) * _oscParamsPtr->SquareWaveLevel->get() * modulationFactor;
				}
				else
				{
					currentSample += waveForms.sineWave(currentAngle) * _oscParamsPtr->SineWaveLevel->get();

					currentSample += waveForms.sawWave(currentAngle) * _oscParamsPtr->SawWaveLevel->get();

					currentSample += waveForms.triangleWave(currentAngle) * _oscParamsPtr->TriWaveLevel->get();

					currentSample += waveForms.squareWave(currentAngle) * _oscParamsPtr->SquareWaveLevel->get();
				}

				currentSample += waveForms.whiteNoise() * _oscParamsPtr->NoiseLevel->get();

				currentSample *= level + levelDiff;
				currentSample *= ampEnv.getValue();

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
				lfoAngle += (_lfoParamsPtr->LfoSpeed->get() / (float)getSampleRate()) * MathConstants<float>::twoPi;

				ampEnv.cycle();
					
				if (ampEnv.getState() == AmpEnvelope::AMPENV_STATE::RELEASE) // [7]
				{
					if (ampEnv.getValue() <= 0.005f) //リリースが十分に小さければ
					{
						ampEnv.releaseEnd();

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

				if (currentAngle > MathConstants<float>::twoPi)
				{
					currentAngle -= MathConstants<float>::twoPi;
				}

				if (lfoAngle > MathConstants<float>::twoPi)
				{
					lfoAngle -= MathConstants<float>::twoPi;
				}

				++startSample;

			}
		}
	}
}
