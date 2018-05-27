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
	//Font panelNameFont = Font(24.0f, Font::plain).withTypefaceStyle("Italic");   // 内部でnewしてるらしいのでメモリリークする
	//Font paramLabelFont = Font(16.0f, Font::plain).withTypefaceStyle("Regular");
	int panelNameFontSize = 24.0f;
	int panelLabelFontSize = 16.0f;
	int panelNameHeight = 42;
	int localMargin = 2;
}

OscillatorParametersComponent::OscillatorParametersComponent(OscillatorParameters* oscParams)
	: _oscParamsPtr(oscParams)
	, sineWaveLevelSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, sawWaveLevelSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, triWaveLevelSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, squareWaveLevelSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, noiseLevelSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
{
	Font paramLabelFont = Font(16.0f, Font::plain).withTypefaceStyle("Regular");

	sineWaveLevelSlider.setRange(_oscParamsPtr->SineWaveLevel->range.start, _oscParamsPtr->SineWaveLevel->range.end, 0.01);
	sineWaveLevelSlider.setValue(_oscParamsPtr->SineWaveLevel->get(), dontSendNotification);
	sineWaveLevelSlider.setPopupDisplayEnabled(true, true, this);
	sineWaveLevelSlider.setPopupMenuEnabled(true);
	sineWaveLevelSlider.addListener(this);
	addAndMakeVisible(sineWaveLevelSlider);

	sawWaveLevelSlider.setRange(_oscParamsPtr->SawWaveLevel->range.start, _oscParamsPtr->SawWaveLevel->range.end, 0.01);
	sawWaveLevelSlider.setValue(_oscParamsPtr->SawWaveLevel->get(), dontSendNotification);
	sawWaveLevelSlider.setPopupDisplayEnabled(true, true, this);
	sawWaveLevelSlider.setPopupMenuEnabled(true);
	sawWaveLevelSlider.addListener(this);
	addAndMakeVisible(sawWaveLevelSlider);

	triWaveLevelSlider.setRange(_oscParamsPtr->TriWaveLevel->range.start, _oscParamsPtr->TriWaveLevel->range.end, 0.01);
	triWaveLevelSlider.setValue(_oscParamsPtr->TriWaveLevel->get(), dontSendNotification);
	triWaveLevelSlider.setPopupDisplayEnabled(true, true, this);
	triWaveLevelSlider.setPopupMenuEnabled(true);
	triWaveLevelSlider.addListener(this);
	addAndMakeVisible(triWaveLevelSlider);

	squareWaveLevelSlider.setRange(_oscParamsPtr->SquareWaveLevel->range.start, _oscParamsPtr->SquareWaveLevel->range.end, 0.01);
	squareWaveLevelSlider.setValue(_oscParamsPtr->SquareWaveLevel->get(), dontSendNotification);
	squareWaveLevelSlider.setPopupDisplayEnabled(true, true, this);
	squareWaveLevelSlider.setPopupMenuEnabled(true);
	squareWaveLevelSlider.addListener(this);
	addAndMakeVisible(squareWaveLevelSlider);

	noiseLevelSlider.setRange(_oscParamsPtr->NoiseLevel->range.start, _oscParamsPtr->NoiseLevel->range.end, 0.01);
	noiseLevelSlider.setValue(_oscParamsPtr->NoiseLevel->get(), dontSendNotification);
	noiseLevelSlider.setPopupDisplayEnabled(true, true, this);
	noiseLevelSlider.setPopupMenuEnabled(true);
	noiseLevelSlider.addListener(this);
	addAndMakeVisible(noiseLevelSlider);

	sineWaveLevelLabel.setFont(paramLabelFont);
	sineWaveLevelLabel.setText(TRANS("Sine"), dontSendNotification);
	sineWaveLevelLabel.setJustificationType(Justification::centred);
	sineWaveLevelLabel.setEditable(false, false, false);
	addAndMakeVisible(sineWaveLevelLabel);

	sawWaveLevelLabel.setFont(paramLabelFont);
	sawWaveLevelLabel.setText(TRANS("Saw"), dontSendNotification);
	sawWaveLevelLabel.setJustificationType(Justification::centred);
	sawWaveLevelLabel.setEditable(false, false, false);
	addAndMakeVisible(sawWaveLevelLabel);

	squareWaveLevelLabel.setFont(paramLabelFont);
	squareWaveLevelLabel.setText(TRANS("Square"), dontSendNotification);
	squareWaveLevelLabel.setJustificationType(Justification::centred);
	squareWaveLevelLabel.setEditable(false, false, false);
	addAndMakeVisible(squareWaveLevelLabel);

	triWaveLevelLabel.setFont(paramLabelFont);
	triWaveLevelLabel.setText(TRANS("Tri"), dontSendNotification);
	triWaveLevelLabel.setJustificationType(Justification::centred);
	triWaveLevelLabel.setEditable(false, false, false);
	addAndMakeVisible(triWaveLevelLabel);

	noiseLevelLabel.setFont(paramLabelFont);
	noiseLevelLabel.setText(TRANS("Noise"), dontSendNotification);
	noiseLevelLabel.setJustificationType(Justification::centred);
	noiseLevelLabel.setEditable(false, false, false);
	addAndMakeVisible(noiseLevelLabel);

	startTimerHz(30.0f);
}

