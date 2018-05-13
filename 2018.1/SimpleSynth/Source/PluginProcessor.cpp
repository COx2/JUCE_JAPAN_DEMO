/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "DSP/SimpleSound.h"
#include "DSP/SimpleVoice.h"

//==============================================================================
SimpleSynthAudioProcessor::SimpleSynthAudioProcessor()
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
	, oscParameters{
		new AudioParameterFloat("SINEWAVE_LEVEL",	"SineWave-Level", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("SAWWAVE_LEVEL",	"SawWave-Level", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("TRIWAVE_LEVEL",	"TriWave-Level", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("SQUAREWAVE_LEVEL", "SquareWave-Level", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("NOISE_LEVEL", "Noise-Level",  0.0f, 1.0f, 0.0f)
	}
	, lfoParameters{
		new AudioParameterChoice("LFO_TARGET", "Lfo-Target", LFO_TARGETS, 0),
		new AudioParameterFloat("LFO_LEVEL", "Lfo-Level",  0.0f, 1.0f, 0.5f),
		new AudioParameterFloat("LFO_SPEED", "Lfo-Speed",  0.0f, 20.0f, 0.2f)
	}
	, ampEnvParameters{
		new AudioParameterFloat("AMPENV_ATTACK", "Attack", 0.01f, 3.0f, 0.1f),
		new AudioParameterFloat("AMPENV_DECAY", "Decay",  0.01f, 3.0f, 0.1f),
		new AudioParameterFloat("AMPENV_SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("AMPENV_RELEASE", "Release", 0.01f, 3.0f, 0.1f)
	}
	, filterParameters{
		new AudioParameterChoice("FILTER_TYPE", "FilterType", FILTER_TYPES, 0),
		new AudioParameterFloat("FILTER_FREQUENCY", "Frequency", 20.f, 20000.0f, 20000.0f),
		new AudioParameterFloat("FILTER_Q", "Q", 0.3f, 20.0f, 1.0f),
	}
	, reverbParameters{
		new AudioParameterFloat("REVERB_ROOM_SIZE",	"Room-Size",	0.0f, 1.0f, 0.0f),
		new AudioParameterFloat("REVERB_DAMPING",	"Damping",		0.0f, 1.0f, 0.0f),
		new AudioParameterFloat("REVERB_WET_LEVEL",	"Wet-Level",	0.0f, 1.0f, 0.0f),
		new AudioParameterFloat("REVERB_DRY_LEVEL",	"Dry-Level",	0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("REVERB_WIDTH",		"Width",		0.0f, 1.0f, 0.0f),
		new AudioParameterFloat("REVERB_FREEZE_MODE",  "Freeze-Mode",	0.0f, 1.0f, 0.0f)
	}
	, driveParameter(new AudioParameterFloat("DRIVE", "Drive", -24.f, 12.f, 0.0f))
	, masterVolumePrameter(new AudioParameterFloat("MASTER_VOLUME", "Volume", -36.f, 6.f, -3.0f))
{
	oscParameters.addAllParameters(*this);
	lfoParameters.addAllParameters(*this);
	ampEnvParameters.addAllParameters(*this);
	addParameter(driveParameter);
	filterParameters.addAllParameters(*this);
	reverbParameters.addAllParameters(*this);
	addParameter(masterVolumePrameter);
}

SimpleSynthAudioProcessor::~SimpleSynthAudioProcessor()
{
}

//==============================================================================
const String SimpleSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String SimpleSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SimpleSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	synth.clearSounds();
	synth.clearVoices();

	synth.setCurrentPlaybackSampleRate(sampleRate);

	// サイン波を割り当てるノート番号の範囲を定義する。関数"setRange" にて0～127の値をtrueに設定する。
	BigInteger sineNotes;
	sineNotes.setRange(0, 127, true);
	synth.addSound(new SimpleSound(sineNotes));

	int numVoices = 8;
	for (int i = 0; i < numVoices; ++i)
	{
		synth.addVoice(new SimpleVoice(&oscParameters, &lfoParameters, &ampEnvParameters, true));
	}

	spec.sampleRate = sampleRate;
	spec.numChannels = getTotalNumOutputChannels();
	spec.maximumBlockSize = samplesPerBlock;

	drive.prepare(spec);

	clipper.prepare(spec);
	clipper.functionToUse = clippingFunction;

	iirFilter.prepare(spec);

	reverb.prepare(spec);

	limiter.prepare(spec);
	limiter.functionToUse = clippingFunction;

	masterVolume.prepare(spec);
}

void SimpleSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SimpleSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

	keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...

		// シンセサイザーでバッファに対して加算処理を行う前にゼロクリアをしておく。
		buffer.clear(channel, 0, buffer.getNumSamples());

    }

	//================================ ボイスセクション ====================================

	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());


	//================================ エフェクトセクション ====================================

	// juce::dspモジュールに渡せるように入力信号バッファをラップする
	dsp::AudioBlock<float> audioBlock(buffer);
	dsp::ProcessContextReplacing<float> context(audioBlock);

	drive.setGainDecibels(driveParameter->get());
	drive.process(context);

	clipper.process(context);

	if (filterParameters.Type->getCurrentChoiceName() == "Low-Pass") 
	{
		*iirFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate
			, filterParameters.Frequency->get()
			, filterParameters.Q->get()
		);
	}
	else if (filterParameters.Type->getCurrentChoiceName() == "High-Pass") 
	{
		*iirFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate
			, filterParameters.Frequency->get()
			, filterParameters.Q->get()
		);
	}
	else if (filterParameters.Type->getCurrentChoiceName() == "Band-Pass")
	{
		*iirFilter.state = *dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate
			, filterParameters.Frequency->get()
			, filterParameters.Q->get()
		);
	}
	iirFilter.process(context);

	juce::dsp::Reverb::Parameters reverbParam;
	reverbParam.roomSize = reverbParameters.RoomSize->get();
	reverbParam.damping = reverbParameters.Damping->get();
	reverbParam.wetLevel = reverbParameters.WetLevel->get();
	reverbParam.dryLevel = reverbParameters.DryLevel->get();
	reverbParam.width = reverbParameters.Width->get();
	reverbParam.freezeMode = reverbParameters.FreezeMode->get();
	reverb.setParameters(reverbParam);
	reverb.process(context);

	limiter.process(context);

	masterVolume.setGainDecibels(masterVolumePrameter->get());
	masterVolume.process(context);
}

