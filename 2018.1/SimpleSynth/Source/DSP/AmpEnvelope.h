/*
  ==============================================================================

    AmpEnvelope.h
    Created: 20 Apr 2018 2:08:26am
    Author:  COx2

  ==============================================================================
*/

#pragma once

class AmpEnvelope 
{
	friend class SimpleVoice;

	enum class AMPENV_STATE 
	{
		ATTACK = 0,
		DECAY,
		SUSTAIN,
		RELEASE,
		WAIT,
	};

public:
	AmpEnvelope(float attackTime, float decayTime, float sustain, float releaseTime);
	~AmpEnvelope();

	void setParameters(float attackTime, float decayTime, float sustain, float releaseTime);
	void cycle();
	float getValue();
	void attackStart(float sampleRate);
	void releaseStart();
	void releaseEnd();
	AMPENV_STATE getState();

private:
	AmpEnvelope();

	float _attackTime, _decayTime, _sustainValue, _releaseTime;
	float _value, _valueOnReleaseStart;
	float _sampleRate;

	AMPENV_STATE _ampState;
};