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

	// シンセサイザーのセットアップ処理
	// サンプル音源のロード、SamplerSoundオブジェクトの生成とセット、SamplerVoiceオブジェクトの生成とセットを行う
	void setupSampler(AudioFormatReader& newReader);

	// サンプル音源をファイルブラウザから選択して読み込み、セットアップ処理を実行する
	void loadSampleFile();

	// サンプル音源をバイナリデータのサイン波を読み込み、セットアップ処理を実行する
	void loadSineWave();

	// MidiKeyboardStateオブジェクトのポインタを返す
	MidiKeyboardState& getMidiKeyboardState() { return keyboardState; };

private:
	Synthesiser synth;								// Synthesiserクラスのインスタンス
	MidiKeyboardState keyboardState;				// MidiKeyboardStateクラスのインスタンス
	bool isChanging;									// processBlockの処理をスキップするかどうかのフラグ

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneShotSamplerAudioProcessor)
};
