/*
  ==============================================================================

    ParametersComponent.cpp
    Created: 16 May 2018 1:55:55am
    Author:  COx2

  ==============================================================================
*/

#include "ParametersComponent.h"

namespace {
	Colour panelColour = juce::Colour(36, 36, 36);
	Font panelFont = Font(22.0f, Font::plain).withTypefaceStyle("Regular");
}

OscillatorParametersComponent::OscillatorParametersComponent(OscillatorParameters* oscParams)
	: _oscParamsPtr(oscParams)
	, sineWaveLevel(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, sawWaveLevel(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, triWaveLevel(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, squareWaveLevel(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, noiseLevel(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
{

	sineWaveLevel.setRange(_oscParamsPtr->SineWaveLevel->range.start, _oscParamsPtr->SineWaveLevel->range.end, 0.01);
	sineWaveLevel.setValue(_oscParamsPtr->SineWaveLevel->get(), dontSendNotification);
	sineWaveLevel.setPopupDisplayEnabled(true, true, this);
	sineWaveLevel.setPopupMenuEnabled(true);
	sineWaveLevel.addListener(this);
	addAndMakeVisible(sineWaveLevel);

	sawWaveLevel.setRange(_oscParamsPtr->SawWaveLevel->range.start, _oscParamsPtr->SawWaveLevel->range.end, 0.01);
	sawWaveLevel.setValue(_oscParamsPtr->SawWaveLevel->get(), dontSendNotification);
	sawWaveLevel.setPopupDisplayEnabled(true, true, this);
	sawWaveLevel.setPopupMenuEnabled(true);
	sawWaveLevel.addListener(this);
	addAndMakeVisible(sawWaveLevel);

	triWaveLevel.setRange(_oscParamsPtr->TriWaveLevel->range.start, _oscParamsPtr->TriWaveLevel->range.end, 0.01);
	triWaveLevel.setValue(_oscParamsPtr->TriWaveLevel->get(), dontSendNotification);
	triWaveLevel.setPopupDisplayEnabled(true, true, this);
	triWaveLevel.setPopupMenuEnabled(true);
	triWaveLevel.addListener(this);
	addAndMakeVisible(triWaveLevel);

	squareWaveLevel.setRange(_oscParamsPtr->SquareWaveLevel->range.start, _oscParamsPtr->SquareWaveLevel->range.end, 0.01);
	squareWaveLevel.setValue(_oscParamsPtr->SquareWaveLevel->get(), dontSendNotification);
	squareWaveLevel.setPopupDisplayEnabled(true, true, this);
	squareWaveLevel.setPopupMenuEnabled(true);
	squareWaveLevel.addListener(this);
	addAndMakeVisible(squareWaveLevel);


	noiseLevel.setRange(_oscParamsPtr->NoiseLevel->range.start, _oscParamsPtr->NoiseLevel->range.end, 0.01);
	noiseLevel.setValue(_oscParamsPtr->NoiseLevel->get(), dontSendNotification);
	noiseLevel.setPopupDisplayEnabled(true, true, this);
	noiseLevel.setPopupMenuEnabled(true);
	noiseLevel.addListener(this);
	addAndMakeVisible(noiseLevel);

	startTimerHz(30.0f);
}

OscillatorParametersComponent::~OscillatorParametersComponent()
{
}

void OscillatorParametersComponent::paint(Graphics & g)
{
	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}

	//{
	//	float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
	//	Colour strokeColour = Colours::white;
	//	g.setColour(strokeColour);
	//	g.drawRoundedRectangle(x, y, width, height, 10.000f, 4.000f);
	//}

	{
		int x = 0.0f, y = 0.0f, width = getWidth(), height = 32;
		String text(TRANS("OSC"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelFont);
		g.drawText(text, x, y, width, height, Justification::centred, true);
	}

}

void OscillatorParametersComponent::resized()
{
	sineWaveLevel.setBoundsRelative(0.05f, 0.1f, 0.15f, 0.8f);
	sawWaveLevel.setBoundsRelative(0.25f, 0.1f, 0.15f, 0.8f);
	triWaveLevel.setBoundsRelative(0.45f, 0.1f, 0.15f, 0.8f);
	squareWaveLevel.setBoundsRelative(0.65f, 0.1f, 0.15f, 0.8f);
	noiseLevel.setBoundsRelative(0.85f, 0.1f, 0.15f, 0.8f);
}

void OscillatorParametersComponent::timerCallback()
{
	sineWaveLevel.setValue(_oscParamsPtr->SineWaveLevel->get(), dontSendNotification);
	sawWaveLevel.setValue(_oscParamsPtr->SawWaveLevel->get(), dontSendNotification);
	triWaveLevel.setValue(_oscParamsPtr->TriWaveLevel->get(), dontSendNotification);
	squareWaveLevel.setValue(_oscParamsPtr->SquareWaveLevel->get(), dontSendNotification);
	noiseLevel.setValue(_oscParamsPtr->NoiseLevel->get(), dontSendNotification);
}

void OscillatorParametersComponent::sliderValueChanged(Slider* slider)
{
	if (slider == &sineWaveLevel)
	{
		*_oscParamsPtr->SineWaveLevel = sineWaveLevel.getValue();
	}
	else if (slider == &sawWaveLevel)
	{
		*_oscParamsPtr->SawWaveLevel = sawWaveLevel.getValue();
	}
	else if (slider == &triWaveLevel)
	{
		*_oscParamsPtr->TriWaveLevel = triWaveLevel.getValue();
	}
	else if (slider == &squareWaveLevel)
	{
		*_oscParamsPtr->SquareWaveLevel = squareWaveLevel.getValue();
	}
	else if (slider == &noiseLevel)
	{
		*_oscParamsPtr->NoiseLevel = noiseLevel.getValue();
	}
}

AmpEnvelopeParametersComponent::AmpEnvelopeParametersComponent(AmpEnvelopePatameters * ampEnvParams)
	:_ampEnvParamsPtr(ampEnvParams)
	, attackSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, decaySlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, sustainSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, releaseSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
{
	attackSlider.setRange(_ampEnvParamsPtr->Attack->range.start, _ampEnvParamsPtr->Attack->range.end, 0.01);
	attackSlider.setValue(_ampEnvParamsPtr->Attack->get(), dontSendNotification);
	attackSlider.setPopupDisplayEnabled(true, true, this);
	attackSlider.setPopupMenuEnabled(true);
	attackSlider.setTextValueSuffix(" seconds");
	attackSlider.addListener(this);
	addAndMakeVisible(attackSlider);

	decaySlider.setRange(_ampEnvParamsPtr->Decay->range.start, _ampEnvParamsPtr->Decay->range.end, 0.01);
	decaySlider.setValue(_ampEnvParamsPtr->Decay->get(), dontSendNotification);
	decaySlider.setPopupDisplayEnabled(true, true, this);
	decaySlider.setPopupMenuEnabled(true);
	decaySlider.setTextValueSuffix(" seconds");
	decaySlider.addListener(this);
	addAndMakeVisible(decaySlider);

	sustainSlider.setRange(_ampEnvParamsPtr->Sustain->range.start, _ampEnvParamsPtr->Sustain->range.end, 0.01);
	sustainSlider.setValue(_ampEnvParamsPtr->Sustain->get(), dontSendNotification);
	sustainSlider.setPopupDisplayEnabled(true, true, this);
	sustainSlider.setPopupMenuEnabled(true);
	sustainSlider.addListener(this);
	addAndMakeVisible(sustainSlider);

	releaseSlider.setRange(_ampEnvParamsPtr->Release->range.start, _ampEnvParamsPtr->Release->range.end, 0.01);
	releaseSlider.setValue(_ampEnvParamsPtr->Release->get(), dontSendNotification);
	releaseSlider.setPopupDisplayEnabled(true, true, this);
	releaseSlider.setPopupMenuEnabled(true);
	releaseSlider.setTextValueSuffix(" seconds");
	releaseSlider.addListener(this);
	addAndMakeVisible(releaseSlider);

	startTimerHz(30.0f);
}

AmpEnvelopeParametersComponent::~AmpEnvelopeParametersComponent()
{
}

void AmpEnvelopeParametersComponent::paint(Graphics & g)
{
	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.000f);
	}

	//{
	//	float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
	//	Colour strokeColour = Colours::white;
	//	g.setColour(strokeColour);
	//	g.drawRoundedRectangle(x, y, width, height, 10.000f, 4.000f);
	//}

	{
		int x = 0.0f, y = 0.0f, width = getWidth(), height = 32;
		String text(TRANS("AMP"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelFont);
		g.drawText(text, x, y, width, height, Justification::centred, true);
	}

}

void AmpEnvelopeParametersComponent::resized()
{
	attackSlider.setBoundsRelative(0.05f, 0.1f, 0.15f, 0.8f);
	decaySlider.setBoundsRelative(0.25f, 0.1f, 0.15f, 0.8f);
	sustainSlider.setBoundsRelative(0.45f, 0.1f, 0.15f, 0.8f);
	releaseSlider.setBoundsRelative(0.65f, 0.1f, 0.15f, 0.8f);
}

void AmpEnvelopeParametersComponent::timerCallback()
{
	attackSlider.setValue(_ampEnvParamsPtr->Attack->get(), dontSendNotification);
	decaySlider.setValue(_ampEnvParamsPtr->Decay->get(), dontSendNotification);
	sustainSlider.setValue(_ampEnvParamsPtr->Sustain->get(), dontSendNotification);
	releaseSlider.setValue(_ampEnvParamsPtr->Release->get(), dontSendNotification);
}

void AmpEnvelopeParametersComponent::sliderValueChanged(Slider * slider)
{
	if (slider == &attackSlider)
	{
		*_ampEnvParamsPtr->Attack = attackSlider.getValue();
	}
	else if (slider == &decaySlider)
	{
		*_ampEnvParamsPtr->Decay = decaySlider.getValue();
	}
	else if (slider == &sustainSlider)
	{
		*_ampEnvParamsPtr->Sustain = sustainSlider.getValue();
	}
	else if (slider == &releaseSlider)
	{
		*_ampEnvParamsPtr->Release = releaseSlider.getValue();
	}
}

LfoParametersComponent::LfoParametersComponent(LfoParameters * lfoParams)
	:_lfoParamsPtr(lfoParams)
	, targetSelector("LFO-Target")
	, waveTypeSelector("LFO-WaveType")
	, levelSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, speedSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
{

	targetSelector.addItemList(_lfoParamsPtr->LfoTarget->getAllValueStrings(), 1);
	targetSelector.setSelectedItemIndex(_lfoParamsPtr->LfoTarget->getIndex(), dontSendNotification);
	targetSelector.addListener(this);
	addAndMakeVisible(targetSelector);

	waveTypeSelector.addItemList(_lfoParamsPtr->LfoWaveType->getAllValueStrings(), 1);
	waveTypeSelector.setSelectedItemIndex(_lfoParamsPtr->LfoWaveType->getIndex(), dontSendNotification);
	waveTypeSelector.addListener(this);
	addAndMakeVisible(waveTypeSelector);

	levelSlider.setRange(_lfoParamsPtr->LfoLevel->range.start, _lfoParamsPtr->LfoLevel->range.end, 0.01);
	levelSlider.setValue(_lfoParamsPtr->LfoLevel->get(), dontSendNotification);
	levelSlider.setPopupDisplayEnabled(true, true, this);
	levelSlider.addListener(this);
	addAndMakeVisible(levelSlider);

	speedSlider.setRange(_lfoParamsPtr->LfoSpeed->range.start, _lfoParamsPtr->LfoSpeed->range.end, 0.01);
	speedSlider.setValue(_lfoParamsPtr->LfoSpeed->get(), dontSendNotification);
	speedSlider.setSkewFactor(0.5, false);
	speedSlider.setPopupDisplayEnabled(true, true, this);
	speedSlider.setTextValueSuffix(" hz");
	speedSlider.addListener(this);
	addAndMakeVisible(speedSlider);

	startTimerHz(30.0f);
}

LfoParametersComponent::~LfoParametersComponent()
{
}

void LfoParametersComponent::paint(Graphics & g)
{
	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.000f);
	}

	//{
	//	float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
	//	Colour strokeColour = Colours::white;
	//	g.setColour(strokeColour);
	//	g.drawRoundedRectangle(x, y, width, height, 10.000f, 4.000f);
	//}

	{
		int x = 0.0f, y = 0.0f, width = getWidth(), height = 32;
		String text(TRANS("LFO"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelFont);
		g.drawText(text, x, y, width, height, Justification::centred, true);
	}
}

void LfoParametersComponent::resized()
{
	targetSelector.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.2f);
	waveTypeSelector.setBoundsRelative(0.0f, 0.2f, 1.0f, 0.2f);
	levelSlider.setBoundsRelative(0.0f, 0.4f, 1.0f, 0.3f);
	speedSlider.setBoundsRelative(0.0f, 0.7f, 1.0f, 0.3f);
}

void LfoParametersComponent::timerCallback()
{
	targetSelector.setSelectedItemIndex(_lfoParamsPtr->LfoTarget->getIndex(), dontSendNotification);
	waveTypeSelector.setSelectedItemIndex(_lfoParamsPtr->LfoWaveType->getIndex(), dontSendNotification);
	levelSlider.setValue(_lfoParamsPtr->LfoLevel->get(), dontSendNotification);
	speedSlider.setValue(_lfoParamsPtr->LfoSpeed->get(), dontSendNotification);
}

void LfoParametersComponent::sliderValueChanged(Slider* slider)
{
	if (slider == &levelSlider)
	{
		*_lfoParamsPtr->LfoLevel = levelSlider.getValue();
	}
	else if (slider == &speedSlider)
	{
		*_lfoParamsPtr->LfoSpeed = speedSlider.getValue();
	}
}

void LfoParametersComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == &targetSelector)
	{
		*_lfoParamsPtr->LfoTarget = targetSelector.getSelectedItemIndex();
	}
	else if (comboBoxThatHasChanged == &waveTypeSelector)
	{
		*_lfoParamsPtr->LfoWaveType = waveTypeSelector.getSelectedItemIndex();
	}
}

