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
#include "SimpleSynthParameters.h"

class SimpleVoice : public SynthesiserVoice
{
public:
	//==============================================================================
	/** Creates a voice. */
	SimpleVoice(OscillatorParameters* oscParams, LfoParameters* lfoParams, AmpEnvelopePatameters* ampEnvParams, AudioParameterBool* velocitySenseParam);

	//SimpleVoice(OscillatorParameters* oscParams, AmpEnvelopeParameters* ampEnvParams, bool velocitySense = true);

	/** Destructor. */
	virtual ~SimpleVoice();

	/** Must return true if this voice object is capable of playing the given sound.

	If there are different classes of sound, and different classes of voice, a voice can
	choose which ones it wants to take on.

	A typical implementation of this method may just return true if there's only one type
	of voice and sound, or it might check the type of the sound object passed-in and
	see if it's one that it understands.
	*/
	virtual bool canPlaySound(SynthesiserSound* sound) override;

	/** Called to start a new note.
	This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;

	/** Called to stop a note.

	This will be called during the rendering callback, so must be fast and thread-safe.

	The velocity indicates how quickly the note was released - 0 is slowly, 1 is quickly.

	If allowTailOff is false or the voice doesn't want to tail-off, then it must stop all
	sound immediately, and must call clearCurrentNote() to reset the state of this voice
	and allow the synth to reassign it another sound.

	If allowTailOff is true and the voice decides to do a tail-off, then it's allowed to
	begin fading out its sound, and it can stop playing until it's finished. As soon as it
	finishes playing (during the rendering callback), it must make sure that it calls
	clearCurrentNote().
	*/
	virtual void stopNote(float velocity, bool allowTailOff) override;

	/** Called to let the voice know that the pitch wheel has been moved.
	This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void pitchWheelMoved(int newPitchWheelValue) override;

	/** Called to let the voice know that a midi controller has been moved.
	This will be called during the rendering callback, so must be fast and thread-safe.
	*/
	virtual void controllerMoved(int controllerNumber, int newControllerValue) override;

	//==============================================================================
	/** Renders the next block of data for this voice.

	The output audio data must be added to the current contents of the buffer provided.
	Only the region of the buffer between startSample and (startSample + numSamples)
	should be altered by this method.

	If the voice is currently silent, it should just return without doing anything.

	If the sound that the voice is playing finishes during the course of this rendered
	block, it must call clearCurrentNote(), to tell the synthesiser that it has finished.

	The size of the blocks that are rendered can change each time it is called, and may
	involve rendering as little as 1 sample at a time. In between rendering callbacks,
	the voice's methods will be called to tell it about note and controller events.
	*/
	virtual void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples);


private:
	float calcSineWave(float currentAngle);
	float calcSawWave(float currentAngle);
	float calcTriWave(float currentAngle);
	float calcSquareWave(float currentAngle);

	float angleDelta;
	float currentAngle;
	float pitchBend;
	float lfoAngle;
	float level;
	float lastLevel;
	float levelDiff;

	Random whiteNoise;

	AmpEnvelope ampEnv;

	OscillatorParameters* _oscParamsPtr;
	LfoParameters* _lfoParamsPtr;
	AmpEnvelopePatameters* _ampEnvParamsPtr;
	AudioParameterBool* _velocitySenseParamPtr;
};