OscillatorParametersComponent::~OscillatorParametersComponent()
{
}

void OscillatorParametersComponent::paint(Graphics & g)
{
	Font panelNameFont = Font(24.0f, Font::plain).withTypefaceStyle("Italic");

	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}

	{
		Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
		String text(TRANS("OSC MIX"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelNameFont);
		g.drawText(text, bounds.removeFromTop(panelNameHeight).reduced(localMargin), Justification::centred, true);
	}

}

void OscillatorParametersComponent::resized()
{
	float rowSize = 5.0f;
	float divide = 1.0f / rowSize;
	int labelHeight = 20;

	Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
	bounds.removeFromTop(panelNameHeight);
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		sineWaveLevelLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		sineWaveLevelSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		sawWaveLevelLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		sawWaveLevelSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		triWaveLevelLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		triWaveLevelSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		squareWaveLevelLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		squareWaveLevelSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		noiseLevelLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		noiseLevelSlider.setBounds(area.reduced(localMargin));
	}
}

void OscillatorParametersComponent::timerCallback()
{
	sineWaveLevelSlider.setValue(_oscParamsPtr->SineWaveLevel->get(), dontSendNotification);
	sawWaveLevelSlider.setValue(_oscParamsPtr->SawWaveLevel->get(), dontSendNotification);
	triWaveLevelSlider.setValue(_oscParamsPtr->TriWaveLevel->get(), dontSendNotification);
	squareWaveLevelSlider.setValue(_oscParamsPtr->SquareWaveLevel->get(), dontSendNotification);
	noiseLevelSlider.setValue(_oscParamsPtr->NoiseLevel->get(), dontSendNotification);
}

void OscillatorParametersComponent::sliderValueChanged(Slider* slider)
{
	if (slider == &sineWaveLevelSlider)
	{
		*_oscParamsPtr->SineWaveLevel = (float)sineWaveLevelSlider.getValue();
	}
	else if (slider == &sawWaveLevelSlider)
	{
		*_oscParamsPtr->SawWaveLevel = (float)sawWaveLevelSlider.getValue();
	}
	else if (slider == &triWaveLevelSlider)
	{
		*_oscParamsPtr->TriWaveLevel = (float)triWaveLevelSlider.getValue();
	}
	else if (slider == &squareWaveLevelSlider)
	{
		*_oscParamsPtr->SquareWaveLevel = (float)squareWaveLevelSlider.getValue();
	}
	else if (slider == &noiseLevelSlider)
	{
		*_oscParamsPtr->NoiseLevel = (float)noiseLevelSlider.getValue();
	}
}

AmpEnvelopeParametersComponent::AmpEnvelopeParametersComponent(AmpEnvelopePatameters * ampEnvParams)
	:_ampEnvParamsPtr(ampEnvParams)
	, attackSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, decaySlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, sustainSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, releaseSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
{
	Font paramLabelFont = Font(16.0f, Font::plain).withTypefaceStyle("Regular");

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

	attackLabel.setFont(paramLabelFont);
	attackLabel.setText(TRANS("Attack"), dontSendNotification);
	attackLabel.setJustificationType(Justification::centred);
	attackLabel.setEditable(false, false, false);
	addAndMakeVisible(attackLabel);

	decayLabel.setFont(paramLabelFont);
	decayLabel.setText(TRANS("Decay"), dontSendNotification);
	decayLabel.setJustificationType(Justification::centred);
	decayLabel.setEditable(false, false, false);
	addAndMakeVisible(decayLabel);

	sustainLabel.setFont(paramLabelFont);
	sustainLabel.setText(TRANS("Sustain"), dontSendNotification);
	sustainLabel.setJustificationType(Justification::centred);
	sustainLabel.setEditable(false, false, false);
	addAndMakeVisible(sustainLabel);

	releaseLabel.setFont(paramLabelFont);
	releaseLabel.setText(TRANS("Release"), dontSendNotification);
	releaseLabel.setJustificationType(Justification::centred);
	releaseLabel.setEditable(false, false, false);
	addAndMakeVisible(releaseLabel);

	startTimerHz(30.0f);
}

