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
#include "GUI/FilmstripSlider.h"
#include "GUI/FilmstripToggleButton.h"

//==============================================================================
/**
*/
class SimpleEqualizerAudioProcessorEditor  : public AudioProcessorEditor,
	private Slider::Listener,
	private Button::Listener,
	private Timer
{
public:
    SimpleEqualizerAudioProcessorEditor (SimpleEqualizerAudioProcessor&);
    ~SimpleEqualizerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleEqualizerAudioProcessor& processor;

	///////////////////////////
	FilmstripSlider Frequency;
	FilmstripSlider BandWidth;
	FilmstripSlider Gain;

	Label FrequencyLabel;
	Label BandWidthLabel;
	Label GainLabel;
	Label BypassLabel;

	FilmstripToggleButton Bypass;

	Image image_knob;
	Image image_bypass;
	Image image_bg;

	void sliderValueChanged(Slider* changedSlider) override;
	void buttonClicked(Button* clickedButton) override;

	void timerCallback() override;
	//////////////////////////


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEqualizerAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
