/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OneshotSamplerAudioProcessor::OneshotSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{}

OneshotSamplerAudioProcessor::~OneshotSamplerAudioProcessor()
{
}

//==============================================================================
const String OneshotSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OneshotSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OneshotSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OneshotSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OneshotSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OneshotSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OneshotSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OneshotSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const String OneshotSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void OneshotSamplerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void OneshotSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    keyboardState.reset();
    
}

void OneshotSamplerAudioProcessor::releaseResources()
{
	// If you pass 0 for the midi channel, it will in fact turn off all notes on all channels.
	keyboardState.allNotesOff(0);

    keyboardState.reset();

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OneshotSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OneshotSamplerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    if(isChanging) return;
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synth.renderNextBlock(buffer, midiMessages, 0,  buffer.getNumSamples());
}

//==============================================================================
bool OneshotSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* OneshotSamplerAudioProcessor::createEditor()
{
    return new OneshotSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void OneshotSamplerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OneshotSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OneshotSamplerAudioProcessor();
}


void OneshotSamplerAudioProcessor::setupSampler(AudioFormatReader& newReader)
{
	isChanging = true;

    synth.clearVoices();
    synth.clearSounds();

    // allow our sound to be played on all notes
    BigInteger allNotes;
    allNotes.setRange(0, 128, true);

	// finally, add our sound
	synth.addSound(new SamplerSound("default", newReader, allNotes, 60, 0, 0.1, 10.0));

    // Monophonic
//    synth.addVoice(new SamplerVoice());
    
    // Polyphonic
    for (int i = 0; i < 128; i++) {
        synth.addVoice(new SamplerVoice());
    }
    
	isChanging = false;
}

void OneshotSamplerAudioProcessor::loadSampleFile()
{
	AudioFormatManager formatManager;

	// サポートするファイルフォーマットを登録(JUCE標準対応)
	formatManager.registerBasicFormats();

	FileChooser chooser("Open audio file to play.",
		File::nonexistent, formatManager.getWildcardForAllFormats());

	if (chooser.browseForFileToOpen())
	{
		File file(chooser.getResult());

		AudioFormatReader* reader = formatManager.createReaderFor(file);

		if (reader != nullptr)
		{
			setupSampler(*reader);

			delete reader;
		}
	}
}

void OneshotSamplerAudioProcessor::loadSineWave()
{
	AudioFormatManager formatManager;

	// サポートするファイルフォーマットを登録(JUCE標準対応)
	formatManager.registerBasicFormats();

	MemoryInputStream* inputStream = new MemoryInputStream(BinaryData::sine_wav, BinaryData::sine_wavSize, true);

	AudioFormatReader* reader = formatManager.createReaderFor(inputStream);

	if (reader != nullptr)
	{
		setupSampler(*reader);

		delete reader;
	}
}
