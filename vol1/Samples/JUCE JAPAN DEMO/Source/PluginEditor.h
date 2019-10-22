/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class JuceJapanDemoAudioProcessorEditor : public AudioProcessorEditor,
    private Slider::Listener,
    private Button::Listener,
    private Timer
{
public:
    JuceJapanDemoAudioProcessorEditor (JuceJapanDemoAudioProcessor&);
    ~JuceJapanDemoAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JuceJapanDemoAudioProcessor& processor;

    Slider Gain;
    Slider Threshold;
    Slider Volume;

    Label GainLabel;
    Label ThresholdLabel;
    Label VolumeLabel;

    ToggleButton Bypass;

    void sliderValueChanged(Slider *changedSlider) override;
    void buttonClicked(Button* clickedButton) override;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceJapanDemoAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