FilterParametersComponent::FilterParametersComponent(FilterPatameters * filterParams)
	:_filterParamsPtr(filterParams)
	, typeSelector("Filter-Type")
	, frequencySlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, qSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
{
	typeSelector.addItemList(_filterParamsPtr->Type->getAllValueStrings(), 1);
	typeSelector.setSelectedItemIndex(_filterParamsPtr->Type->getIndex(), dontSendNotification);
	typeSelector.addListener(this);
	addAndMakeVisible(typeSelector);

	frequencySlider.setRange(_filterParamsPtr->Frequency->range.start, _filterParamsPtr->Frequency->range.end, 0.01);
	frequencySlider.setValue(_filterParamsPtr->Frequency->get(), dontSendNotification);
	frequencySlider.setSkewFactor(0.3, false);
	frequencySlider.setPopupDisplayEnabled(true, true, this);
	frequencySlider.setTextValueSuffix(" hz");
	frequencySlider.addListener(this);
	addAndMakeVisible(frequencySlider);

	qSlider.setRange(_filterParamsPtr->Q->range.start, _filterParamsPtr->Q->range.end, 0.01);
	qSlider.setValue(_filterParamsPtr->Q->get(), dontSendNotification);
	qSlider.setPopupDisplayEnabled(true, true, this);
	qSlider.addListener(this);
	addAndMakeVisible(qSlider);

	startTimerHz(30.0f);
}

