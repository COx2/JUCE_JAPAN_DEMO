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
public:
	enum class AMPENV_STATE
	{
		ATTACK = 0,
		DECAY,
		SUSTAIN,
		RELEASE,
		WAIT,
	};

	AmpEnvelope(float attackTime, float decayTime, float sustain, float releaseTime);
	~AmpEnvelope();

	AMPENV_STATE getState();
	float getValue();
	void setParameters(float attackTime, float decayTime, float sustain, float releaseTime);
	void attackStart(float sampleRate);
	void releaseStart();
	void releaseEnd();
	bool isHolding();
	bool isReleasing();
	void cycle();

private:
	AmpEnvelope();

	float _attackTime, _decayTime, _sustainValue, _releaseTime;
	float _value, _valueOnReleaseStart;
	float _sampleRate;

	AMPENV_STATE _ampState;
};