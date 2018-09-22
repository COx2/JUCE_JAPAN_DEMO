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

// プラグインをロードした時やホスト側のセットアップ処理を実行した時にホストから呼び出される。
void OneShotSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Synthesiserオブジェクトにホストアプリケーションのサンプリングレートをセットする
	synth.setCurrentPlaybackSampleRate(sampleRate);

	// MidiKeyboardStateオブジェクトの状態を初期化する
	keyboardState.reset();
}

// プラグインを非アクティブ化した時や削除する時にホストから呼び出される。
void OneShotSamplerAudioProcessor::releaseResources()
{

	// MidiKeyboardStateオブジェクトをオール・ノートOFF状態にする。引数に数値0を渡すことですべてのMIDIチャンネルに作用する.
	keyboardState.allNotesOff(0);

	// MidiKeyboardStateオブジェクトの状態を初期化する
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

// ホストアプリケーションからオーディオバッファとMIDIバッファの参照を取得してオーディオレンダリングを実行する関数。
void OneShotSamplerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	if (isChanging) { return; } // isChangingフラグがtrueなら処理をスキップする

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	// MidiKeyboardStateオブジェクトのMIDIメッセージとMIDIバッファのMIDIメッセージをマージする。
	keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

	// オーディオバッファのサンプルデータをクリアしておく。
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// Synthesiserオブジェクトにオーディオバッファの参照とMIDIバッファの参照を渡して、オーディオレンダリングを行う。
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
	isChanging = true;					// 関数processBlock内の処理をスキップするためのフラグ

	// ①Synthesiserオブジェクトに登録されたSamplerVoiceオブジェクトとSamplerSoundオブジェクトをすべて破棄する。
	synth.clearSounds();
	synth.clearVoices();

	// ②読み込んだサンプル音源を割り当てるノート番号の範囲を定義する。setRange関数で0～127（C-2～G8）の範囲をtrueに設定する。
	BigInteger allNotes;
	allNotes.setRange(0, 128, true);

	// ③SamplerSoundオブジェクトをSynthesiserオブジェクトに追加する。
	synth.addSound(new SamplerSound("default", newReader, allNotes, 60, 0, 0.1, 10.0));

	// ④SamplerVoiceオブジェクトをSynthesiserオブジェクトに追加する。
	for (int i = 0; i < 128; i++) {
		synth.addVoice(new SamplerVoice());
	}

	isChanging = false;						// processBlock関数内の処理をスキップするためのフラグ
}

void OneShotSamplerAudioProcessor::loadSineWave()
{
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();

	// バイナリリソースに置かれた正弦波のサンプル音源からMemoyInputSteramクラスのインスタンスを生成する。
	MemoryInputStream* inputStream = new MemoryInputStream(BinaryData::sine_wav, BinaryData::sine_wavSize, true);

	// MemoyInputSteramオブジェクトを引数に渡してAudioFormatReaderクラスのインスタンスを生成する。
	AudioFormatReader* reader = formatManager.createReaderFor(inputStream);

	if (reader != nullptr)
	{
		setupSampler(*reader);
		delete reader;
	}
}

void OneShotSamplerAudioProcessor::loadSampleFile()
{
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();

	// FileChooserクラスのインスタンスを生成する。コンストラクタの実行とともにファイルブラウザが表示される。
	FileChooser chooser("Open audio file to play.", File::nonexistent, formatManager.getWildcardForAllFormats());

	// ファイルブラウザから選択したファイルを開くことができたらif文内の処理を実行する。
	if (chooser.browseForFileToOpen())
	{
		// 開いたFileオブジェクトからAudioFormatReaderクラスのインスタンスを生成する。
		File file(chooser.getResult());
		AudioFormatReader* reader = formatManager.createReaderFor(chooser.getResult());

		if (reader != nullptr)
		{
			// setupSampler関数にAudioFormatReaderオブジェクトを参照渡しする。
			setupSampler(*reader);

			// セットアップ処理が完了したらAudioFormatReaderオブジェクトを破棄する。
			delete reader;
		}
	}
}