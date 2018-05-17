/*
  ==============================================================================

    ParametersComponent.h
    Created: 16 May 2018 1:55:55am
    Author:  COx2

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../DSP/SimpleSynthParameters.h"

class OscillatorParametersComponent : public juce::Component, juce::Slider::Listener
									, private juce::Timer
{
public:
	OscillatorParametersComponent(OscillatorParameters* oscParams);
	~OscillatorParametersComponent();

	void paint(Graphics& g) override;
	void resized() override;
	
private:
	OscillatorParametersComponent();

	virtual void timerCallback() override;
	virtual void sliderValueChanged(Slider* slider) override;

	OscillatorParameters* _oscParamsPtr;
	Slider sineWaveLevel;
	Slider sawWaveLevel;
	Slider triWaveLevel;
	Slider squareWaveLevel;
	Slider noiseLevel;
};


class AmpEnvelopeParametersComponent : public juce::Component, juce::Slider::Listener, private juce::Timer
{
public:
	AmpEnvelopeParametersComponent(AmpEnvelopePatameters* ampEnvParams);
	~AmpEnvelopeParametersComponent();

	void paint(Graphics& g) override;
	void resized() override;

private:
	AmpEnvelopeParametersComponent();

	virtual void timerCallback() override;
	virtual void sliderValueChanged(Slider* slider) override;

	AmpEnvelopePatameters* _ampEnvParamsPtr;
	Slider attackSlider;
	Slider decaySlider;
	Slider sustainSlider;
	Slider releaseSlider;
};


class LfoParametersComponent : public juce::Component, juce::Slider::Listener, juce::ComboBox::Listener, private juce::Timer
{
public:
	LfoParametersComponent(LfoParameters* lfoParams);
	~LfoParametersComponent();

	void paint(Graphics& g) override;
	void resized() override;

private:
	LfoParametersComponent();

	virtual void timerCallback() override;
	virtual void sliderValueChanged(Slider* slider) override;
	virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

	LfoParameters* _lfoParamsPtr;
	ComboBox targetSelector;
	ComboBox waveTypeSelector;
	Slider levelSlider;
	Slider speedSlider;
};

class FilterParametersComponent : public juce::Component, juce::Slider::Listener, juce::ComboBox::Listener, private juce::Timer
{
public:
	FilterParametersComponent(FilterPatameters* filterParams);
	~FilterParametersComponent();

	void paint(Graphics& g) override;
	void resized() override;

private:
	FilterParametersComponent();

	virtual void timerCallback() override;
	virtual void sliderValueChanged(Slider* slider) override;
	virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

	FilterPatameters* _filterParamsPtr;
	ComboBox typeSelector;
	Slider frequencySlider;
	Slider qSlider;
};

class ReverbParametersComponent : public juce::Component, juce::Slider::Listener, private juce::Timer
{
public:
	ReverbParametersComponent(ReverbPatameters* reverbParams);
	~ReverbParametersComponent();

	void paint(Graphics& g) override;
	void resized() override;

private:
	ReverbParametersComponent();

	virtual void timerCallback() override;
	virtual void sliderValueChanged(Slider* slider) override;

	ReverbPatameters* _reverbParamsPtr;
	Slider roomSizeSlider;
	Slider dampingSlider;
	Slider wetLevelSlider;
	Slider dryLevelSlider;
	Slider widthSlider;
	Slider freezeModeSlider;
};