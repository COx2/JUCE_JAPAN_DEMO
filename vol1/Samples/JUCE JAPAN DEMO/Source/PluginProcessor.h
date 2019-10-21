/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class JuceJapanDemoAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    JuceJapanDemoAudioProcessor();
    ~JuceJapanDemoAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
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

	//for doc
	//Processor
	enum Parameters { 
		MasterBypass = 0,
		Gain,
		Threshold,
		Volume,
		totalNumParam
	};

	int getNumParameters() override;
	float getParameter(int index) override;
	void setParameter(int index, float value) override;
	const String getParameterName(int index) override;
	const String getParameterText(int index) override;

	//GUI
	bool NeedsUIUpdate() { return UIUpdateFlag; };
	void RequestUIUpdate() { UIUpdateFlag = true; };
	void ClearUIUpdateFlag() { UIUpdateFlag = false; };

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceJapanDemoAudioProcessor)

	//for doc
	//Processor
	float UserParams[totalNumParam];
	//GUI
	bool UIUpdateFlag;
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
