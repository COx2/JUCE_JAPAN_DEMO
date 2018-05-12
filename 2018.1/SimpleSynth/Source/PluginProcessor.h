/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DSP/SimpleSynthParameters.h"

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

	//Parameter�̗p��[1]
	const StringArray LFO_TARGETS{ "None", "WaveLevel", "WaveAngle" };
	const StringArray FILTER_TYPES{ "Low-Pass", "High-Pass", "Band-Pass" };

	OscillatorParameters oscParameters;
	LfoParameters lfoParameters;
	AmpEnvelopePatameters ampEnvParameters;
	FilterPatameters filterParameters;
	ReverbPatameters reverbParameters;
	AudioParameterFloat* driveParameter;
	AudioParameterFloat* masterVolumePrameter;

private:
	MidiKeyboardState keyboardState;
	Synthesiser		  synth;

	//�c�ݗp�̊֐�[2]
	static float clippingFunction(float inputValue);

	//[3]�̃I�u�W�F�N�g�̏������ɕK�v�ȏ���ێ�����\����[4]
	dsp::ProcessSpec spec;

	dsp::WaveShaper<float> clipper, limiter;
	dsp::Gain<float> drive, masterVolume;

	//IIRFilter���}���`�`�����l���Ή��ɂ��Ă���
	dsp::ProcessorDuplicator < dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> iirFilter;

	dsp::Reverb reverb;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSynthAudioProcessor)
};
