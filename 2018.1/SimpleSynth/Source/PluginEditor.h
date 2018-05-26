/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "GUI/ParametersComponent.h"
#include "GUI/ScopeComponent.h"

//==============================================================================
/**
*/
class SimpleSynthAudioProcessorEditor  : public AudioProcessorEditor, private juce::Timer
{
public:
    SimpleSynthAudioProcessorEditor (SimpleSynthAudioProcessor&);
    ~SimpleSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	virtual void timerCallback() override;

	float keyboardHeight{80.0f};

	MidiKeyboardComponent keyboardComponent;

	ScopeComponent<float> scopeComponent;

	OscillatorParametersComponent oscParamsComponent;

	AmpEnvelopeParametersComponent ampEnvParamsComponent;

	LfoParametersComponent lfoParamsComponent;

	FilterParametersComponent filterParamsComponent;
	
	ReverbParametersComponent reverbParamsComponent;

	DriveParametersComponent driveParamsComponent;

	MiscParametersComponent miscParamsComponent;

	Slider masterVolumeSlider;
	Slider voiceSizeSlider;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleSynthAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSynthAudioProcessorEditor)
};