FilterParametersComponent::~FilterParametersComponent()
{
}

void FilterParametersComponent::paint(Graphics & g)
{
	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}

	//{
	//	float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
	//	Colour strokeColour = Colours::white;
	//	g.setColour(strokeColour);
	//	g.drawRoundedRectangle(x, y, width, height, 10.000f, 4.000f);
	//}

	{
		int x = 0.0f, y = 0.0f, width = getWidth(), height = 32;
		String text(TRANS("FILTER"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelFont);
		g.drawText(text, x, y, width, height, Justification::centred, true);
	}

}

void FilterParametersComponent::resized()
{
	typeSelector.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.3f);
	frequencySlider.setBoundsRelative(0.0f, 0.3f, 1.0f, 0.3f);
	qSlider.setBoundsRelative(0.0f, 0.6f, 1.0f, 0.3f);
}

void FilterParametersComponent::timerCallback()
{
	typeSelector.setSelectedItemIndex(_filterParamsPtr->Type->getIndex(), dontSendNotification);
	frequencySlider.setValue(_filterParamsPtr->Frequency->get(), dontSendNotification);
	qSlider.setValue(_filterParamsPtr->Q->get(), dontSendNotification);
}

void FilterParametersComponent::sliderValueChanged(Slider* slider)
{
	if (slider == &frequencySlider)
	{
		*_filterParamsPtr->Frequency = frequencySlider.getValue();
	}
	else if (slider == &qSlider)
	{
		*_filterParamsPtr->Q = qSlider.getValue();
	}
}

