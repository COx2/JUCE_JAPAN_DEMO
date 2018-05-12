/*
  ==============================================================================

    SimpleVoice.cpp
    Created: 20 Apr 2018 2:08:09am
    Author:  COx2

  ==============================================================================
*/

#include "SimpleVoice.h"

#include <iostream>
#include <string>

SimpleVoice::SimpleVoice(OscillatorParameters* oscParams, LfoParameters* lfoParams, AmpEnvelopePatameters* ampEnvParams, bool velocitySense)
	: _oscParams(oscParams)
	, _lfoParams(lfoParams)
	, _ampEnvParams(ampEnvParams)
	, ampEnv(ampEnvParams->Attack->get(), ampEnvParams->Decay->get(), ampEnvParams->Sustain->get(), ampEnvParams->Release->get())
	, currentAngle(0.0)
	, lastLevel(0.0f)
	, pitchBend(0.0f)
	, isVelocitySense(velocitySense)
{
}

	SimpleVoice::~SimpleVoice()
{}

bool SimpleVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<const SimpleSound*> (sound) != nullptr;
}

void SimpleVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* s, int currentPitchWheelPosition)
{
	// velocity = 0...1 
	DBG("[VOICE] startNote(): Note:" + juce::String(midiNoteNumber) + ", Vel:" + juce::String(velocity));

	if (SimpleSound* sound = dynamic_cast<SimpleSound*> (s))
	{
		// 異なるノートの場合はstopNoteが実行されずリリース状態ではないので、現在のラジアンを維持
		if (ampEnv.getState() != AmpEnvelope::AMPENV_STATE::RELEASE
			&& ampEnv.getState() != AmpEnvelope::AMPENV_STATE::WAIT)
		{
			//ボイスの発音が初めての場合はラジアンを0としてリセットする
			currentAngle = 0.0;
			lfoAngle = 0.0;
		}

		if (isVelocitySense) {
			if (velocity <= 0.01f)
				velocity = 0.01f;
			level = velocity * 0.4f; 		// * 0.15をしておかないと、加算合成されるので破綻する;
		}
		else 
		{
			level = 0.8f;
		}

		pitchBend = ((double)currentPitchWheelPosition - 8192.0) / 8192.0;

		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		double cyclesPerSample = cyclesPerSecond / getSampleRate();

		// startNoteではじめてangelDeltaが確定する
		// MathConstants<double>::piは記事で説明
		angleDelta = cyclesPerSample * MathConstants<double>::twoPi;

		// この時にサンプルレートは確定している
		ampEnv.attackStart((float)getSampleRate());
		ampEnv.cycle();

		levelDiff = lastLevel - level;



		DBG(juce::String("[VOICE] StartNote: ") + "CurrentSine: " + juce::String(std::to_string(sin(currentAngle))) + ", CurrentAngle: " + juce::String(std::to_string(currentAngle)) );
	}
}

void SimpleVoice::stopNote(float velocity, bool allowTailOff)
{
	// シンセサイザークラスから呼ばれるとき、キーリリースだとallowTailOff == true、キーリリース直後のボイススチールではallowTailOff == false
	DBG("[VOICE] stopNote()" + juce::String((int)allowTailOff));
	DBG(juce::String("[VOICE] SropNote: ") + "CurrentSine: " + juce::String(std::to_string(sin(currentAngle))) + ", CurrentAngle: " + juce::String(std::to_string(currentAngle)));

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
	DBG("[VOICE] pitchWheelMoved(): " + juce::String(newPitchWheelValue));

	pitchBend = ((double)newPitchWheelValue - 8192.0) / 8192.0;

	DBG("[VOICE] pitchBend: " + juce::String(powf(2.0f, pitchBend)));
}

void SimpleVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void SimpleVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	ampEnv.setParameters(_ampEnvParams->Attack->get(), _ampEnvParams->Decay->get(), _ampEnvParams->Sustain->get(), _ampEnvParams->Release->get());

	// clearCurrentNote()実行後ではここは通れない
	if (SimpleSound* playingSound = static_cast<SimpleSound*>(getCurrentlyPlayingSound().get()))
	{
		if (angleDelta != 0.0f)
		{
			while (--numSamples >= 0) // 通常のキーON時はここを通る

			{
				levelDiff *= 0.99f;

				float modulationFactor = (float)sin(lfoAngle) * _lfoParams->LfoLevel->get();

				float currentSample = 0.0f;

				if (_lfoParams->LfoTarget->getCurrentChoiceName() == "WaveLevel")
				{
					currentSample += (float)sin(currentAngle) * _oscParams->SineWaveLevel->get() * modulationFactor;

					currentSample += (float)calcSawWave(currentAngle) * _oscParams->SawWaveLevel->get() * modulationFactor;

					currentSample += (float)calcTriWave(currentAngle) * _oscParams->TriWaveLevel->get() * modulationFactor;

					currentSample += (float)calcSquareWave(currentAngle) * _oscParams->SquareWaveLevel->get() * modulationFactor;
				}
				else
				{
					currentSample += (float)sin(currentAngle) * _oscParams->SineWaveLevel->get();

					currentSample += (float)calcSawWave(currentAngle) * _oscParams->SawWaveLevel->get();

					currentSample += (float)calcTriWave(currentAngle) * _oscParams->TriWaveLevel->get();

					currentSample += (float)calcSquareWave(currentAngle) * _oscParams->SquareWaveLevel->get();

				}

				currentSample += (whiteNoise.nextFloat() * 2.0f - 1.0f) * _oscParams->NoiseLevel->get();

				currentSample *= level + levelDiff;
				currentSample *= ampEnv.getValue();

				for (int channelNum = outputBuffer.getNumChannels(); --channelNum >= 0;)
					outputBuffer.addSample(channelNum, startSample, currentSample); //バッファに対して加算処理を行う。加算じゃないとダメ

				if (_lfoParams->LfoTarget->getCurrentChoiceName() == "WaveAngle")
				{
					currentAngle += angleDelta * pow(2.0, pitchBend) * (1 + modulationFactor);
				}
				else
				{
					currentAngle += angleDelta * pow(2.0, pitchBend);
				}
				lfoAngle += (_lfoParams->LfoSpeed->get() / getSampleRate()) * MathConstants<double>::twoPi;
				ampEnv.cycle(); // [8]
					
				if (ampEnv.getState() == AmpEnvelope::AMPENV_STATE::RELEASE) // [7]
				{
					if (ampEnv.getValue() <= 0.005) //リリースが十分に小さければ
					{
						ampEnv.releaseEnd();

						clearCurrentNote();
						angleDelta = 0.0;
						currentAngle = 0.0;
						break;
					}
				}

				if (abs(levelDiff) < 0.0001f)
				{
					levelDiff = 0.0f;
				}

				if (currentAngle > MathConstants<double>::twoPi)
				{
					currentAngle -= MathConstants<double>::twoPi;
				}

				++startSample;

			}
		}
	}
}

double SimpleVoice::calcSawWave(double currentAngle)
{
	if (currentAngle <= MathConstants<double>::pi) 
	{
		return (currentAngle / MathConstants<double>::pi);
	}
	else
	{
		return -1.0 + ((currentAngle - MathConstants<double>::pi) / MathConstants<double>::pi) ;
	}
}

double SimpleVoice::calcTriWave(double currentAngle)
{
	if (currentAngle <= MathConstants<double>::halfPi)
	{
		return (currentAngle / MathConstants<double>::halfPi);
	}
	else if(currentAngle > MathConstants<double>::halfPi && currentAngle <= (MathConstants<double>::pi + MathConstants<double>::halfPi))
	{
		return 1.0 - (2.0 * ((currentAngle - MathConstants<double>::halfPi) / MathConstants<double>::pi));
	}
	else
	{
		return -1.0 + (currentAngle - ((MathConstants<double>::pi + MathConstants<double>::halfPi) / MathConstants<double>::halfPi));
	}
}

double SimpleVoice::calcSquareWave(double currentAngle)
{
	if (currentAngle <= MathConstants<double>::pi)
	{
		return 1.0f;
	}
	else
	{
		return -1.0f;
	}
}
