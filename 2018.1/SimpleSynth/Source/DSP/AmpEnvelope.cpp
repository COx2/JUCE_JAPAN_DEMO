/*
  ==============================================================================

    AmpEnvelope.cpp
    Created: 20 Apr 2018 2:08:26am
    Author:  COx2

  ==============================================================================
*/

#include "AmpEnvelope.h"
#include "JuceHeader.h"

#include <iostream>
#include <string>

namespace {
	const float AMP_MAX = 1.0f;
	const float AMP_MIN = 0.0f;
}

AmpEnvelope::AmpEnvelope(float attackTime, float decayTime, float sustain, float releaseTime)
	: _attackTime(attackTime), _decayTime(decayTime), _sustainValue(sustain), _releaseTime(releaseTime)
	, _sampleRate(0.0f), _value(0.0f), _valueOnReleaseStart(0.0f), _ampState(AMPENV_STATE::WAIT)
{
	if (_attackTime <= 0.1f)
		_attackTime = 0.1f;

	if (_decayTime <= 0.1f)
		_decayTime = 0.1f;

	if (_sustainValue > AMP_MAX)
		_sustainValue = AMP_MAX;

	if (_sustainValue < AMP_MIN)
		_sustainValue = AMP_MIN;

}

AmpEnvelope::~AmpEnvelope()
{
}

void AmpEnvelope::setParameters(float attackTime, float decayTime, float sustain, float releaseTime)
{
	_attackTime = attackTime;
	_decayTime = decayTime;
	_sustainValue = sustain;
	_releaseTime = releaseTime;


	if (_attackTime <= 0.1f)
		_attackTime = 0.1f;

	if (_decayTime <= 0.1f)
		_decayTime = 0.1f;

	if (_sustainValue > AMP_MAX)
		_sustainValue = AMP_MAX;
	
	if (_sustainValue < AMP_MIN)
		_sustainValue = AMP_MIN;
}

void AmpEnvelope::cycle()
{
	// 1サンプルごとのvalue値の変化

	switch (_ampState)
	{
	case AMPENV_STATE::ATTACK:
		_value += AMP_MAX / (_sampleRate * _attackTime);
		if (_value >= AMP_MAX) {
			_value = AMP_MAX;
			_ampState = AMPENV_STATE::DECAY;
		}
		break;

	case AMPENV_STATE::DECAY:
		_value -= AMP_MAX / (_sampleRate * _decayTime);
		if (_value <= _sustainValue) {
			_value = _sustainValue;
			_ampState = AMPENV_STATE::SUSTAIN;
		}
		break;

	case AMPENV_STATE::SUSTAIN:
		_value = _sustainValue;
		break;

	case AMPENV_STATE::RELEASE:
		_value -= _valueOnReleaseStart / (_sampleRate * _releaseTime);
		if (_value <= AMP_MIN) {
			_value = AMP_MIN;
			_ampState = AMPENV_STATE::WAIT;
		}
		break;

	case AMPENV_STATE::WAIT:
		_value = AMP_MIN;
		break;
	}

}

AmpEnvelope::AMPENV_STATE  AmpEnvelope::getState()
{
	return _ampState;
}

float AmpEnvelope::getValue()
{
	return _value;
}

void AmpEnvelope::attackStart(float sampleRate)
{
	DBG("[ADSR] AmpEnvelope::attackStart()");

	if (_ampState != AMPENV_STATE::RELEASE)
	{
		_value = AMP_MIN;
	}

	_sampleRate = sampleRate;

	_ampState = AMPENV_STATE::ATTACK;

	DBG(juce::String("[ADSR] Value:") + juce::String(std::to_string(_value)));
}

void AmpEnvelope::releaseStart()
{
	DBG("[ADSR] AmpEnvelope::releaseStart()");

	if (_ampState != AMPENV_STATE::RELEASE && _ampState != AMPENV_STATE::WAIT) {
		_ampState = AMPENV_STATE::RELEASE;
		_valueOnReleaseStart = _value;
	}

	DBG(juce::String("[ADSR] Value:") + juce::String(std::to_string(_value)));
}

void AmpEnvelope::releaseEnd()
{
	DBG("[ADSR] AmpEnvelope::releaseEnd()");

	_value = AMP_MIN;

	_ampState = AMPENV_STATE::WAIT;

	DBG(juce::String("[ADSR] Value:") + juce::String(std::to_string(_value)));
}
