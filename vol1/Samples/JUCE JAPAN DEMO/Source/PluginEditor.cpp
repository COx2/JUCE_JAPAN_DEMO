/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JuceJapanDemoAudioProcessorEditor::JuceJapanDemoAudioProcessorEditor (JuceJapanDemoAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    // these define the parameters of our slider object
    Gain.setSliderStyle(Slider::LinearBarVertical);
    Gain.setRange(0, 1);
    Gain.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    //Gain.setPopupDisplayEnabled(true, this);
    //Gain.setTextValueSuffix("[dB] Gain");
    //Gain.setSkewFactorFromMidPoint(-20);
    Gain.setValue(0.708);
    
    // these define the parameters of our slider object
    Threshold.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    Threshold.setRange(0, 1);
    Threshold.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    Threshold.setValue(1);
    /*
    Threshold.setRange(-100.0, 0.0, 0.01);
    Threshold.setTextBoxStyle(Slider::TextBoxBelow, false, 90, 20);
    Threshold.setPopupDisplayEnabled(true, this);
    Threshold.setTextValueSuffix("[dB] Threshold");
    Threshold.setSkewFactor(0.5);
    Threshold.setValue(0.0);*/
    
    // these define the parameters of our slider object
    Volume.setSliderStyle(Slider::LinearVertical);
    Volume.setRange(0, 1);
    Volume.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    Volume.setValue(0.708);
    /*
    Volume.setRange(-100.0, 6.0, 0.01);
    Volume.setTextBoxStyle(Slider::TextBoxBelow, false, 60 ,20);
    Volume.setPopupDisplayEnabled(true, this);
    Volume.setTextValueSuffix("[dB] Volume");
    Volume.setSkewFactor(0.5);
    Volume.setValue(0.0);
    */

    GainLabel.setFont(Font(13.00f, Font::plain));
    GainLabel.setJustificationType(Justification::centredTop);
    GainLabel.setEditable(false, false, false);
    GainLabel.setColour(Label::backgroundColourId, Colours::white);
    GainLabel.setColour(Label::textColourId, Colours::black);
    GainLabel.setColour(Label::backgroundColourId, Colour(0x00000000));
    GainLabel.setText("Gain\n", dontSendNotification);

    ThresholdLabel.setFont(Font(13.00f, Font::plain));
    ThresholdLabel.setJustificationType(Justification::centredTop);
    ThresholdLabel.setEditable(false, false, false);
    ThresholdLabel.setColour(Label::backgroundColourId, Colours::white);
    ThresholdLabel.setColour(Label::textColourId, Colours::black);
    ThresholdLabel.setColour(Label::backgroundColourId, Colour(0x00000000));
    ThresholdLabel.setText("Threshold\n", dontSendNotification);

    VolumeLabel.setFont(Font(13.00f, Font::plain));
    VolumeLabel.setJustificationType(Justification::centredTop);
    VolumeLabel.setEditable(false, false, false);
    VolumeLabel.setColour(Label::backgroundColourId, Colours::white);
    VolumeLabel.setColour(Label::textColourId, Colours::black);
    VolumeLabel.setColour(Label::backgroundColourId, Colour(0x00000000));
    VolumeLabel.setText("Volume\n", dontSendNotification);

    Bypass.setButtonText("Bypass");
    Bypass.setColour(ToggleButton::textColourId, Colours::blue);
    Bypass.setColour(ToggleButton::tickDisabledColourId, Colours::blue);
    Bypass.setColour(ToggleButton::tickColourId, Colours::blue);

    // this function adds the slider to the editor
    addAndMakeVisible(&Gain);
    addAndMakeVisible(&Threshold);
    addAndMakeVisible(&Volume);

    //Label
    addAndMakeVisible(&GainLabel);
    addAndMakeVisible(&ThresholdLabel);
    addAndMakeVisible(&VolumeLabel);

    //
    addAndMakeVisible(&Bypass);

    Gain.addListener(this);
    Threshold.addListener(this);
    Volume.addListener(this);
    Bypass.addListener(this);

    //TimerCallback
    startTimer(30);

}

JuceJapanDemoAudioProcessorEditor::~JuceJapanDemoAudioProcessorEditor()
{
}

//==============================================================================
void JuceJapanDemoAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);
    Image image_bg = ImageCache::getFromMemory(BinaryData::JUCE_JAPAN_DEMO_BG_png,BinaryData::JUCE_JAPAN_DEMO_BG_pngSize);
    g.drawImageWithin(image_bg, 0, 0, 400, 300, RectanglePlacement::centred, false);
}

void JuceJapanDemoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // sets the position and size of the slider with arguments (x, y, width, height)
    Gain.setBounds(40, 30, 60, 150);
    Threshold.setBounds(140, 30, 120, 150);
    Volume.setBounds(300, 30, 60, 150);
    Bypass.setBounds(getWidth()*0.2f, getHeight() - 60, getWidth()*0.6f, 30);

    //Label
    GainLabel.setBounds(40, 190, 60, 30);
    ThresholdLabel.setBounds(140, 190, 120, 30);
    VolumeLabel.setBounds(300, 190, 60, 30);
}

void JuceJapanDemoAudioProcessorEditor::sliderValueChanged(Slider *changedSlider)
{
    if (changedSlider == &Gain)
    {
        processor.setParameterNotifyingHost(JuceJapanDemoAudioProcessor::Gain, Gain.getValue());
        GainLabel.setText("Gain\n" + processor.getParameterText(JuceJapanDemoAudioProcessor::Gain), dontSendNotification);
    }
    else if (changedSlider == &Threshold)
    {
        processor.setParameterNotifyingHost(JuceJapanDemoAudioProcessor::Threshold, Threshold.getValue());
        ThresholdLabel.setText("Threshold\n" + processor.getParameterText(JuceJapanDemoAudioProcessor::Threshold), dontSendNotification);
    }
    else if (changedSlider == &Volume)
    {
        processor.setParameterNotifyingHost(JuceJapanDemoAudioProcessor::Volume, Volume.getValue());
        VolumeLabel.setText("Volume\n" + processor.getParameterText(JuceJapanDemoAudioProcessor::Volume), dontSendNotification);
    }
}

void JuceJapanDemoAudioProcessorEditor::buttonClicked(Button * clickedButton)
{
    if (clickedButton == &Bypass)
    {
        processor.setParameterNotifyingHost(JuceJapanDemoAudioProcessor::MasterBypass, Bypass.getToggleState());
    }
}

void JuceJapanDemoAudioProcessorEditor::timerCallback()
{
    Gain.setValue(processor.getParameter(JuceJapanDemoAudioProcessor::Gain), dontSendNotification);
    Threshold.setValue(processor.getParameter(JuceJapanDemoAudioProcessor::Threshold), dontSendNotification);
    Volume.setValue(processor.getParameter(JuceJapanDemoAudioProcessor::Volume), dontSendNotification);
    Bypass.setToggleState(processor.getParameter(JuceJapanDemoAudioProcessor::MasterBypass) == 1.0f ? true : false, dontSendNotification);

    GainLabel.setText("Gain\n" + processor.getParameterText(JuceJapanDemoAudioProcessor::Gain), dontSendNotification);
    ThresholdLabel.setText("Threshold\n" + processor.getParameterText(JuceJapanDemoAudioProcessor::Threshold), dontSendNotification);
    VolumeLabel.setText("Volume\n" + processor.getParameterText(JuceJapanDemoAudioProcessor::Volume), dontSendNotification);
}

