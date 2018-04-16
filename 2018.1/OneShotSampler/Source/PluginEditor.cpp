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
	// �A �e�L�X�g�{�^���R���|�[�l���g�̏ڍאݒ�
	sampleSelectButton.setButtonText("Sample Select");
	sampleSelectButton.addListener(this);

	// �A �e�L�X�g�{�^���R���|�[�l���g�̏ڍאݒ�
	sineWaveButton.setButtonText("Sine Wave");
	sineWaveButton.addListener(this);

	// �B�eGUI�R���|�[�l���g���q�I�u�W�F�N�g�Ƃ��Ĕz�u�E�\������
	addAndMakeVisible(keyboardComponent);
	addAndMakeVisible(sampleSelectButton);
	addAndMakeVisible(sineWaveButton);

	// �C �E�C���h�E�T�C�Y���w�肷��
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

	// �R���|�[�l���g�̍��W�ƃT�C�Y���A�E�C���h�E�T�C�Y����Ƃ���p�[�Z���g�ɂ�鑊�Βl�Ō��肷��
	sampleSelectButton.setBoundsRelative(0.6, 0.2, 0.2, 0.2);
	sineWaveButton.setBoundsRelative(0.2, 0.2, 0.2, 0.2);
	keyboardComponent.setBoundsRelative(0.0, 0.7, 1.0, 0.3);
}

void OneShotSamplerAudioProcessorEditor::buttonClicked(Button* button)
{
	// �N���b�N���ꂽ�{�^����sampleSelectButton�I�u�W�F�N�g�������ꍇ,�֐�loadSampleFile�����s����.
	if (button == &sampleSelectButton) {
		processor.loadSampleFile();
	}
	// �N���b�N���ꂽ�{�^����sineWaveButton�I�u�W�F�N�g�������ꍇ,�֐�loadSineWave�����s����. 
	else if (button == &sineWaveButton) {
		processor.loadSineWave();
	}
}