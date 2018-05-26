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
	, miscParamsComponent(p.masterVolumePrameter, p.voiceSizeParameter)
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
}

SimpleSynthAudioProcessorEditor::~SimpleSynthAudioProcessorEditor()
{

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
	auto panelMargin = 2;

	Rectangle<int> bounds = this->getBounds();
	keyboardComponent.setBounds(bounds.removeFromBottom(keyboardHeight));

	Rectangle<int> upperArea = bounds.removeFromTop(bounds.getHeight() / 2);
	Rectangle<int> lowerArea = bounds;

	oscParamsComponent.setBounds(upperArea.removeFromLeft(280).reduced(panelMargin));

	ampEnvParamsComponent.setBounds(upperArea.removeFromLeft(200).reduced(panelMargin));

	lfoParamsComponent.setBounds(upperArea.removeFromLeft(240).reduced(panelMargin));

	filterParamsComponent.setBounds(upperArea.removeFromLeft(240).reduced(panelMargin));


	scopeComponent.setBounds(lowerArea.removeFromLeft(400).reduced(panelMargin));

	driveParamsComponent.setBounds(lowerArea.removeFromLeft(80).reduced(panelMargin));
	
	reverbParamsComponent.setBounds(lowerArea.removeFromLeft(240).reduced(panelMargin));

	miscParamsComponent.setBounds(lowerArea.reduced(panelMargin));
}

void SimpleSynthAudioProcessorEditor::timerCallback()
{
}