AmpEnvelopeParametersComponent::~AmpEnvelopeParametersComponent()
{
}

void AmpEnvelopeParametersComponent::paint(Graphics & g)
{
	Font panelNameFont = Font(24.0f, Font::plain).withTypefaceStyle("Italic");

	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}

	{
		Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
		String text(TRANS("AMP EG"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelNameFont);
		g.drawText(text, bounds.removeFromTop(panelNameHeight).reduced(localMargin), Justification::centred, true);
	}

}

void AmpEnvelopeParametersComponent::resized()
{
	float rowSize = 4.0f;
	float divide = 1.0f / rowSize;
	int labelHeight = 20;

	Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
	bounds.removeFromTop(panelNameHeight);
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		attackLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		attackSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		decayLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		decaySlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		sustainLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		sustainSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		releaseLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		releaseSlider.setBounds(area.reduced(localMargin));
	}
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
		*_ampEnvParamsPtr->Attack = (float)attackSlider.getValue();
	}
	else if (slider == &decaySlider)
	{
		*_ampEnvParamsPtr->Decay = (float)decaySlider.getValue();
	}
	else if (slider == &sustainSlider)
	{
		*_ampEnvParamsPtr->Sustain = (float)sustainSlider.getValue();
	}
	else if (slider == &releaseSlider)
	{
		*_ampEnvParamsPtr->Release = (float)releaseSlider.getValue();
	}
}

LfoParametersComponent::LfoParametersComponent(LfoParameters * lfoParams)
	:_lfoParamsPtr(lfoParams)
	, targetSelector("LFO-Target")
	, waveTypeSelector("LFO-WaveType")
	, amountSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, speedSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
{
	Font paramLabelFont = Font(16.0f, Font::plain).withTypefaceStyle("Regular");

	targetSelector.addItemList(_lfoParamsPtr->LfoTarget->getAllValueStrings(), 1);
	targetSelector.setSelectedItemIndex(_lfoParamsPtr->LfoTarget->getIndex(), dontSendNotification);
	targetSelector.setJustificationType(Justification::centred);
	targetSelector.addListener(this);
	addAndMakeVisible(targetSelector);

	waveTypeSelector.addItemList(_lfoParamsPtr->LfoWaveType->getAllValueStrings(), 1);
	waveTypeSelector.setSelectedItemIndex(_lfoParamsPtr->LfoWaveType->getIndex(), dontSendNotification);
	waveTypeSelector.setJustificationType(Justification::centred);
	waveTypeSelector.addListener(this);
	addAndMakeVisible(waveTypeSelector);

	amountSlider.setRange(_lfoParamsPtr->LfoAmount->range.start, _lfoParamsPtr->LfoAmount->range.end, 0.01);
	amountSlider.setValue(_lfoParamsPtr->LfoAmount->get(), dontSendNotification);
	amountSlider.setDoubleClickReturnValue(true, _lfoParamsPtr->LfoAmount->range.end);
	amountSlider.setPopupDisplayEnabled(true, true, this);
	amountSlider.setPopupMenuEnabled(true);
	amountSlider.addListener(this);
	addAndMakeVisible(amountSlider);

	speedSlider.setRange(_lfoParamsPtr->LfoSpeed->range.start, _lfoParamsPtr->LfoSpeed->range.end, 0.01);
	speedSlider.setValue(_lfoParamsPtr->LfoSpeed->get(), dontSendNotification);
	speedSlider.setSkewFactor(0.5, false);
	speedSlider.setPopupDisplayEnabled(true, true, this);
	speedSlider.setPopupMenuEnabled(true);
	speedSlider.setTextValueSuffix(" hz");
	speedSlider.addListener(this);
	addAndMakeVisible(speedSlider);

	targetLabel.setFont(paramLabelFont);
	targetLabel.setText(TRANS("Target"), dontSendNotification);
	targetLabel.setJustificationType(Justification::centred);
	targetLabel.setEditable(false, false, false);
	addAndMakeVisible(targetLabel);

	waveTypeLabel.setFont(paramLabelFont);
	waveTypeLabel.setText(TRANS("WaveType"), dontSendNotification);
	waveTypeLabel.setJustificationType(Justification::centred);
	waveTypeLabel.setEditable(false, false, false);
	addAndMakeVisible(waveTypeLabel);

	amountLabel.setFont(paramLabelFont);
	amountLabel.setText(TRANS("Amount"), dontSendNotification);
	amountLabel.setJustificationType(Justification::centred);
	amountLabel.setEditable(false, false, false);
	addAndMakeVisible(amountLabel);

	speedLabel.setFont(paramLabelFont);
	speedLabel.setText(TRANS("Speed"), dontSendNotification);
	speedLabel.setJustificationType(Justification::centred);
	speedLabel.setEditable(false, false, false);
	addAndMakeVisible(speedLabel);
	
	startTimerHz(30.0f);
}

