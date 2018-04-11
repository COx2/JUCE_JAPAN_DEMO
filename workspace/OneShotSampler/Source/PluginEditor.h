/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class OneshotSamplerAudioProcessorEditor  : public AudioProcessorEditor, private Button::Listener
{
public:
    OneshotSamplerAudioProcessorEditor (OneshotSamplerAudioProcessor&);
    ~OneshotSamplerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void buttonClicked(Button* button) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OneshotSamplerAudioProcessor& processor;
    
    TextButton sampleSelectButton;
    TextButton sineWaveButton;

    MidiKeyboardComponent keyboardComponent;     // MIDIKeyboadComponentクラスのポインタ
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneshotSamplerAudioProcessorEditor)
};