void FilterParametersComponent::comboBoxChanged(ComboBox * comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == &typeSelector)
	{
		*_filterParamsPtr->Type = typeSelector.getSelectedItemIndex();
	}
}

ReverbParametersComponent::ReverbParametersComponent(ReverbPatameters * reverbParams)
	: _reverbParamsPtr(reverbParams)
	, roomSizeSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, dampingSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, wetLevelSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, dryLevelSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, widthSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, freezeModeSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
{

	roomSizeSlider.setRange(_reverbParamsPtr->RoomSize->range.start, _reverbParamsPtr->RoomSize->range.end, 0.01);
	roomSizeSlider.setValue(_reverbParamsPtr->RoomSize->get(), dontSendNotification);
	roomSizeSlider.setPopupDisplayEnabled(true, true, this);
	roomSizeSlider.addListener(this);
	addAndMakeVisible(roomSizeSlider);

	dampingSlider.setRange(_reverbParamsPtr->Damping->range.start, _reverbParamsPtr->Damping->range.end, 0.01);
	dampingSlider.setValue(_reverbParamsPtr->Damping->get(), dontSendNotification);
	dampingSlider.setPopupDisplayEnabled(true, true, this);
	dampingSlider.addListener(this);
	addAndMakeVisible(dampingSlider);

	wetLevelSlider.setRange(_reverbParamsPtr->WetLevel->range.start, _reverbParamsPtr->WetLevel->range.end, 0.01);
	wetLevelSlider.setValue(_reverbParamsPtr->WetLevel->get(), dontSendNotification);
	wetLevelSlider.setPopupDisplayEnabled(true, true, this);
	wetLevelSlider.addListener(this);
	addAndMakeVisible(wetLevelSlider);

	dryLevelSlider.setRange(_reverbParamsPtr->DryLevel->range.start, _reverbParamsPtr->DryLevel->range.end, 0.01);
	dryLevelSlider.setValue(_reverbParamsPtr->DryLevel->get(), dontSendNotification);
	dryLevelSlider.setPopupDisplayEnabled(true, true, this);
	dryLevelSlider.addListener(this);
	addAndMakeVisible(dryLevelSlider);

	widthSlider.setRange(_reverbParamsPtr->Width->range.start, _reverbParamsPtr->Width->range.end, 0.01);
	widthSlider.setValue(_reverbParamsPtr->Width->get(), dontSendNotification);
	widthSlider.setPopupDisplayEnabled(true, true, this);
	widthSlider.addListener(this);
	addAndMakeVisible(widthSlider);

	startTimerHz(30.0f);
}