LfoParametersComponent::~LfoParametersComponent()
{
}

void LfoParametersComponent::paint(Graphics & g)
{
	Font panelNameFont = Font(24.0f, Font::plain).withTypefaceStyle("Italic");

	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.000f);
	}

	{
		Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
		String text(TRANS("LFO"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelNameFont);
		g.drawText(text, bounds.removeFromTop(panelNameHeight).reduced(localMargin), Justification::centred, true);
	}
}

void LfoParametersComponent::resized()
{
	float rowSize = 2.0f;
	float divide = 1.0f / rowSize;
	int labelHeight = 20;

	Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
	bounds.removeFromTop(panelNameHeight);

	Rectangle<int> upperArea = bounds.removeFromTop(bounds.getHeight() * 0.5);
	{
		auto area = upperArea.removeFromLeft(getWidth() * divide);
		targetLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		targetSelector.setBounds(area.removeFromTop(labelHeight * 2).reduced(localMargin * 2));
	}
	{
		auto area = upperArea.removeFromLeft(getWidth() * divide);
		amountLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		amountSlider.setBounds(area.reduced(localMargin));
	}

	Rectangle<int> lowerArea = bounds;
	{
		auto area = lowerArea.removeFromLeft(getWidth() * divide);
		waveTypeLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		waveTypeSelector.setBounds(area.removeFromTop(labelHeight * 2).reduced(localMargin * 2));
	}
	{
		auto area = lowerArea.removeFromLeft(getWidth() * divide);
		speedLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		speedSlider.setBounds(area.reduced(localMargin));
	}
}

void LfoParametersComponent::timerCallback()
{
	targetSelector.setSelectedItemIndex(_lfoParamsPtr->LfoTarget->getIndex(), dontSendNotification);
	waveTypeSelector.setSelectedItemIndex(_lfoParamsPtr->LfoWaveType->getIndex(), dontSendNotification);
	amountSlider.setValue(_lfoParamsPtr->LfoAmount->get(), dontSendNotification);
	speedSlider.setValue(_lfoParamsPtr->LfoSpeed->get(), dontSendNotification);
}

