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
	, driveParamsComponent(p.driveParameter)
	, miscParamsComponent(p.masterVolumePrameter, p.voiceSizeParameter, p.velocitySenseParameter)
	, scopeComponent(p.getAudioBufferQueue())
{

	keyboardComponent.setKeyWidth(32.0f);

	addAndMakeVisible(keyboardComponent);
	addAndMakeVisible(oscParamsComponent);
	addAndMakeVisible(ampEnvParamsComponent);
	addAndMakeVisible(lfoParamsComponent);
	addAndMakeVisible(filterParamsComponent);
	addAndMakeVisible(driveParamsComponent);
	addAndMakeVisible(reverbParamsComponent);
	addAndMakeVisible(miscParamsComponent);
	addAndMakeVisible(scopeComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setSize (960, 540 + keyboardHeight);

	startTimerHz(30);

	// assign custom look and feel.
	customLookAndFeel = new LookAndFeel_V4(LookAndFeel_V4::getGreyColourScheme());
	customLookAndFeel->setColour(TooltipWindow::ColourIds::textColourId, Colours::white);
	customLookAndFeel->setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::mediumvioletred);
	customLookAndFeel->setColour(Slider::ColourIds::trackColourId, Colours::yellow);

	for (Component* child : getChildren()) {
		child->setLookAndFeel(customLookAndFeel);
	}
}

SimpleSynthAudioProcessorEditor::~SimpleSynthAudioProcessorEditor()
{
	for (Component* child : getChildren()) {
		child->setLookAndFeel(nullptr);
	}

	delete customLookAndFeel;
}

//==============================================================================
void SimpleSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colours::black);

}

void SimpleSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	int panelMargin = 2;

	Rectangle<int> bounds = this->getBounds();
	keyboardComponent.setBounds(bounds.removeFromBottom(keyboardHeight));

	Rectangle<int> upperArea = bounds.removeFromTop(bounds.getHeight() * 0.5);
	{
		oscParamsComponent.setBounds(upperArea.removeFromLeft(280).reduced(panelMargin));
		lfoParamsComponent.setBounds(upperArea.removeFromLeft(240).reduced(panelMargin));
		ampEnvParamsComponent.setBounds(upperArea.removeFromLeft(240).reduced(panelMargin));
		filterParamsComponent.setBounds(upperArea.reduced(panelMargin));
	}

	Rectangle<int> lowerArea = bounds;
	{
		scopeComponent.setBounds(lowerArea.removeFromLeft(420).reduced(panelMargin));
		driveParamsComponent.setBounds(lowerArea.removeFromLeft(100).reduced(panelMargin));
		reverbParamsComponent.setBounds(lowerArea.removeFromLeft(280).reduced(panelMargin));
		miscParamsComponent.setBounds(lowerArea.reduced(panelMargin));
	}
}

void SimpleSynthAudioProcessorEditor::timerCallback()
{}
