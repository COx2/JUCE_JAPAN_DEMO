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
class OneShotSamplerAudioProcessorEditor  : public AudioProcessorEditor, private Button::Listener

{
public:
    OneShotSamplerAudioProcessorEditor (OneShotSamplerAudioProcessor&);
    ~OneShotSamplerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	void buttonClicked(Button* button) override;	// Button::Listenerクラスのイベントハンドラをオーバーライドする

	TextButton sineWaveButton;						// TextButtonクラスのインスタンス（サイン波選択用）
	TextButton sampleSelectButton;					// TextButtonクラスのインスタンス（サンプル音源選択用）
	MidiKeyboardComponent keyboardComponent;		// MidiKeyboadComponentクラスのインスタンス

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OneShotSamplerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneShotSamplerAudioProcessorEditor)
};
