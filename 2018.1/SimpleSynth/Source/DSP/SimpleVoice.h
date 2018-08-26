/*
  ==============================================================================

    SimpleVoice.h
    Created: 20 Apr 2018 2:08:09am
    Author:  COx2

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SimpleSound.h"
#include "AmpEnvelope.h"
#include "Waveforms.h"
#include "SimpleSynthParameters.h"

class SimpleVoice : public SynthesiserVoice
{
public:
	//==============================================================================
	/** Creates a voice. */
	SimpleVoice(OscillatorParameters* oscParams, LfoParameters* lfoParams, AmpEnvelopePatameters* ampEnvParams, AudioParameterBool* velocitySenseParam);

	/** Destructor. */
	virtual ~SimpleVoice();

	virtual bool canPlaySound(SynthesiserSound* sound) override;

	virtual void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;

	virtual void stopNote(float velocity, bool allowTailOff) override;

	virtual void pitchWheelMoved(int newPitchWheelValue) override;

	virtual void controllerMoved(int controllerNumber, int newControllerValue) override;

	virtual void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples);


private:
	float calcModulationFactor(float angle);

	float angleDelta;
	float currentAngle;
    float lfoAngle;
	float pitchBend;
	float level;
	float lastLevel;
	float levelDiff;

    Waveforms waveForms;
	AmpEnvelope ampEnv;

	OscillatorParameters* _oscParamsPtr;
	LfoParameters* _lfoParamsPtr;
	AmpEnvelopePatameters* _ampEnvParamsPtr;
	AudioParameterBool* _velocitySenseParamPtr;
};