void LfoParametersComponent::sliderValueChanged(Slider* slider)
{
	if (slider == &amountSlider)
	{
		*_lfoParamsPtr->LfoAmount = (float)amountSlider.getValue();
	}
	else if (slider == &speedSlider)
	{
		*_lfoParamsPtr->LfoSpeed = (float)speedSlider.getValue();
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
	Font paramLabelFont = Font(16.0f, Font::plain).withTypefaceStyle("Regular");

	typeSelector.addItemList(_filterParamsPtr->Type->getAllValueStrings(), 1);
	typeSelector.setSelectedItemIndex(_filterParamsPtr->Type->getIndex(), dontSendNotification);
	typeSelector.setJustificationType(Justification::centred);
	typeSelector.addListener(this);
	addAndMakeVisible(typeSelector);

	frequencySlider.setRange(_filterParamsPtr->Frequency->range.start, _filterParamsPtr->Frequency->range.end, 0.01);
	frequencySlider.setValue(_filterParamsPtr->Frequency->get(), dontSendNotification);
	frequencySlider.setSkewFactor(0.3, false);
	frequencySlider.setPopupDisplayEnabled(true, true, this);
	frequencySlider.setPopupMenuEnabled(true);
	frequencySlider.setTextValueSuffix(" hz");
	frequencySlider.addListener(this);
	addAndMakeVisible(frequencySlider);

	qSlider.setRange(_filterParamsPtr->Q->range.start, _filterParamsPtr->Q->range.end, 0.01);
	qSlider.setValue(_filterParamsPtr->Q->get(), dontSendNotification);
	qSlider.setPopupDisplayEnabled(true, true, this);
	qSlider.setPopupMenuEnabled(true);
	qSlider.addListener(this);
	addAndMakeVisible(qSlider);

	typeLabel.setFont(paramLabelFont);
	typeLabel.setText(TRANS("Type"), dontSendNotification);
	typeLabel.setJustificationType(Justification::centred);
	typeLabel.setEditable(false, false, false);
	addAndMakeVisible(typeLabel);

	frequencyLabel.setFont(paramLabelFont);
	frequencyLabel.setText(TRANS("Frequency"), dontSendNotification);
	frequencyLabel.setJustificationType(Justification::centred);
	frequencyLabel.setEditable(false, false, false);
	addAndMakeVisible(frequencyLabel);

	qLabel.setFont(paramLabelFont);
	qLabel.setText(TRANS("Q"), dontSendNotification);
	qLabel.setJustificationType(Justification::centred);
	qLabel.setEditable(false, false, false);
	addAndMakeVisible(qLabel);

	startTimerHz(30.0f);
}

FilterParametersComponent::~FilterParametersComponent()
{
}

void FilterParametersComponent::paint(Graphics & g)
{
	Font panelNameFont = Font(24.0f, Font::plain).withTypefaceStyle("Italic");

	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}

	{
		Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
		String text(TRANS("FILTER"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelNameFont);
		g.drawText(text, bounds.removeFromTop(panelNameHeight).reduced(localMargin), Justification::centred, true);
	}

}

void FilterParametersComponent::resized()
{
	float rowSize = 2.0f;
	float divide = 1.0f / rowSize;
	int labelHeight = 20;

	Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
	bounds.removeFromTop(panelNameHeight);

	Rectangle<int> upperArea = bounds.removeFromTop(bounds.getHeight() * 0.5);
	{
		auto area = upperArea;// .removeFromLeft(getWidth() * divide);
		typeLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		typeSelector.setBounds(area.removeFromTop(labelHeight * 2).reduced(localMargin * 2));
	}
	{
		auto area = upperArea.removeFromLeft(getWidth() * divide);
		//frequencyLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		//frequencySlider.setBounds(area.reduced(localMargin));
	}
	Rectangle<int> lowerArea = bounds;
	{
		auto area = lowerArea.removeFromLeft(getWidth() * divide);
		frequencyLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		frequencySlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = lowerArea.removeFromLeft(getWidth() * divide);
		qLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		qSlider.setBounds(area.reduced(localMargin));
	}
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
		*_filterParamsPtr->Frequency = (float)frequencySlider.getValue();
	}
	else if (slider == &qSlider)
	{
		*_filterParamsPtr->Q = (float)qSlider.getValue();
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
	Font paramLabelFont = Font(16.0f, Font::plain).withTypefaceStyle("Regular");

	roomSizeSlider.setRange(_reverbParamsPtr->RoomSize->range.start, _reverbParamsPtr->RoomSize->range.end, 0.01);
	roomSizeSlider.setValue(_reverbParamsPtr->RoomSize->get(), dontSendNotification);
	roomSizeSlider.setPopupDisplayEnabled(true, true, this);
	roomSizeSlider.setPopupMenuEnabled(true);
	roomSizeSlider.addListener(this);
	addAndMakeVisible(roomSizeSlider);

	dampingSlider.setRange(_reverbParamsPtr->Damping->range.start, _reverbParamsPtr->Damping->range.end, 0.01);
	dampingSlider.setValue(_reverbParamsPtr->Damping->get(), dontSendNotification);
	dampingSlider.setPopupDisplayEnabled(true, true, this);
	dampingSlider.setPopupMenuEnabled(true);
	dampingSlider.addListener(this);
	addAndMakeVisible(dampingSlider);

	wetLevelSlider.setRange(_reverbParamsPtr->WetLevel->range.start, _reverbParamsPtr->WetLevel->range.end, 0.01);
	wetLevelSlider.setValue(_reverbParamsPtr->WetLevel->get(), dontSendNotification);
	wetLevelSlider.setPopupDisplayEnabled(true, true, this);
	wetLevelSlider.setPopupMenuEnabled(true);
	wetLevelSlider.addListener(this);
	addAndMakeVisible(wetLevelSlider);

	dryLevelSlider.setRange(_reverbParamsPtr->DryLevel->range.start, _reverbParamsPtr->DryLevel->range.end, 0.01);
	dryLevelSlider.setValue(_reverbParamsPtr->DryLevel->get(), dontSendNotification);
	dryLevelSlider.setPopupDisplayEnabled(true, true, this);
	dryLevelSlider.setPopupMenuEnabled(true);
	dryLevelSlider.addListener(this);
	addAndMakeVisible(dryLevelSlider);

	widthSlider.setRange(_reverbParamsPtr->Width->range.start, _reverbParamsPtr->Width->range.end, 0.01);
	widthSlider.setValue(_reverbParamsPtr->Width->get(), dontSendNotification);
	widthSlider.setPopupDisplayEnabled(true, true, this);
	widthSlider.setPopupMenuEnabled(true);
	widthSlider.addListener(this);
	addAndMakeVisible(widthSlider);

	freezeModeSlider.setRange(_reverbParamsPtr->FreezeMode->range.start, _reverbParamsPtr->FreezeMode->range.end, 0.01);
	freezeModeSlider.setValue(_reverbParamsPtr->FreezeMode->get(), dontSendNotification);
	freezeModeSlider.setPopupDisplayEnabled(true, true, this);
	freezeModeSlider.setPopupMenuEnabled(true);
	freezeModeSlider.addListener(this);
	addAndMakeVisible(freezeModeSlider);

	roomSizeLabel.setFont(paramLabelFont);
	roomSizeLabel.setText(TRANS("RoomSize"), dontSendNotification);
	roomSizeLabel.setJustificationType(Justification::centred);
	roomSizeLabel.setEditable(false, false, false);
	addAndMakeVisible(roomSizeLabel);

	dampingLabel.setFont(paramLabelFont);
	dampingLabel.setText(TRANS("Damping"), dontSendNotification);
	dampingLabel.setJustificationType(Justification::centred);
	dampingLabel.setEditable(false, false, false);
	addAndMakeVisible(dampingLabel);

	wetLevelLabel.setFont(paramLabelFont);
	wetLevelLabel.setText(TRANS("WetLevel"), dontSendNotification);
	wetLevelLabel.setJustificationType(Justification::centred);
	wetLevelLabel.setEditable(false, false, false);
	addAndMakeVisible(wetLevelLabel);

	dryLevelLabel.setFont(paramLabelFont);
	dryLevelLabel.setText(TRANS("DryLevel"), dontSendNotification);
	dryLevelLabel.setJustificationType(Justification::centred);
	dryLevelLabel.setEditable(false, false, false);
	addAndMakeVisible(dryLevelLabel);

	widthLabel.setFont(paramLabelFont);
	widthLabel.setText(TRANS("Width"), dontSendNotification);
	widthLabel.setJustificationType(Justification::centred);
	widthLabel.setEditable(false, false, false);
	addAndMakeVisible(widthLabel);

	freezeModeLabel.setFont(paramLabelFont);
	freezeModeLabel.setText(TRANS("Freeze"), dontSendNotification);
	freezeModeLabel.setJustificationType(Justification::centred);
	freezeModeLabel.setEditable(false, false, false);
	addAndMakeVisible(freezeModeLabel);


	startTimerHz(30.0f);
}

ReverbParametersComponent::~ReverbParametersComponent()
{
}

void ReverbParametersComponent::paint(Graphics & g)
{
	Font panelNameFont = Font(24.0f, Font::plain).withTypefaceStyle("Italic");

	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}

	{
		Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
		String text(TRANS("REVERB"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelNameFont);
		g.drawText(text, bounds.removeFromTop(panelNameHeight).reduced(localMargin), Justification::centred, true);
	}

}

void ReverbParametersComponent::resized()
{
	float rowSize = 3.0f;
	float divide = 1.0f / rowSize;
	int labelHeight = 20;

	Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
	bounds.removeFromTop(panelNameHeight);

	Rectangle<int> upperArea = bounds.removeFromTop(bounds.getHeight() * 0.5);
	{
		auto area = upperArea.removeFromLeft(getWidth() * divide);
		roomSizeLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		roomSizeSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = upperArea.removeFromLeft(getWidth() * divide);
		dampingLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		dampingSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = upperArea.removeFromLeft(getWidth() * divide);
		widthLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		widthSlider.setBounds(area.reduced(localMargin));
	}

	Rectangle<int> lowerArea = bounds;
	{
		auto area = lowerArea.removeFromLeft(getWidth() * divide);
		wetLevelLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		wetLevelSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = lowerArea.removeFromLeft(getWidth() * divide);
		dryLevelLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		dryLevelSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = lowerArea.removeFromLeft(getWidth() * divide);
		freezeModeLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		freezeModeSlider.setBounds(area.reduced(localMargin));
	}
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
		*_reverbParamsPtr->RoomSize = (float)roomSizeSlider.getValue();
	}
	else if (slider == &dampingSlider)
	{
		*_reverbParamsPtr->Damping = (float)dampingSlider.getValue();
	}
	else if (slider == &wetLevelSlider)
	{
		*_reverbParamsPtr->WetLevel = (float)wetLevelSlider.getValue();
	}
	else if (slider == &dryLevelSlider)
	{
		*_reverbParamsPtr->DryLevel = (float)dryLevelSlider.getValue();
	}
	else if (slider == &widthSlider)
	{
		*_reverbParamsPtr->Width = (float)widthSlider.getValue();
	}
}

