/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class OneShotSamplerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    OneShotSamplerAudioProcessor();
    ~OneShotSamplerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	// �V���Z�T�C�U�[�̃Z�b�g�A�b�v����
	// �T���v�������̃��[�h�ASamplerSound�I�u�W�F�N�g�̐����ƃZ�b�g�ASamplerVoice�I�u�W�F�N�g�̐����ƃZ�b�g���s��
	void setupSampler(AudioFormatReader& newReader);

	// �T���v���������t�@�C���u���E�U����I�����ēǂݍ��݁A�Z�b�g�A�b�v���������s����
	void loadSampleFile();

	// �T���v���������o�C�i���f�[�^�̃T�C���g��ǂݍ��݁A�Z�b�g�A�b�v���������s����
	void loadSineWave();

	// MidiKeyboardState�I�u�W�F�N�g�̃|�C���^��Ԃ�
	MidiKeyboardState& getMidiKeyboardState() { return keyboardState; };

private:
	Synthesiser synth;								// Synthesiser�N���X�̃C���X�^���X
	MidiKeyboardState keyboardState;				// MidiKeyboardState�N���X�̃C���X�^���X
	bool isChanging;									// processBlock�̏������X�L�b�v���邩�ǂ����̃t���O

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneShotSamplerAudioProcessor)
};
