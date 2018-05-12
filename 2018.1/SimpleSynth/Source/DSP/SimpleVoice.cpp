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
		// �قȂ�m�[�g�̏ꍇ��stopNote�����s���ꂸ�����[�X��Ԃł͂Ȃ��̂ŁA���݂̃��W�A�����ێ�
		if (ampEnv.getState() != AmpEnvelope::AMPENV_STATE::RELEASE
			&& ampEnv.getState() != AmpEnvelope::AMPENV_STATE::WAIT)
		{
			//�{�C�X�̔��������߂Ă̏ꍇ�̓��W�A����0�Ƃ��ă��Z�b�g����
			currentAngle = 0.0;
			lfoAngle = 0.0;
		}

		if (isVelocitySense) {
			if (velocity <= 0.01f)
				velocity = 0.01f;
			level = velocity * 0.4f; 		// * 0.15�����Ă����Ȃ��ƁA���Z���������̂Ŕj�]����;
		}
		else 
		{
			level = 0.8f;
		}

		pitchBend = ((double)currentPitchWheelPosition - 8192.0) / 8192.0;

		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		double cyclesPerSample = cyclesPerSecond / getSampleRate();

		// startNote�ł͂��߂�angelDelta���m�肷��
		// MathConstants<double>::pi�͋L���Ő���
		angleDelta = cyclesPerSample * MathConstants<double>::twoPi;

		// ���̎��ɃT���v�����[�g�͊m�肵�Ă���
		ampEnv.attackStart((float)getSampleRate());
		ampEnv.cycle();

		levelDiff = lastLevel - level;



		DBG(juce::String("[VOICE] StartNote: ") + "CurrentSine: " + juce::String(std::to_string(sin(currentAngle))) + ", CurrentAngle: " + juce::String(std::to_string(currentAngle)) );
	}
}

void SimpleVoice::stopNote(float velocity, bool allowTailOff)
{
	// �V���Z�T�C�U�[�N���X����Ă΂��Ƃ��A�L�[�����[�X����allowTailOff == true�A�L�[�����[�X����̃{�C�X�X�`�[���ł�allowTailOff == false
	DBG("[VOICE] stopNote()" + juce::String((int)allowTailOff));
	DBG(juce::String("[VOICE] SropNote: ") + "CurrentSine: " + juce::String(std::to_string(sin(currentAngle))) + ", CurrentAngle: " + juce::String(std::to_string(currentAngle)));

	lastLevel = level;

	if (allowTailOff)
	{
		ampEnv.releaseStart();

	}
	else
	{
		// �����[�X��ԂłȂ��Ƃ���angleDelta�����Z�b�g�����Ƀ����[�X��ԂɈڍs
		if (ampEnv.getState() != AmpEnvelope::AMPENV_STATE::RELEASE
			&& ampEnv.getState() != AmpEnvelope::AMPENV_STATE::WAIT)
		{
			ampEnv.releaseStart();
		}
		else if(ampEnv.getState() == AmpEnvelope::AMPENV_STATE::RELEASE)
		{
			// �����[�X��Ԃɓ����Ă����Ƃ���angleDelta�����Z�b�g����
			angleDelta = 0.0f;		
		}

		clearCurrentNote();
	}
}

void SimpleVoice::pitchWheelMoved(int newPitchWheelValue)
{
	// �V���Z�T�C�U�[�N���X����Ă΂��Ƃ��A�L�[�����[�X����allowTailOff == true�A�L�[�����[�X����̃{�C�X�X�`�[���ł�allowTailOff == false
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

	// clearCurrentNote()���s��ł͂����͒ʂ�Ȃ�
	if (SimpleSound* playingSound = static_cast<SimpleSound*>(getCurrentlyPlayingSound().get()))
	{
		if (angleDelta != 0.0f)
		{
			while (--numSamples >= 0) // �ʏ�̃L�[ON���͂�����ʂ�

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
					outputBuffer.addSample(channelNum, startSample, currentSample); //�o�b�t�@�ɑ΂��ĉ��Z�������s���B���Z����Ȃ��ƃ_��

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
					if (ampEnv.getValue() <= 0.005) //�����[�X���\���ɏ��������
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
