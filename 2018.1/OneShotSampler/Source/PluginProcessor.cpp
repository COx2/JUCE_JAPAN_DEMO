/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OneShotSamplerAudioProcessor::OneShotSamplerAudioProcessor()
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
{
}

OneShotSamplerAudioProcessor::~OneShotSamplerAudioProcessor()
{
}

//==============================================================================
const String OneShotSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OneShotSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OneShotSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OneShotSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OneShotSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OneShotSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OneShotSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OneShotSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const String OneShotSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void OneShotSamplerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void OneShotSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	// Synthesiser�I�u�W�F�N�g�Ƀz�X�g�A�v���P�[�V�����̃T���v�����O���[�g���Z�b�g����
	synth.setCurrentPlaybackSampleRate(sampleRate);

	// MidiKeyboardState�I�u�W�F�N�g�̏�Ԃ�����������
	keyboardState.reset();
}

void OneShotSamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

	// MidiKeyboardState�I�u�W�F�N�g���I�[���E�m�[�g�E�I�t��Ԃɂ���.�����ɐ��l0��n�����Ƃł��ׂĂ�MIDI�`�����l���ɍ�p����.
	keyboardState.allNotesOff(0);

	// MidiKeyboardState�I�u�W�F�N�g�̏�Ԃ�����������
	keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OneShotSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void OneShotSamplerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	if (isChanging) {
		return;				// �X�L�b�v����t���O��true�Ȃ珈�����X�L�b�v����
	}

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	// MidiKeyboardState�I�u�W�F�N�g�̏�Ԃɕω��������MIDI���b�Z�[�W���擾����
	keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

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
    }


	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// Synthesiser�I�u�W�F�N�g�ɃI�[�f�B�I�o�b�t�@�̎Q�Ƃ�MIDI�o�b�t�@�̎Q�Ƃ�n���āA�I�[�f�B�I�����_�����O���s��
	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================
bool OneShotSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* OneShotSamplerAudioProcessor::createEditor()
{
    return new OneShotSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void OneShotSamplerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OneShotSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OneShotSamplerAudioProcessor();
}


void OneShotSamplerAudioProcessor::setupSampler(AudioFormatReader& newReader)
{
	isChanging = true;					// �֐�processBlock���̏������X�L�b�v���邽�߂̃t���O

										// �@Synthesiser�I�u�W�F�N�g�����ݕێ�����SamplerVoice�I�u�W�F�N�g��SamplerSound�I�u�W�F�N�g�����ׂĉ������
	synth.clearSounds();
	synth.clearVoices();

	// �A�ǂݍ��񂾃T���v�����������蓖�Ă�m�[�g�ԍ��͈̔͂��`����B�֐�"setRange" �ɂ�0�`127�̒l��true�ɐݒ肷��B
	BigInteger allNotes;
	allNotes.setRange(0, 128, true);

	// �BSamplerSound�I�u�W�F�N�g��Synthesiser�I�u�W�F�N�g�ɒǉ�����
	synth.addSound(new SamplerSound("default", newReader, allNotes, 60, 0, 0.1, 10.0));

	// �CSamplerVoice�I�u�W�F�N�g��Synthesiser�I�u�W�F�N�g�ɒǉ�����
	for (int i = 0; i < 128; i++) {
		synth.addVoice(new SamplerVoice());
	}

	isChanging = false;						// �֐�processBlock���̏������X�L�b�v���邽�߂̃t���O
}

void OneShotSamplerAudioProcessor::loadSampleFile()
{
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();

	FileChooser chooser("Open audio file to play.", File::nonexistent, formatManager.getWildcardForAllFormats());

	if (chooser.browseForFileToOpen())
	{
		File file(chooser.getResult());
		AudioFormatReader* reader = formatManager.createReaderFor(file);

		if (reader != nullptr)
		{
			// �֐�"setupSampler"��AudioFormatReader�I�u�W�F�N�g���Q�Ɠn������
			setupSampler(*reader);

			// �Z�b�g�A�b�v����������������AudioFormatReader�I�u�W�F�N�g���������
			delete reader;
		}
	}
}

void OneShotSamplerAudioProcessor::loadSineWave()
{
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();

	// �o�C�i�����\�[�X�ɂ���T�C���g����MemoyInputSteram�N���X�̃C���X�^���X�𐶐�����
	MemoryInputStream* inputStream = new MemoryInputStream(BinaryData::sine_wav, BinaryData::sine_wavSize, true);

	// MemoyInputSteram�I�u�W�F�N�g�������ɓn����AudioFormatReader�N���X�̃C���X�^���X�𐶐�����
	AudioFormatReader* reader = formatManager.createReaderFor(inputStream);

	if (reader != nullptr)
	{
		setupSampler(*reader);
		delete reader;
	}
}