ReverbParametersComponent::~ReverbParametersComponent()
{
}

void ReverbParametersComponent::paint(Graphics & g)
{
	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}

	//{
	//	float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
	//	Colour strokeColour = Colours::white;
	//	g.setColour(strokeColour);
	//	g.drawRoundedRectangle(x, y, width, height, 10.000f, 4.000f);
	//}

	{
		int x = 0.0f, y = 0.0f, width = getWidth(), height = 32;
		String text(TRANS("REVERB"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelFont);
		g.drawText(text, x, y, width, height, Justification::centred, true);
	}

}

void ReverbParametersComponent::resized()
{
	roomSizeSlider.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.2f);
	dampingSlider.setBoundsRelative(0.0f, 0.2f, 1.0f, 0.2f);
	widthSlider.setBoundsRelative(0.0f, 0.4f, 1.0f, 0.2f);
	wetLevelSlider.setBoundsRelative(0.0f, 0.6f, 1.0f, 0.2f);
	dryLevelSlider.setBoundsRelative(0.0f, 0.8f, 1.0f, 0.2f);
}

void ReverbParametersComponent::timerCallback()
{
	roomSizeSlider.setValue(_reverbParamsPtr->RoomSize->get(), dontSendNotification);
	dampingSlider.setValue(_reverbParamsPtr->Damping->get(), dontSendNotification);
	wetLevelSlider.setValue(_reverbParamsPtr->WetLevel->get(), dontSendNotification);
	dryLevelSlider.setValue(_reverbParamsPtr->DryLevel->get(), dontSendNotification);
	widthSlider.setValue(_reverbParamsPtr->Width->get(), dontSendNotification);
}

