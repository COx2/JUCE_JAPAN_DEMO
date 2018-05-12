/*
  ==============================================================================

    SimpleSynthParameters.h
    Created: 10 May 2018 12:28:18am
    Author:  COx2

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
struct SynthParametersBase
{
	virtual ~SynthParametersBase() {};

	virtual void addAllParameters(AudioProcessor& processor) = 0;
	virtual void saveParameters(XmlElement& xml) = 0; 
	virtual void loadParameters(XmlElement& xml) = 0;
};

struct OscillatorParameters : public SynthParametersBase
{
	AudioParameterFloat* SineWaveLevel;
	AudioParameterFloat* SawWaveLevel;
	AudioParameterFloat* TriWaveLevel;
	AudioParameterFloat* SquareWaveLevel;
	AudioParameterFloat* NoiseLevel;

	OscillatorParameters(AudioParameterFloat* sineWaveLevel,
		AudioParameterFloat* sawWaveLevel,
		AudioParameterFloat* triWaveLevel,
		AudioParameterFloat* squareWaveLevel,
		AudioParameterFloat* noiseLevel);

	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	OscillatorParameters() {};
};

struct AmpEnvelopePatameters : public SynthParametersBase
{
	AudioParameterFloat* Attack;
	AudioParameterFloat* Decay;
	AudioParameterFloat* Sustain;
	AudioParameterFloat* Release;

	AmpEnvelopePatameters(AudioParameterFloat* attack,
		AudioParameterFloat* decay,
		AudioParameterFloat* sustain,
		AudioParameterFloat* release);

	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	AmpEnvelopePatameters() {};
};

struct LfoParameters : public SynthParametersBase
{
	AudioParameterChoice* LfoTarget;
	AudioParameterFloat*  LfoLevel;
	AudioParameterFloat*  LfoSpeed;

	LfoParameters(AudioParameterChoice* lfoTarget,
				AudioParameterFloat*  lfoLevel,
				AudioParameterFloat*  lfoSpeed);

	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	LfoParameters() {};
};

struct FilterPatameters : public SynthParametersBase
{
	AudioParameterChoice* Type;
	AudioParameterFloat*  Frequency;
	AudioParameterFloat*  Q;

	FilterPatameters(AudioParameterChoice* type,
		AudioParameterFloat*  frequency,
		AudioParameterFloat*  q);

	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	FilterPatameters() {};
};

struct ReverbPatameters : public SynthParametersBase
{
	AudioParameterFloat*  RoomSize;
	AudioParameterFloat*  Damping;
	AudioParameterFloat*  WetLevel;
	AudioParameterFloat*  DryLevel;
	AudioParameterFloat*  Width;
	AudioParameterFloat*  FreezeMode;

	ReverbPatameters(AudioParameterFloat*  roomSize,
				AudioParameterFloat*  damping,
				AudioParameterFloat*  wetLevel,
				AudioParameterFloat*  dryLevel,
				AudioParameterFloat*  width,
				AudioParameterFloat*  freezeMode);

	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	ReverbPatameters() {};
};