DriveParametersComponent::DriveParametersComponent(AudioParameterFloat* driveParam)
	:_driveParamPtr(driveParam)
	, gainSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
{
	Font paramLabelFont = Font(16.0f, Font::plain).withTypefaceStyle("Regular");

	gainSlider.setRange(_driveParamPtr->range.start, _driveParamPtr->range.end, 0.01);
	gainSlider.setValue(_driveParamPtr->get(), dontSendNotification);
	gainSlider.setPopupDisplayEnabled(true, true, this);
	gainSlider.setDoubleClickReturnValue(true, 0.0);
	gainSlider.setTextValueSuffix(" dB");
	gainSlider.addListener(this);
	addAndMakeVisible(gainSlider);

	gainLabel.setFont(paramLabelFont);
	gainLabel.setText(TRANS("Gain"), dontSendNotification);
	gainLabel.setJustificationType(Justification::centred);
	gainLabel.setEditable(false, false, false);
	addAndMakeVisible(gainLabel);

	startTimerHz(30.0f);
}

DriveParametersComponent::~DriveParametersComponent()
{
}

void DriveParametersComponent::paint(Graphics & g)
{
	Font panelNameFont = Font(24.0f, Font::plain).withTypefaceStyle("Italic");

	{
		float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
		g.setColour(panelColour);
		g.fillRoundedRectangle(x, y, width, height, 10.0f);
	}

	{
		Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
		String text(TRANS("DRIVE"));
		Colour fillColour = Colours::white;
		g.setColour(fillColour);
		g.setFont(panelNameFont);
		g.drawText(text, bounds.removeFromTop(panelNameHeight).reduced(localMargin), Justification::centred, true);
	}
}

