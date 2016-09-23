/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleEqualizerAudioProcessor::SimpleEqualizerAudioProcessor()
{
	UserParams[MasterBypass] = 0.0f;
	UserParams[Frequency] = 0.5664f;
	UserParams[BandWidth] = 0.393f;
	UserParams[Gain] = 0.5f;

	UIUpdateFlag = true;//Request UI update
}

SimpleEqualizerAudioProcessor::~SimpleEqualizerAudioProcessor()
{
}

//==============================================================================
const String SimpleEqualizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleEqualizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleEqualizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double SimpleEqualizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleEqualizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleEqualizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleEqualizerAudioProcessor::setCurrentProgram (int index)
{
}

const String SimpleEqualizerAudioProcessor::getProgramName (int index)
{
    return String();
}

void SimpleEqualizerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SimpleEqualizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SimpleEqualizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleEqualizerAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void SimpleEqualizerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

	// Unuse buffer clear process
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	if (UserParams[MasterBypass] >= 1.0f) return;

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		// EQ Process
		UserParams[SampleRate] = getSampleRate();
		float _sampleRate = getSampleRate();
		float _frequency = 20.0f * pow(1000.0, UserParams[Frequency]);
		float _bandWidth = 0.1f * pow(60.0, UserParams[BandWidth]);
		float _gain = 48.0f * (UserParams[Gain] - 0.5f);
		if (channel < 2) {
			peakingEQ[channel].SetParameter(_sampleRate, _frequency, _bandWidth, _gain);
			peakingEQ[channel].DoProcess(buffer.getWritePointer(channel), buffer.getNumSamples());
		}
    }
}

//==============================================================================
bool SimpleEqualizerAudioProcessor::hasEditor() const
{
	return false;
    //return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SimpleEqualizerAudioProcessor::createEditor()
{
    return new SimpleEqualizerAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleEqualizerAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	//Save UserParams/Data to file
	XmlElement root("Root");
	XmlElement *el;
	el = root.createNewChildElement("Bypass");
	el->addTextElement(String(UserParams[Parameters::MasterBypass]));
	el = root.createNewChildElement("Frequency");
	el->addTextElement(String(UserParams[Parameters::Frequency]));
	el = root.createNewChildElement("BandWidth");
	el->addTextElement(String(UserParams[Parameters::BandWidth]));
	el = root.createNewChildElement("Gain");
	el->addTextElement(String(UserParams[Parameters::Gain]));
	copyXmlToBinary(root, destData);
}

void SimpleEqualizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	//Load UserParams/Data from file
	XmlElement* pRoot = getXmlFromBinary(data, sizeInBytes);
	if (pRoot != NULL)
	{
		forEachXmlChildElement((*pRoot), pChild)
		{
			if (pChild->hasTagName("Bypass"))
			{
				String text = pChild->getAllSubText();
				setParameter(Parameters::MasterBypass, text.getFloatValue());
			}
			else if (pChild->hasTagName("Frequency"))
			{
				String text = pChild->getAllSubText();
				setParameter(Parameters::Frequency, text.getFloatValue());
			}
			else if (pChild->hasTagName("BandWidth"))
			{
				String text = pChild->getAllSubText();
				setParameter(Parameters::BandWidth, text.getFloatValue());
			}
			else if (pChild->hasTagName("Gain"))
			{
				String text = pChild->getAllSubText();
				setParameter(Parameters::Gain, text.getFloatValue());
			}
		}
		delete pRoot;
	}

	UIUpdateFlag = true;//Request UI update
}

//==============================================================================
int SimpleEqualizerAudioProcessor::getNumParameters()
{
	return Parameters::totalNumParam;
}

//Get Return Filter Parameter range 0~1.0
float SimpleEqualizerAudioProcessor::getParameter(int index)
{
	if (index >= 0 && index < totalNumParam)
		return UserParams[index];
	else return 0;
}

//The host will call this method to change the value of one of the filter's parameters.
//The host may call this at any time, including during the audio processing callback, so the filter has to process this very fast and avoid blocking.
//If you want to set the value of a parameter internally, e.g. from your editor component, then don't call this directly - instead, use the setParameterNotifyingHost() method, which will also send a message to the host telling it about the change. If the message isn't sent, the host won't be able to automate your parameters properly.
//The value passed will be between 0 and 1.0.
//...
//Your filter can call this when it needs to change one of its parameters.
//This could happen when the editor or some other internal operation changes a parameter. 
//This method will call the setParameter() method to change the value, and will then send a message to the host telling it about the change.
//The value passed will be between 0 and 1.0.
void SimpleEqualizerAudioProcessor::setParameter(int index, float newValue)
{
	if (index >= 0 && index < totalNumParam)
		UserParams[index] = newValue;
	else return;
	
	UIUpdateFlag = true;//Request UI update -- Some OSX hosts use alternate editors, this updates ours
}

//return parameter Name as string to HOST.
const String SimpleEqualizerAudioProcessor::getParameterName(int index)
{
	switch (index)
	{
	case MasterBypass: return "Master Bypass";
	case Frequency: return "Frequency";
	case BandWidth: return "Band Width";
	case Gain: return "Gain";
	case SampleRate: return "SampleRate";
	default:return String::empty;
	}
}

//return parameter value as string to HOST.
const String SimpleEqualizerAudioProcessor::getParameterText(int index)
{
	switch (index)
	{
	case Parameters::MasterBypass:
		return UserParams[MasterBypass]  != 1.0f ? "EFFECT" : "BYPASS";

	case Parameters::Frequency:
		return String((int)(pow(1000.0, UserParams[Frequency]) * 20.0)) + String(" Hz");

	case Parameters::BandWidth:
		return String(pow(60.0, UserParams[BandWidth]) * 0.1f, 2) + String(" Octave");

	case Parameters::Gain:
		return String((48.0f * (UserParams[Gain]-0.5f)), 1) + String(" dB");

	case Parameters::SampleRate:
		return String(UserParams[SampleRate]) + String(" Hz");

	default:return String::empty;
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleEqualizerAudioProcessor();
}
