/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

// ヘッダファイルをインクルードする
#include "../JuceLibraryCode/JuceHeader.h"
#include "DSP/SimpleSynthParameters.h"

// C-1. 5-4.で実装したAudioBufferQueueクラスとScopeDataCollectorクラスを使用するためにヘッダをインクルードする
#include "GUI/ScopeComponent.h"

//==============================================================================
/**
*/
class SimpleSynthAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SimpleSynthAudioProcessor();
    ~SimpleSynthAudioProcessor();

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


    // 継承クラス独自の関数を宣言する
    // ボイス数（同時発音数）を変更するために実行する処理
    void changeVoiceSize();

    // B-2. MidiKeyboardStateクラスのオブジェクトのアドレスを返す関数
    MidiKeyboardState& getKeyboardState() { return keyboardState; }

    // C-3. AudioBufferQueueクラスのオブジェクトのアドレスを返す関数
    AudioBufferQueue<float>& getAudioBufferQueue() { return scopeDataQueue; }

    // Parameterの用意
    const StringArray LFO_TARGETS{ "None", "WaveLevel", "WaveAngle" };
    const StringArray LFO_WAVE_TYPES{ "Sine", "Saw", "Tri", "Square", "Noise" };
    const StringArray FILTER_TYPES{ "Low-Pass", "High-Pass", "Band-Pass" };

    // パラメータ変数の実態を参照するポインタ変数およびクラスのオブジェクトを宣言する。
    OscillatorParameters oscParameters;
    LfoParameters lfoParameters;
    AmpEnvelopeParameters ampEnvParameters;
    FilterParameters filterParameters;
    ReverbParameters reverbParameters;
    AudioParameterFloat* driveParameter;
    AudioParameterFloat* masterVolumePrameter;
    AudioParameterInt* voiceSizeParameter;
    AudioParameterBool* velocitySenseParameter;

private:
    // juce::Synthesiserクラスのオブジェクト
    juce::Synthesiser   synth;

    // エフェクトセクションを構成する各モジュールで使用するDSPオブジェクトを宣言する。
    dsp::ProcessSpec spec;                                  // juce::dspオブジェクトの初期化に必要な情報を保持する構造体

    dsp::WaveShaper<float> clipper, limiter;                // 波形を変形・生成するDSPオブジェクトの宣言

    static float clippingFunction(float inputValue);        // WaveShaper<float>オブジェクトで波形の変形・生成に用いるクリッピング処理の関数

    dsp::Gain<float> drive, masterVolume;                   // 波形の振幅を増減するDSPオブジェクトの宣言

    dsp::Reverb reverb;                                     // リバーブ効果を適用するDSPオブジェクトの宣言

    // マルチチャンネルの入出力に対応したIIRFilterのDSPオブジェクトの宣言
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> iirFilter;

    // GUI上のキーボードコンポーネントで生成されたMIDI情報を保持しておくオブジェクト.
    // B-1. MIDIデバイスおよびキーボードコンポーネントで生成されたMIDIメッセージをマージして保持するオブジェクト
    MidiKeyboardState keyboardState;

    // C-2. 5-4.で実装したAudioBufferQueueクラスとScopeDataCollectorクラスのオブジェクトを宣言する
    AudioBufferQueue<float> scopeDataQueue;
    ScopeDataCollector<float> scopeDataCollector;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSynthAudioProcessor)
};