void DriveParametersComponent::resized()
{
	float rowSize = 1.0f;
	float divide = 1.0f / rowSize;
	int labelHeight = 20;

	//	Rectangle<int> bounds = getBounds(); // ウインドウ基準の値

	Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
	bounds.removeFromTop(panelNameHeight);
	{
		auto area = bounds.removeFromLeft(getWidth() * divide);
		gainLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		gainSlider.setBounds(area.reduced(localMargin));
	}
}

void DriveParametersComponent::timerCallback()
{
	gainSlider.setValue(_driveParamPtr->get(), dontSendNotification);
}

void DriveParametersComponent::sliderValueChanged(Slider * slider)
{
	if (slider == &gainSlider)
	{
		*_driveParamPtr = (float)gainSlider.getValue();
	}
}

MiscParametersComponent::MiscParametersComponent(AudioParameterFloat* masterVolumeParam, AudioParameterInt* voiceSizeParam, AudioParameterBool* velocitySenseParam)
	:_masterVolumeParamPtr(masterVolumeParam)
	, _voiceSizeParamPtr(voiceSizeParam)
	, _velocitySenseParamPtr(velocitySenseParam)
	, masterVolumeSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
	, voiceSizeSlider(Slider::SliderStyle::IncDecButtons, Slider::TextEntryBoxPosition::TextBoxRight)
	, velocitySenseButton()
{
	Font paramLabelFont = Font(16.0f, Font::plain).withTypefaceStyle("Regular");

	masterVolumeSlider.setRange(_masterVolumeParamPtr->range.start, _masterVolumeParamPtr->range.end, 0.01);
	masterVolumeSlider.setValue(_masterVolumeParamPtr->get(), dontSendNotification);
	masterVolumeSlider.setPopupDisplayEnabled(true, true, this);
	masterVolumeSlider.setPopupMenuEnabled(true);
	masterVolumeSlider.setDoubleClickReturnValue(true, 0.0);
	masterVolumeSlider.setTextValueSuffix(" dB");
	masterVolumeSlider.addListener(this);
	addAndMakeVisible(masterVolumeSlider);

	voiceSizeSlider.setRange(_voiceSizeParamPtr->getRange().getStart(), _voiceSizeParamPtr->getRange().getEnd(), 1.0);
	voiceSizeSlider.setValue(_voiceSizeParamPtr->get(), dontSendNotification);
	voiceSizeSlider.setTooltip("Size of synthesiser voice");
	voiceSizeSlider.addListener(this);
	addAndMakeVisible(voiceSizeSlider);

	velocitySenseButton.setButtonText(TRANS("Key Velocity Sense"));
	velocitySenseButton.addListener(this);
	addAndMakeVisible(velocitySenseButton);

	masterVolumeLabel.setFont(paramLabelFont);
	masterVolumeLabel.setText(TRANS("Volume"), dontSendNotification);
	masterVolumeLabel.setJustificationType(Justification::centred);
	masterVolumeLabel.setEditable(false, false, false);
	addAndMakeVisible(masterVolumeLabel);

	voiceSizeLabel.setFont(paramLabelFont);
	voiceSizeLabel.setText(TRANS("Voice Size"), dontSendNotification);
	voiceSizeLabel.setJustificationType(Justification::centred);
	voiceSizeLabel.setEditable(false, false, false);
	addAndMakeVisible(voiceSizeLabel);
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
	float columnSize = 7.0f;
	float divide = 1.0f / columnSize;
	int labelHeight = 20;

	//	Rectangle<int> bounds = getBounds(); // ウインドウ基準の値

	Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
	bounds.removeFromTop(4);
	{
		auto area = bounds.removeFromTop(getHeight() * divide * 3);
		masterVolumeLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		masterVolumeSlider.setBounds(area.reduced(localMargin));
	}
	{
		auto area = bounds.removeFromTop(getHeight() * divide * 2);
		voiceSizeLabel.setBounds(area.removeFromTop(labelHeight).reduced(localMargin));
		voiceSizeSlider.setBounds(area.reduced(localMargin));
		voiceSizeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, false, area.getWidth() * 0.2, voiceSizeSlider.getHeight());
	}
	{
		auto area = bounds.removeFromTop(getHeight() * divide * 2);
		velocitySenseButton.setBounds(area.reduced(localMargin));
	}
}

void MiscParametersComponent::timerCallback()
{
	masterVolumeSlider.setValue(_masterVolumeParamPtr->get(), dontSendNotification);
	voiceSizeSlider.setValue(_voiceSizeParamPtr->get(), dontSendNotification);
	velocitySenseButton.setToggleState(_velocitySenseParamPtr->get(), dontSendNotification);
}

void MiscParametersComponent::sliderValueChanged(Slider* slider)
{
	if (slider == &masterVolumeSlider)
	{
		*_masterVolumeParamPtr = (float)masterVolumeSlider.getValue();
	}
	else if (slider == &voiceSizeSlider)
	{
		*_voiceSizeParamPtr = (int)voiceSizeSlider.getValue();
	}
}

void MiscParametersComponent::buttonClicked(Button* button)
{
	if (button == &velocitySenseButton)
	{
		*_velocitySenseParamPtr = velocitySenseButton.getToggleState();
	}
}
