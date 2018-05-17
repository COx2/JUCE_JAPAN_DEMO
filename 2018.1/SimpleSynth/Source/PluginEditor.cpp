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
{

	addAndMakeVisible(keyboardComponent);

	addAndMakeVisible(oscParamsComponent);

	addAndMakeVisible(ampEnvParamsComponent);

	addAndMakeVisible(lfoParamsComponent);

	addAndMakeVisible(filterParamsComponent);

	addAndMakeVisible(reverbParamsComponent);

	addAndMakeVisible(scopeComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 800);
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
	keyboardComponent.setBoundsRelative(0.0f, 0.8f, 1.0f, 0.2f);

	oscParamsComponent.setBoundsRelative(0.02f, 0.05f, 0.3f, 0.3f);

	ampEnvParamsComponent.setBoundsRelative(0.7f, 0.05f, 0.25f, 0.3f);

	lfoParamsComponent.setBoundsRelative(0.02f, 0.35f, 0.5f, 0.2f);

	filterParamsComponent.setBoundsRelative(0.6f, 0.35f, 0.4f, 0.2f);

	reverbParamsComponent.setBoundsRelative(0.2f, 0.60f, 0.6f, 0.2f);

	scopeComponent.setBoundsRelative(0.35f, 0.05f, 0.3f, 0.3f);
}
