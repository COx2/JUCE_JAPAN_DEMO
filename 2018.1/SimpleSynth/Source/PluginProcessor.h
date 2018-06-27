/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DSP/SimpleSynthParameters.h"
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


	//==============================================================================
	MidiKeyboardState& getKeyboardState() { return keyboardState; }

	AudioBufferQueue<float>& getAudioBufferQueue() { return scopeDataQueue; }

	void changeVoiceSize();

	//Parameterの用意[1]
	const StringArray LFO_TARGETS{ "None", "WaveLevel", "WaveAngle" };
	const StringArray LFO_WAVE_TYPES{ "Sine", "Saw", "Tri", "Square", "Noise" };
	const StringArray FILTER_TYPES{ "Low-Pass", "High-Pass", "Band-Pass" };

	OscillatorParameters oscParameters;
	LfoParameters lfoParameters;
	AmpEnvelopePatameters ampEnvParameters;
	FilterPatameters filterParameters;
	ReverbPatameters reverbParameters;
	AudioParameterFloat* driveParameter;
	AudioParameterFloat* masterVolumePrameter;
	AudioParameterInt* voiceSizeParameter;
	AudioParameterBool* velocitySenseParameter;

private:
	Synthesiser		  synth;

	//歪み用の関数[2]
	static float clippingFunction(float inputValue);

	//[3]のオブジェクトの初期化に必要な情報を保持する構造体[4]
	dsp::ProcessSpec spec;

	dsp::WaveShaper<float> clipper, limiter;
	dsp::Gain<float> drive, masterVolume;

	//IIRFilterをマルチチャンネル対応にしておく
	dsp::ProcessorDuplicator < dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> iirFilter;

	dsp::Reverb reverb;

	// GUI上のキーボードコンポーネントで生成されたMIDI情報を保持しておくオブジェクト.
	// MIDIキーボードの状態を同期するためのステートオブジェクト
	MidiKeyboardState keyboardState;

	// スコープパネルに波形を表示するためのデータバッファ
	AudioBufferQueue<float> scopeDataQueue;
	ScopeDataCollector<float> scopeDataCollector;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSynthAudioProcessor)
};