//==============================================================================
bool SimpleSynthAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SimpleSynthAudioProcessor::createEditor()
{
    return new SimpleSynthAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	
	std::unique_ptr<XmlElement> xml(new XmlElement("SimpleSynthParameters"));

	oscParameters.saveParameters(*xml);
	lfoParameters.saveParameters(*xml);
	ampEnvParameters.saveParameters(*xml);
	filterParameters.saveParameters(*xml);
	reverbParameters.saveParameters(*xml);
	xml->setAttribute(driveParameter->paramID, (double)driveParameter->get());
	xml->setAttribute(masterVolumePrameter->paramID, (double)masterVolumePrameter->get());
	
	copyXmlToBinary(*xml, destData);
}

void SimpleSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName("SimpleSynthParameters")) 
		{
			oscParameters.loadParameters(*xmlState);
			lfoParameters.loadParameters(*xmlState);
			ampEnvParameters.loadParameters(*xmlState);
			filterParameters.loadParameters(*xmlState);
			reverbParameters.loadParameters(*xmlState);
			*driveParameter = (float)xmlState->getDoubleAttribute(driveParameter->paramID, 0.0);
			*masterVolumePrameter = (float)xmlState->getDoubleAttribute(masterVolumePrameter->paramID, -3.0);
		}
	}
}

float SimpleSynthAudioProcessor::clippingFunction(float inputValue)
{
	float threshold = std::tanhf(inputValue); // 双曲線正接...1の時に0.8の値を, -1の時に-0.8の値を取る
	float outputValue = inputValue;

	//inputValueが正の値ならthresholdも正の値，inputValueが負の値ならthresholdも負の値を取るので，絶対値で比較する
	if (abs(inputValue) >= abs(threshold)) outputValue = threshold;

	return outputValue;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleSynthAudioProcessor();
}
