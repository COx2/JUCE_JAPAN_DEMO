/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

// ①新規に実装したコンポーネントのヘッダファイルをインクルードする。
#include "GUI/ParametersComponent.h"
#include "GUI/ScopeComponent.h"

//==============================================================================
// クラス宣言。プラグインウインドウの基本クラスであるAudioProcessorEditorクラスを継承している。
class SimpleSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SimpleSynthAudioProcessorEditor (SimpleSynthAudioProcessor&);
    ~SimpleSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	// DSPオブジェクトのアドレスを保持しておくための変数。
	SimpleSynthAudioProcessor& processor;

	// ②ウインドウ内に配置するGUIコンポーネントクラスのオブジェクト宣言を追加する。
	MidiKeyboardComponent keyboardComponent;						// キーボードコンポーネント（JUCEライブラリが提供する）
	ScopeComponent<float> scopeComponent;							// SCOPEパネルのコンポーネント
	OscillatorParametersComponent oscParamsComponent;				// OSC MIXパネルのコンポーネント
	AmpEnvelopeParametersComponent ampEnvParamsComponent;			// AMP EGパネルのコンポーネント
	LfoParametersComponent lfoParamsComponent;						// LFOパネルのコンポーネント
	FilterParametersComponent filterParamsComponent;				// FILTERパネルのコンポーネント
	ReverbParametersComponent reverbParamsComponent;				// REVERBパネルのコンポーネント
	DriveParametersComponent driveParamsComponent;					// DRIVEパネルのコンポーネント
	MiscParametersComponent miscParamsComponent;					// 『その他』パネルのコンポーネント

	// ①juce::LookAndFeelクラスのポインタ変数を宣言する
	LookAndFeel* customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSynthAudioProcessorEditor)
};
