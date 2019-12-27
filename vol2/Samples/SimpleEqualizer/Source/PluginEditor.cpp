/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SimpleEqualizerAudioProcessorEditor::SimpleEqualizerAudioProcessorEditor (SimpleEqualizerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // 2-5...
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (480, 144);

    Image image_knob = ImageCache::getFromMemory(BinaryData::SimpleEqualizer_knob_png, BinaryData::SimpleEqualizer_knob_pngSize);
    Image image_bypass = ImageCache::getFromMemory(BinaryData::SimpleEqualizer_bypass_png, BinaryData::SimpleEqualizer_bypass_pngSize);

    //Knob
    //Squared strip can count by image_knob.getWidth() / image_knob.getHeight()
    Frequency.setImage(image_knob, image_knob.getWidth() / image_knob.getHeight(), true);
    Frequency.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    Frequency.setRange(0, 1);
    Frequency.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    BandWidth.setImage(image_knob, image_knob.getWidth() / image_knob.getHeight(), true);
    BandWidth.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    BandWidth.setRange(0, 1);
    BandWidth.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    Gain.setImage(image_knob, image_knob.getWidth() / image_knob.getHeight(), true);
    Gain.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    Gain.setRange(0, 1);
    Gain.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);


    // Label
    FrequencyLabel.setFont(Font(13.0f, Font::plain));
    FrequencyLabel.setJustificationType(Justification::centredTop);
    FrequencyLabel.setEditable(false, false, false);
    FrequencyLabel.setColour(Label::backgroundColourId, Colours::black);
    FrequencyLabel.setColour(Label::textColourId, Colours::cyan);
    FrequencyLabel.setText("Frequency\n", dontSendNotification);

    BandWidthLabel.setFont(Font(13.0f, Font::plain));
    BandWidthLabel.setJustificationType(Justification::centredTop);
    BandWidthLabel.setEditable(false, false, false);
    BandWidthLabel.setColour(Label::backgroundColourId, Colours::black);
    BandWidthLabel.setColour(Label::textColourId, Colours::cyan);
    BandWidthLabel.setText("BandWidth\n", dontSendNotification);

    GainLabel.setFont(Font(13.0f, Font::plain));
    GainLabel.setJustificationType(Justification::centredTop);
    GainLabel.setEditable(false, false, false);
    GainLabel.setColour(Label::backgroundColourId, Colours::black);
    GainLabel.setColour(Label::textColourId, Colours::cyan);
    GainLabel.setText("Gain\n", dontSendNotification);

    BypassLabel.setFont(Font(13.0f, Font::plain));
    BypassLabel.setJustificationType(Justification::centredTop);
    BypassLabel.setEditable(false, false, false);
    BypassLabel.setColour(Label::backgroundColourId, Colours::black);
    BypassLabel.setColour(Label::textColourId, Colours::orange);
    BypassLabel.setText("EFFECT", dontSendNotification);

    // Button
    Bypass.setImage(image_bypass, image_bypass.getWidth() / image_bypass.getHeight());

    // addAndMakeVisible

    addAndMakeVisible(&Frequency);
    addAndMakeVisible(&BandWidth);
    addAndMakeVisible(&Gain);
    addAndMakeVisible(&FrequencyLabel);
    addAndMakeVisible(&BandWidthLabel);
    addAndMakeVisible(&GainLabel);
    addAndMakeVisible(&BypassLabel);
    addAndMakeVisible(&Bypass);

    // 2-6...
    // addListener
    Frequency.addListener(this);
    BandWidth.addListener(this);
    Gain.addListener(this);
    Bypass.addListener(this);

    startTimer(30);
}

SimpleEqualizerAudioProcessorEditor::~SimpleEqualizerAudioProcessorEditor()
{
}

//==============================================================================
void SimpleEqualizerAudioProcessorEditor::paint (Graphics& g)
{

    //g.fillAll (Colours::white);
    //g.setColour (Colours::black);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);

    g.fillAll(Colours::black);

    image_bg = ImageCache::getFromMemory(BinaryData::SimpleEqualizer_bg_png, BinaryData::SimpleEqualizer_bg_pngSize);

    g.drawImageWithin(image_bg,
        0, 0,
        image_bg.getWidth(), image_bg.getHeight(),
        RectanglePlacement::yTop,
        false);
}

void SimpleEqualizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int _destX, _destY;
    const int buttonWidth = 64, buttonHeight = 64;
    const int knobWidth = 96, knobHeight = 96;
    const int labelWidth = knobWidth, labelHeight = 32;

    _destX = 24;
    _destY = 8;
    Bypass.setBounds(_destX, _destY+16, buttonWidth, buttonHeight);
    BypassLabel.setBounds(_destX, _destY + knobHeight + 4, buttonWidth, labelHeight);

    _destX += buttonWidth + 32;
    Frequency.setBounds(_destX, _destY, knobWidth, knobHeight);
    FrequencyLabel.setBounds(_destX, _destY + knobHeight + 4, labelWidth, labelHeight);

    _destX += knobWidth + 24;
    BandWidth.setBounds(_destX, _destY, knobWidth, knobHeight);
    BandWidthLabel.setBounds(_destX, _destY + knobHeight + 4, labelWidth, labelHeight);

    _destX += knobWidth + 24;
    Gain.setBounds(_destX, _destY, knobWidth, knobHeight);
    GainLabel.setBounds(_destX, _destY + knobHeight + 4, labelWidth, labelHeight);

}


// ADD ==============================================================================
void SimpleEqualizerAudioProcessorEditor::sliderValueChanged(Slider* changedSlider)
{
    if (changedSlider == &Frequency) {
        processor.setParameterNotifyingHost(SimpleEqualizerAudioProcessor::Frequency, Frequency.getValue());
        FrequencyLabel.setText("Frequency\n"
            + processor.getParameterText(SimpleEqualizerAudioProcessor::Frequency), dontSendNotification);
    }
    else if (changedSlider == &BandWidth) {
        processor.setParameterNotifyingHost(SimpleEqualizerAudioProcessor::BandWidth, BandWidth.getValue());
        BandWidthLabel.setText("BandWidth\n"
            + processor.getParameterText(SimpleEqualizerAudioProcessor::BandWidth), dontSendNotification);

    }
    else if (changedSlider == &Gain) {
        processor.setParameterNotifyingHost(SimpleEqualizerAudioProcessor::Gain, Gain.getValue());
        GainLabel.setText("Gain\n"
            + processor.getParameterText(SimpleEqualizerAudioProcessor::Gain), dontSendNotification);
    }
}

void SimpleEqualizerAudioProcessorEditor::buttonClicked(Button* clickedButton)
{
    if (clickedButton == &Bypass) {
        processor.setParameterNotifyingHost(SimpleEqualizerAudioProcessor::MasterBypass, Bypass.getToggleState());
        BypassLabel.setText(processor.getParameterText(SimpleEqualizerAudioProcessor::MasterBypass), dontSendNotification);
    }
}

void SimpleEqualizerAudioProcessorEditor::timerCallback()
{
    // Set to Knob
    Frequency.setValue(processor.getParameter(SimpleEqualizerAudioProcessor::Frequency), dontSendNotification);
    BandWidth.setValue(processor.getParameter(SimpleEqualizerAudioProcessor::BandWidth), dontSendNotification);
    Gain.setValue(processor.getParameter(SimpleEqualizerAudioProcessor::Gain), dontSendNotification);

    // Set to Button
    Bypass.setToggleState(processor.getParameter(SimpleEqualizerAudioProcessor::MasterBypass) != 1.0f ? false : true, dontSendNotification);

    // Set to Label
    FrequencyLabel.setText("Frequency\n"
        + processor.getParameterText(SimpleEqualizerAudioProcessor::Frequency), dontSendNotification);
    BandWidthLabel.setText("BandWidth\n"
        + processor.getParameterText(SimpleEqualizerAudioProcessor::BandWidth), dontSendNotification);
    GainLabel.setText("Gain\n"
        + processor.getParameterText(SimpleEqualizerAudioProcessor::Gain), dontSendNotification);
    BypassLabel.setText(processor.getParameterText(SimpleEqualizerAudioProcessor::MasterBypass), dontSendNotification);
}
