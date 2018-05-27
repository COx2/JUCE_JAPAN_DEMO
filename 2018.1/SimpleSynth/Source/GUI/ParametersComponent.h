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
	Slider sineWaveLevelSlider;
	Slider sawWaveLevelSlider;
	Slider triWaveLevelSlider;
	Slider squareWaveLevelSlider;
	Slider noiseLevelSlider;

	Label sineWaveLevelLabel;
	Label sawWaveLevelLabel;
	Label triWaveLevelLabel;
	Label squareWaveLevelLabel;
	Label noiseLevelLabel;
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
	Label attackLabel;
	Label decayLabel;
	Label sustainLabel;
	Label releaseLabel;
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
	Slider amountSlider;
	Slider speedSlider;

	Label targetLabel;
	Label waveTypeLabel;
	Label amountLabel;
	Label speedLabel;
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

	Label typeLabel;
	Label frequencyLabel;
	Label qLabel;
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

	Label roomSizeLabel;
	Label dampingLabel;
	Label wetLevelLabel;
	Label dryLevelLabel;
	Label widthLabel;
	Label freezeModeLabel;
};

class DriveParametersComponent : public juce::Component, juce::Slider::Listener, private juce::Timer
{
public:
	DriveParametersComponent(AudioParameterFloat* driveParam);
	~DriveParametersComponent();

	void paint(Graphics& g) override;
	void resized() override;

private:
	DriveParametersComponent();

	virtual void timerCallback() override;
	virtual void sliderValueChanged(Slider* slider) override;

	AudioParameterFloat* _driveParamPtr;
	Slider gainSlider;
	Label gainLabel;
};

class MiscParametersComponent : public juce::Component, juce::Slider::Listener, juce::Button::Listener, private juce::Timer
{
public:
	MiscParametersComponent(AudioParameterFloat* masterVolumeParam, AudioParameterInt* voiceSizeParam, AudioParameterBool* velocitySenseParam);
	~MiscParametersComponent();

	void paint(Graphics& g) override;
	void resized() override;

private:
	MiscParametersComponent();

	virtual void timerCallback() override;
	virtual void sliderValueChanged(Slider* slider) override;
	virtual void buttonClicked(Button* button) override;

	AudioParameterFloat* _masterVolumeParamPtr;
	AudioParameterInt* _voiceSizeParamPtr;
	AudioParameterBool* _velocitySenseParamPtr;

	Slider masterVolumeSlider;
	Slider voiceSizeSlider;
	ToggleButton velocitySenseButton;

	Label masterVolumeLabel;
	Label voiceSizeLabel;
};