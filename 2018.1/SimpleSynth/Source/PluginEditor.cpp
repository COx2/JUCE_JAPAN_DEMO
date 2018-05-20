/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleSynthAudioProcessorEditor::SimpleSynthAudioProcessorEditor(SimpleSynthAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
	, keyboardComponent(p.getKeyboardState(), MidiKeyboardComponent::Orientation::horizontalKeyboard)
	, oscParamsComponent(&p.oscParameters)
	, ampEnvParamsComponent(&p.ampEnvParameters)
	, lfoParamsComponent(&p.lfoParameters)
	, filterParamsComponent(&p.filterParameters)
	, reverbParamsComponent(&p.reverbParameters)
	, scopeComponent(p.getAudioBufferQueue())
	, driveSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow)
	, masterVolumeSlider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow)
	, voiceSizeSlider(Slider::SliderStyle::IncDecButtons, Slider::TextEntryBoxPosition::TextBoxBelow)
{

	driveSlider.setRange(processor.driveParameter->range.start, processor.driveParameter->range.end, 0.01);
	driveSlider.setValue(processor.driveParameter->get(), dontSendNotification);
	driveSlider.addListener(this);
	addAndMakeVisible(driveSlider);

	masterVolumeSlider.setRange(processor.masterVolumePrameter->range.start, processor.masterVolumePrameter->range.end, 0.01);
	masterVolumeSlider.setValue(processor.masterVolumePrameter->get(), dontSendNotification);
	masterVolumeSlider.addListener(this);
	addAndMakeVisible(masterVolumeSlider);

	voiceSizeSlider.setRange(processor.voiceSize->getRange().getStart(), processor.voiceSize->getRange().getEnd(), 1.0);
	voiceSizeSlider.setValue(processor.voiceSize->get(), dontSendNotification);
	voiceSizeSlider.addListener(this);
	addAndMakeVisible(voiceSizeSlider);

	keyboardComponent.setKeyWidth(32.0f);
	addAndMakeVisible(keyboardComponent);

	addAndMakeVisible(oscParamsComponent);

	addAndMakeVisible(ampEnvParamsComponent);

	addAndMakeVisible(lfoParamsComponent);

	addAndMakeVisible(filterParamsComponent);

	addAndMakeVisible(reverbParamsComponent);

	addAndMakeVisible(scopeComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (960, 540 + keyboardHeight);

	startTimerHz(30);
}

SimpleSynthAudioProcessorEditor::~SimpleSynthAudioProcessorEditor()
{

}

//==============================================================================
void SimpleSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void SimpleSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	auto bounds = this->getBounds();

	keyboardComponent.setBounds(0.0f, bounds.getHeight() - keyboardHeight, bounds.getWidth(), keyboardHeight);

	oscParamsComponent.setBounds(0.0f, 0.0f, 280.0f, 240.0f);

	ampEnvParamsComponent.setBounds(280.0f, 0.0f, 200.0f, 240.0f);

	lfoParamsComponent.setBounds(480.0f, 0.0f, 160.0f, 540.0f);

	filterParamsComponent.setBounds(640.0, 0.0f, 160.0f, 460.0f);

	driveSlider.setBounds(640.0f, 460.0f, 80.0f, 80.0f);

	reverbParamsComponent.setBounds(800.0f, 0.0f, 80.0f, 540.0f);

	masterVolumeSlider.setBounds(880.0f, 0.0f, 80.0f, 80.0f);
	voiceSizeSlider.setBounds(880.0f, 80.0f, 80.0f, 260.0f);

	scopeComponent.setBounds(0.0f, 240.0f, 480.0f, 300.0f);
}

void SimpleSynthAudioProcessorEditor::sliderValueChanged(Slider * slider)
{
	if (slider == &driveSlider)
	{
		*processor.driveParameter = driveSlider.getValue();
	}
	else if (slider == &masterVolumeSlider)
	{
		*processor.masterVolumePrameter = masterVolumeSlider.getValue();
	}
	else if (slider == &voiceSizeSlider)
	{
		*processor.voiceSize = voiceSizeSlider.getValue();
		processor.changeVoiceSize();
	}
}

void SimpleSynthAudioProcessorEditor::timerCallback()
{
	driveSlider.setValue(processor.driveParameter->get(), dontSendNotification);
	masterVolumeSlider.setValue(processor.masterVolumePrameter->get(), dontSendNotification);
	voiceSizeSlider.setValue(processor.voiceSize->get(), dontSendNotification);
}
