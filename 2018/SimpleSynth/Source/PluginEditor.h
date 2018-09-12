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
class SimpleSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SimpleSynthAudioProcessorEditor (SimpleSynthAudioProcessor&);
    ~SimpleSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	SimpleSynthAudioProcessor & processor;

	MidiKeyboardComponent keyboardComponent;
	ScopeComponent<float> scopeComponent;
	OscillatorParametersComponent oscParamsComponent;
	AmpEnvelopeParametersComponent ampEnvParamsComponent;
	LfoParametersComponent lfoParamsComponent;
	FilterParametersComponent filterParamsComponent;
	ReverbParametersComponent reverbParamsComponent;
	DriveParametersComponent driveParamsComponent;
	MiscParametersComponent miscParamsComponent;


	LookAndFeel* customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSynthAudioProcessorEditor)
};
