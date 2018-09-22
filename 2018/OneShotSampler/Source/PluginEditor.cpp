/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OneShotSamplerAudioProcessorEditor::OneShotSamplerAudioProcessorEditor (OneShotSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
	, keyboardComponent(p.getMidiKeyboardState(), MidiKeyboardComponent::horizontalKeyboard)
{
	// ② テキストボタンコンポーネントの詳細設定
	sineWaveButton.setButtonText("Sine Wave");
	sineWaveButton.addListener(this);

	// ② テキストボタンコンポーネントの詳細設定
	sampleSelectButton.setButtonText("Sample Select");
	sampleSelectButton.addListener(this);

	// ③各GUIコンポーネントを子オブジェクトにして表示
	addAndMakeVisible(sineWaveButton);
	addAndMakeVisible(sampleSelectButton);
	addAndMakeVisible(keyboardComponent);

	// ④ ウインドウサイズを指定
	setSize(800, 600);
}

OneShotSamplerAudioProcessorEditor::~OneShotSamplerAudioProcessorEditor()
{
}

//==============================================================================
void OneShotSamplerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void OneShotSamplerAudioProcessorEditor::resized()
{
	// コンポーネントの座標とサイズを、ウインドウサイズを基準とするパーセントによる相対値で決定する
	sineWaveButton.setBoundsRelative(0.2, 0.2, 0.2, 0.2);
	sampleSelectButton.setBoundsRelative(0.6, 0.2, 0.2, 0.2);
	keyboardComponent.setBoundsRelative(0.0, 0.7, 1.0, 0.3);
}

void OneShotSamplerAudioProcessorEditor::buttonClicked(Button* button)
{
	// クリックされたボタンがsineWaveButtonオブジェクトだったら、関数loadSineWaveを実行する。
	if (button == &sineWaveButton) {
		processor.loadSineWave();
	}
	// クリックされたボタンがsampleSelectButtonオブジェクトだったら、関数loadSampleFileを実行する。
	else if (button == &sampleSelectButton) {
		processor.loadSampleFile();
	}

}