void ReverbParametersComponent::sliderValueChanged(Slider * slider)
{
	if (slider == &roomSizeSlider)
	{
		*_reverbParamsPtr->RoomSize = roomSizeSlider.getValue();
	}
	else if (slider == &dampingSlider)
	{
		*_reverbParamsPtr->Damping = dampingSlider.getValue();
	}
	else if (slider == &wetLevelSlider)
	{
		*_reverbParamsPtr->WetLevel = wetLevelSlider.getValue();
	}
	else if (slider == &dryLevelSlider)
	{
		*_reverbParamsPtr->DryLevel = dryLevelSlider.getValue();
	}
	else if (slider == &widthSlider)
	{
		*_reverbParamsPtr->Width = widthSlider.getValue();
	}
}

DriveParametersComponent::DriveParametersComponent(AudioParameterFloat* driveParam)
	:_driveParamPtr(driveParam)
	, driveSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
{
	driveSlider.setRange(_driveParamPtr->range.start, _driveParamPtr->range.end, 0.01);
	driveSlider.setValue(_driveParamPtr->get(), dontSendNotification);
	driveSlider.setPopupDisplayEnabled(true, true, this);
	driveSlider.setTextValueSuffix(" dB");
	driveSlider.addListener(this);
	addAndMakeVisible(driveSlider);

}

DriveParametersComponent::~DriveParametersComponent()
{
}

void DriveParametersComponent::paint(Graphics & g)
{
	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}

	//{
	//	float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
	//	Colour strokeColour = Colours::white;
	//	g.setColour(strokeColour);
	//	g.drawRoundedRectangle(x, y, width, height, 10.000f, 4.000f);
	//}

	{
		int x = 0.0f, y = 0.0f, width = getWidth(), height = 32;
		String text(TRANS("DRIVE"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelFont);
		g.drawText(text, x, y, width, height, Justification::centred, true);
	}
}

void DriveParametersComponent::resized()
{
	driveSlider.setBounds(0, 40, 80, 80);
}

void DriveParametersComponent::timerCallback()
{
	driveSlider.setValue(_driveParamPtr->get(), dontSendNotification);
}

void DriveParametersComponent::sliderValueChanged(Slider * slider)
{
	if (slider == &driveSlider)
	{
		*_driveParamPtr = driveSlider.getValue();
	}
}

MiscParametersComponent::MiscParametersComponent(AudioParameterFloat* masterVolumeParam, AudioParameterInt* voiceSizeParam)
	:_masterVolumeParamPtr(masterVolumeParam)
	, _voiceSizeParamPtr(voiceSizeParam)
	, masterVolumeSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, voiceSizeSlider(Slider::SliderStyle::IncDecButtons, Slider::TextEntryBoxPosition::TextBoxBelow)
{
	masterVolumeSlider.setRange(_masterVolumeParamPtr->range.start, _masterVolumeParamPtr->range.end, 0.01);
	masterVolumeSlider.setValue(_masterVolumeParamPtr->get(), dontSendNotification);
	masterVolumeSlider.setPopupDisplayEnabled(true, true, this);
	masterVolumeSlider.setTextValueSuffix(" dB");
	masterVolumeSlider.addListener(this);
	addAndMakeVisible(masterVolumeSlider);

	voiceSizeSlider.setRange(_voiceSizeParamPtr->getRange().getStart(), _voiceSizeParamPtr->getRange().getEnd(), 1.0);
	voiceSizeSlider.setValue(_voiceSizeParamPtr->get(), dontSendNotification);
	voiceSizeSlider.setTooltip("Size of synthesiser voice");
	voiceSizeSlider.addListener(this);
	addAndMakeVisible(voiceSizeSlider);
}

MiscParametersComponent::~MiscParametersComponent()
{
}

void MiscParametersComponent::paint(Graphics & g)
{
	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}
}

void MiscParametersComponent::resized()
{
	masterVolumeSlider.setBounds(0, 80, getWidth(), 80);
	voiceSizeSlider.setBounds(0, 160, getWidth(), 80);
}

void MiscParametersComponent::timerCallback()
{
	masterVolumeSlider.setValue(_masterVolumeParamPtr->get(), dontSendNotification);
	voiceSizeSlider.setValue(_voiceSizeParamPtr->get(), dontSendNotification);
}

void MiscParametersComponent::sliderValueChanged(Slider * slider)
{
	if (slider == &masterVolumeSlider)
	{
		*_masterVolumeParamPtr = masterVolumeSlider.getValue();
	}
	else if (slider == &voiceSizeSlider)
	{
		*_voiceSizeParamPtr = voiceSizeSlider.getValue();
	}
}
