/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JuceJapanDemoAudioProcessor::JuceJapanDemoAudioProcessor()
{
	UserParams[MasterBypass] = 0.0f;//default to not bypassed
	UserParams[Gain] = 0.7f;		//default Width 1.0 (no change)
	UserParams[Threshold] = 1.0f;	//default Width 1.0 (no change)
	UserParams[Volume] = 0.7f;		//default Width 1.0 (no change)
	//GUI
	UIUpdateFlag = true;			//Request UI update
}

JuceJapanDemoAudioProcessor::~JuceJapanDemoAudioProcessor()
{
}

//==============================================================================
const String JuceJapanDemoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceJapanDemoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JuceJapanDemoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double JuceJapanDemoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceJapanDemoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JuceJapanDemoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuceJapanDemoAudioProcessor::setCurrentProgram (int index)
{
}

const String JuceJapanDemoAudioProcessor::getProgramName (int index)
{
    return String();
}

void JuceJapanDemoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void JuceJapanDemoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void JuceJapanDemoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JuceJapanDemoAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
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

void JuceJapanDemoAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
	
	if (UserParams[MasterBypass] == 1.0f) return;

	buffer.applyGain(pow(UserParams[Gain], 2) * 2.0f);
	float threshold = pow(UserParams[Threshold], 2);
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		float* channelData = buffer.getWritePointer (channel);
		for (long buffNum = 0; buffNum<buffer.getNumSamples(); buffNum++){
			if (channelData[buffNum] >= threshold) {
				channelData[buffNum] = threshold;
			}
			else if (channelData[buffNum] <= -threshold) {
				channelData[buffNum] = -threshold;
			}
		}
    }
	buffer.applyGain(pow(UserParams[Volume], 2) * 2.0f);
}

//==============================================================================
bool JuceJapanDemoAudioProcessor::hasEditor() const
{
	//return false;
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* JuceJapanDemoAudioProcessor::createEditor()
{
    return new JuceJapanDemoAudioProcessorEditor (*this);
}

//==============================================================================
void JuceJapanDemoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	//Save UserParams/Data to file
	XmlElement root("Root");
	XmlElement *el;
	el = root.createNewChildElement("Bypass");
	el->addTextElement(String(UserParams[MasterBypass]));
	el = root.createNewChildElement("Gain");
	el->addTextElement(String(UserParams[Gain]));
	el = root.createNewChildElement("Threshold");
	el->addTextElement(String(UserParams[Threshold]));
	el = root.createNewChildElement("Volume");
	el->addTextElement(String(UserParams[Volume]));
	copyXmlToBinary(root, destData);

}

void JuceJapanDemoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	//Load UserParams/Data from file
	XmlElement* pRoot = getXmlFromBinary(data, sizeInBytes);
	if (pRoot != NULL)
	{
		forEachXmlChildElement((*pRoot), pChild)
		{
			if (pChild->hasTagName("Bypass"))
			{
				String text = pChild->getAllSubText();
				setParameter(MasterBypass, text.getFloatValue());
			}
			else if (pChild->hasTagName("Gain"))
			{
				String text = pChild->getAllSubText();
				setParameter(Gain, text.getFloatValue());
			}
			else if (pChild->hasTagName("Threshold"))
			{
				String text = pChild->getAllSubText();
				setParameter(Threshold, text.getFloatValue());
			}
			else if (pChild->hasTagName("Volume"))
			{
				String text = pChild->getAllSubText();
				setParameter(Parameters::Volume, text.getFloatValue());
			}
		}
		delete pRoot;
	}

	//GUI
	UIUpdateFlag = true;//Request UI update
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceJapanDemoAudioProcessor();
}

//Addd for Host
int JuceJapanDemoAudioProcessor::getNumParameters() { return totalNumParam; }

float JuceJapanDemoAudioProcessor::getParameter(int index){
	if (index >= 0 && index < totalNumParam)
		return UserParams[index];
	else return 0;
}

void JuceJapanDemoAudioProcessor::setParameter(int index, float value)
{
	switch (index)
	{
	case MasterBypass:
		UserParams[MasterBypass] = value;
		break;
	case Gain:
		UserParams[Gain] = value;
		break;
	case Threshold:
		UserParams[Threshold] = value;
		break;
	case Volume:
		UserParams[Volume] = value;
		break;
	default: return;
	}
	//GUI
	UIUpdateFlag = true;//Request UI update -- Some OSX hosts use alternate editors, this updates ours
}

const String JuceJapanDemoAudioProcessor::getParameterName(int index)
{
	switch (index)
	{
	case MasterBypass: return "Master Bypass";
	case Gain: return "Gain";
	case Threshold: return "Threshold";
	case Volume: return "Volume";
	default:return String::empty;
	}
}

const String JuceJapanDemoAudioProcessor::getParameterText(int index)
{
	switch (index)
	{
	case MasterBypass: 
		return UserParams[MasterBypass] == 1.0f ? "BYPASS" : "EFFECT";
	case Gain: 
		return String(Decibels::gainToDecibels(pow(UserParams[Gain], 2)*2.0f), 1)+"dB";
	case Threshold: 
		return String(Decibels::gainToDecibels(pow(UserParams[Threshold], 2)), 1)+"dB";
	case Volume: 
		return String(Decibels::gainToDecibels(pow(UserParams[Volume], 2)*2.0f), 1)+"dB";
	default:return String::empty;
	}
}