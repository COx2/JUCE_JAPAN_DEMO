/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

// ヘッダファイルをインクルードする。
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "DSP/SimpleSound.h"
#include "DSP/SimpleVoice.h"

// コンストラクタ。初期化指定子で各種パラメータ群の初期化を行う。
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
	// OSC MIXモジュールを操作するためのパラメータ群
	, oscParameters{
		new AudioParameterFloat("SINEWAVE_LEVEL",	"SineWave-Level", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("SAWWAVE_LEVEL",	"SawWave-Level", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("TRIWAVE_LEVEL",	"TriWave-Level", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("SQUAREWAVE_LEVEL", "SquareWave-Level", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("NOISE_LEVEL", "Noise-Level",  0.0f, 1.0f, 0.0f)
	}
	// LFOモジュールを操作するためのパラメータ群
	, lfoParameters{
		new AudioParameterChoice("LFO_TARGET", "Lfo-Target", LFO_TARGETS, 0),
		new AudioParameterChoice("LFO_WAVE_TYPE", "Lfo-WaveType", LFO_WAVE_TYPES, 0),
		new AudioParameterFloat("LFO_LEVEL", "Lfo-Level",  0.0f, 1.0f, 0.5f),
		new AudioParameterFloat("LFO_SPEED", "Lfo-Speed",  0.0f, 20.0f, 0.2f)
	}
	// AMP EGモジュールを操作するためのパラメータ群
	, ampEnvParameters{
		new AudioParameterFloat("AMPENV_ATTACK", "Attack", 0.01f, 3.0f, 0.1f),
		new AudioParameterFloat("AMPENV_DECAY", "Decay",  0.01f, 3.0f, 0.1f),
		new AudioParameterFloat("AMPENV_SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("AMPENV_RELEASE", "Release", 0.01f, 3.0f, 0.1f)
	}
	// Filterモジュールを操作するためのパラメータ群
	, filterParameters{
		new AudioParameterChoice("FILTER_TYPE", "FilterType", FILTER_TYPES, 0),
		new AudioParameterFloat("FILTER_FREQUENCY", "Frequency", 20.f, 20000.0f, 20000.0f),
		new AudioParameterFloat("FILTER_Q", "Q", 0.3f, 20.0f, 1.0f),
	}
	// Reverbモジュールを操作するためのパラメータ群
	, reverbParameters{
		new AudioParameterFloat("REVERB_ROOM_SIZE",	"Room-Size",	0.0f, 1.0f, 0.0f),
		new AudioParameterFloat("REVERB_DAMPING",	"Damping",		0.0f, 1.0f, 0.0f),
		new AudioParameterFloat("REVERB_WET_LEVEL",	"Wet-Level",	0.0f, 1.0f, 0.0f),
		new AudioParameterFloat("REVERB_DRY_LEVEL",	"Dry-Level",	0.0f, 1.0f, 1.0f),
		new AudioParameterFloat("REVERB_WIDTH",		"Width",		0.0f, 1.0f, 0.0f),
		new AudioParameterFloat("REVERB_FREEZE_MODE",  "Freeze-Mode",	0.0f, 1.0f, 0.0f)
	}
	// その他のモジュールを操作するためのパラメータ群
	, driveParameter(new AudioParameterFloat("DRIVE", "Drive", -24.f, 12.f, 0.0f))
	, masterVolumePrameter(new AudioParameterFloat("MASTER_VOLUME", "Volume", -36.f, 6.f, -3.0f))
	, voiceSizeParameter(new AudioParameterInt("VOICE_SIZE", "Voice-Size", 1, 128, 8))
	, velocitySenseParameter(new AudioParameterBool("VELOCITY_SENSE", "Velocity-Sense", true))
	// GUI: SCOPEコンポネントで用いるScopeDataCollectorクラスのオブジェクトを初期化する処理
	, scopeDataCollector(scopeDataQueue)
{
	// 当クラスのオブジェクトにパラメータ変数（AudioParameterXxxクラスのインスタンス）を登録する。
	// 登録されたパラメータ変数は、基底クラスに宣言されたインターフェースを介して更新処理が行われる。
	// また、パラメータ変数のインスタンスの破棄は、基底クラスの実装によって適切なタイミングで破棄される。
	// SynthParametersBaseで宣言されたインターフェースを介して共通の関数名を呼び出している。
	oscParameters.addAllParameters(*this);
	lfoParameters.addAllParameters(*this);
	ampEnvParameters.addAllParameters(*this);
	addParameter(driveParameter);
	filterParameters.addAllParameters(*this);
	reverbParameters.addAllParameters(*this);
	addParameter(masterVolumePrameter);
	addParameter(voiceSizeParameter);
	addParameter(velocitySenseParameter);
}

SimpleSynthAudioProcessor::~SimpleSynthAudioProcessor()
{}

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
	//================================ ボイスセクション ====================================
	synth.clearSounds();
	synth.clearVoices();

	synth.setCurrentPlaybackSampleRate(sampleRate);

	// サウンド再生可能なノート番号の範囲を定義する。関数"setRange" にて0～127の値をtrueに設定する。
	BigInteger canPlayNotes;
	canPlayNotes.setRange(0, 127, true);
	// サウンド再生可能なチャンネル番号の範囲を定義する。関数"setRange" にて0～127の値をtrueに設定する。
	BigInteger canPlayChannels;
	canPlayChannels.setRange(1, 16, true);
	synth.addSound(new SimpleSound(canPlayNotes, canPlayChannels));

	int numVoices = voiceSizeParameter->get();
	for (int i = 0; i < numVoices; ++i)
	{
		synth.addVoice(new SimpleVoice(&oscParameters, &lfoParameters, &ampEnvParameters, velocitySenseParameter));
	}

	//================================ エフェクトセクション ====================================
	spec.sampleRate = sampleRate;
	spec.numChannels = getTotalNumOutputChannels();
	spec.maximumBlockSize = samplesPerBlock;

	iirFilter.prepare(spec);

	drive.prepare(spec);

	clipper.prepare(spec);
	clipper.functionToUse = clippingFunction;

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
	// ⑦MidiKeyboardStateクラスのオブジェクトにMIDIデバイスから入力されたMIDIバッファを渡すとともに、
	//   GUIのキーボードコンポーネントで生成されたMIDIデータをのMIDIバッファに追加する処理を行う。
	keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

	if ((int)voiceSizeParameter->get() != synth.getNumVoices()) {
		changeVoiceSize();
	}

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

		// シンセサイザーでバッファに対して加算処理を行う前にゼロクリアをしておく。
		buffer.clear(channel, 0, buffer.getNumSamples());
    }

	//================================ ボイスセクション ====================================

	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());


	//================================ エフェクトセクション ====================================

	// JUCE DSPモジュールで処理できるようにオーディオバッファをラップする
	dsp::AudioBlock<float> audioBlock(buffer);
	dsp::ProcessContextReplacing<float> context(audioBlock);

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

	// ゲインを上げる
	drive.setGainDecibels(driveParameter->get());
	drive.process(context);

	// クリッピング処理
	clipper.process(context);

	// リバーブ処理
	juce::dsp::Reverb::Parameters reverbParam;
	reverbParam.roomSize = reverbParameters.RoomSize->get();
	reverbParam.damping = reverbParameters.Damping->get();
	reverbParam.wetLevel = reverbParameters.WetLevel->get();
	reverbParam.dryLevel = reverbParameters.DryLevel->get();
	reverbParam.width = reverbParameters.Width->get();
	reverbParam.freezeMode = reverbParameters.FreezeMode->get();
	reverb.setParameters(reverbParam);
	reverb.process(context);

	// リミッター（クリッピング処理）
	limiter.process(context);

	// ⑧現時点でオーディオバッファで保持しているサンプルデータをScopeDataCollectorクラスのオブジェクトに渡す。
	scopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples());

	// マスターボリューム調整
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
// ⑦パラメータ設定値を外部ファイルにセーブする関数。
void SimpleSynthAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// XMLフォーマットのオブジェクトを宣言する。
	std::unique_ptr<XmlElement> xml(new XmlElement("SimpleSynthParameters"));

	// 以下、該当するパラメータ変数から値を取得してXMLオブジェクトに値を書き込む処理を行う
	// SimpleSynthParameters.h/cppに定義されたパラメータの値をセーブする関数を実行する。
	oscParameters.saveParameters(*xml);
	lfoParameters.saveParameters(*xml);
	ampEnvParameters.saveParameters(*xml);
	filterParameters.saveParameters(*xml);
	reverbParameters.saveParameters(*xml);

	// XMLオブジェクトにタグと値のペアとなる文字列のデータを追加する。
	xml->setAttribute(driveParameter->paramID, (double)driveParameter->get());
	xml->setAttribute(masterVolumePrameter->paramID, (double)masterVolumePrameter->get());
	xml->setAttribute(voiceSizeParameter->paramID, voiceSizeParameter->get());
	xml->setAttribute(velocitySenseParameter->paramID, velocitySenseParameter->get());

	// XMLフォーマットのオブジェクトをバイナリデータ（外部ファイル）にコピーする。
	copyXmlToBinary(*xml, destData);
}

// ⑧パラメータ設定値を外部ファイルからロードする関数。
void SimpleSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{

	// 外部ファイルのバイナリデータからXMLフォーマットのオブジェクトのポインタを取得する。
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	// XMLオブジェクトが空でなければ、パラメータ設定値のロード処理を行う。
	if (xmlState.get() != nullptr)
	{
		// XMLオブジェクトからSimpleSynthのパラメータ設定値を保持する先頭のタグを取得する。
		if (xmlState->hasTagName("SimpleSynthParameters"))
		{
			// 以下、該当するXMLオブジェクトから値を取得してパラメータ変数に値を代入する処理を行う
			// SimpleSynthParameters.h/cppに定義されたパラメータの値をロードする関数を実行する。
			oscParameters.loadParameters(*xmlState);
			lfoParameters.loadParameters(*xmlState);
			ampEnvParameters.loadParameters(*xmlState);
			filterParameters.loadParameters(*xmlState);
			reverbParameters.loadParameters(*xmlState);

			// driveParameterのパラメータID（文字列）と一致するタグからパラメータの値を浮動小数点型として取得する。
			*driveParameter = (float)xmlState->getDoubleAttribute(driveParameter->paramID, 0.0);
			*masterVolumePrameter = (float)xmlState->getDoubleAttribute(masterVolumePrameter->paramID, -3.0);

			// voiceSizeParameterのパラメータID（文字列）と一致するタグからパラメータの値を整数型として取得する。
			*voiceSizeParameter = xmlState->getIntAttribute(voiceSizeParameter->paramID, 1);
			*velocitySenseParameter = xmlState->getBoolAttribute(velocitySenseParameter->paramID, true);
		}
	}
}

void SimpleSynthAudioProcessor::changeVoiceSize()
{
	while (synth.getNumVoices() != voiceSizeParameter->get())
	{
		if (synth.getNumVoices() > voiceSizeParameter->get())
		{
			synth.removeVoice(synth.getNumVoices() - 1);
		}
		else
		{
			synth.addVoice(new SimpleVoice(&oscParameters, &lfoParameters, &ampEnvParameters, velocitySenseParameter));
		}
	}
}

float SimpleSynthAudioProcessor::clippingFunction(float inputValue)
{
	// 双曲線正接...1の時に0.8の値を, -1の時に-0.8の値を取る
	float threshold = tanhf(inputValue);
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
