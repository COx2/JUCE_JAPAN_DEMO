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
	sampleSelectButton.setButtonText("Sample Select");
	sampleSelectButton.addListener(this);

	// ② テキストボタンコンポーネントの詳細設定
	sineWaveButton.setButtonText("Sine Wave");
	sineWaveButton.addListener(this);

	// ③各GUIコンポーネントを子オブジェクトとして配置・表示する
	addAndMakeVisible(keyboardComponent);
	addAndMakeVisible(sampleSelectButton);
	addAndMakeVisible(sineWaveButton);

	// ④ ウインドウサイズを指定する
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	// コンポーネントの座標とサイズを、ウインドウサイズを基準とするパーセントによる相対値で決定する
	sampleSelectButton.setBoundsRelative(0.6, 0.2, 0.2, 0.2);
	sineWaveButton.setBoundsRelative(0.2, 0.2, 0.2, 0.2);
	keyboardComponent.setBoundsRelative(0.0, 0.7, 1.0, 0.3);
}

void OneShotSamplerAudioProcessorEditor::buttonClicked(Button* button)
{
	// クリックされたボタンがsampleSelectButtonオブジェクトだった場合,関数loadSampleFileを実行する.
	if (button == &sampleSelectButton) {
		processor.loadSampleFile();
	}
	// クリックされたボタンがsineWaveButtonオブジェクトだった場合,関数loadSineWaveを実行する. 
	else if (button == &sineWaveButton) {
		processor.loadSineWave();
	}
}