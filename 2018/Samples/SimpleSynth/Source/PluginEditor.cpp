/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

// ①当ソースファイル内で繰り返し使用する定数宣言を無名名前空間にまとめておく
namespace {
	const float KEY_HEIGHT = 80.0f;		// キーボードコンポーネントの高さ方向の大きさ
	const float KEY_WIDTH = 32.0f;			// キー1個分の幅方向の大きさ
	const int PANEL_MARGIN = 2;			// パネル同士の間隔を空けるための大きさ
}

//==============================================================================
// ②引数付きコンストラクタ。プロジェクト新規作成時に既に定義されており、コンポーネントオブジェクトの初期化子を追加する。
//   OSC MIXパネルなどの各コンポーネントに対してSimpleSynthAudioProcessorオブジェクトが保持するパラメータセットを渡す
//   引数として受け取った参照変数"p"を介してSimpleSynthAudioProcessorクラスのpublic領域に定義されたパラメータセットの
//   アドレスやパラメータのポインタ変数を取得し、各パネルの引数付きコンストラクタの引数として渡している。
SimpleSynthAudioProcessorEditor::SimpleSynthAudioProcessorEditor(SimpleSynthAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
	, keyboardComponent(p.getKeyboardState(), MidiKeyboardComponent::Orientation::horizontalKeyboard)
	, oscParamsComponent(&p.oscParameters)
	, ampEnvParamsComponent(&p.ampEnvParameters)
	, lfoParamsComponent(&p.lfoParameters)
	, filterParamsComponent(&p.filterParameters)
	, reverbParamsComponent(&p.reverbParameters)
	, driveParamsComponent(p.driveParameter)
	, miscParamsComponent(p.masterVolumePrameter, p.voiceSizeParameter, p.velocitySenseParameter)
	, scopeComponent(p.getAudioBufferQueue())
{
	// ③キーボードコンポーネントのキー1個分の幅方向の大きさを変更する。
	keyboardComponent.setKeyWidth(KEY_WIDTH);

	// ④各コンポーネントを表示する
	addAndMakeVisible(keyboardComponent);
	addAndMakeVisible(oscParamsComponent);
	addAndMakeVisible(ampEnvParamsComponent);
	addAndMakeVisible(lfoParamsComponent);
	addAndMakeVisible(filterParamsComponent);
	addAndMakeVisible(driveParamsComponent);
	addAndMakeVisible(reverbParamsComponent);
	addAndMakeVisible(miscParamsComponent);
	addAndMakeVisible(scopeComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	// ⑤ウインドウサイズを設定する。キーボードコンポーネントの高さ分を確保しつつ、パネルを並べる領域を16:9のアスペクト比とする。
	//   この関数が実行されたことをトリガーとしてresized関数が実行される。
	setSize (960, 540 + KEY_HEIGHT);

	// assign custom look and feel.
	// ②juce::LookAndFeel_V4クラスのインスタンスを生成する。初期値としてLookAndFeel_V4のグレイカラースキームを生成する。
	customLookAndFeel = new LookAndFeel_V4(LookAndFeel_V4::getGreyColourScheme());

	// ③LookAndFeelオブジェクトに対して各種設定の変更を行う。以下ではコンポーネントのパーツに対して色の変更を行っている。
	customLookAndFeel->setColour(TooltipWindow::ColourIds::textColourId, Colours::white);
	customLookAndFeel->setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::mediumvioletred);
	customLookAndFeel->setColour(Slider::ColourIds::trackColourId, Colours::yellow);

	// ④ウインドウのすべての子コンポーネントに対してカスタムLookAndFeelオブジェクトを渡す。
	for (Component* child : getChildren()) {
		child->setLookAndFeel(customLookAndFeel);
	}
}

// デストラクタ
SimpleSynthAudioProcessorEditor::~SimpleSynthAudioProcessorEditor()
{
	// ⑤ウインドウのすべての子コンポーネントに対してLookAndFeelにnullptrを渡すことで、LookAndFeelの参照をクリアする。
	for (Component* child : getChildren()) {
		child->setLookAndFeel(nullptr);
	}

	// ⑥カスタムLookAndFeelのインスタンスを破棄する
	delete customLookAndFeel;
}

// ①juce::Componentクラスの仮想関数であるpaint関数をオーバーライドする。新規作成の時点で定義されている。
void SimpleSynthAudioProcessorEditor::paint(Graphics& g)
{
	// ②背景を黒色で塗りつぶす。グラフィックコンテキストのメンバ関数"fillAll"を実行する。
	g.fillAll(Colours::black);
}

// ③juce::Componentクラスの仮想関数である"resized"関数をオーバーライドする。
void SimpleSynthAudioProcessorEditor::resized()
{
	// ④ウインドウの矩形領域を取得する
	Rectangle<int> bounds = getLocalBounds();

	// ⑤キーボードコンポーネントを配置する
	keyboardComponent.setBounds(bounds.removeFromBottom(KEY_HEIGHT));

	// ⑥"bounds"変数に保持されている矩形領域からOSC MIXパネルなどを配置する最上段のエリアを取得する。
	Rectangle<int> upperArea = bounds.removeFromTop(bounds.getHeight() * 0.5);
	{
		// "upperArea"変数に保持されている矩形領域の左から右に向かって順番にパネルを配置する処理を繰り返す

		// OSC MIXパネルを配置する。"upperArea"の左から280ピクセルの幅の矩形領域を切り取ってマージン分縮小して引数に渡す。
		oscParamsComponent.setBounds(upperArea.removeFromLeft(280).reduced(PANEL_MARGIN));

		// LFOパネルを配置する。"upperArea"の左から240ピクセルの幅の矩形領域を切り取ってマージン分縮小して引数に渡す。
		lfoParamsComponent.setBounds(upperArea.removeFromLeft(240).reduced(PANEL_MARGIN));

		// AMP EGパネルを配置する。"upperArea"の左から240ピクセルの幅の矩形領域を切り取ってマージン分縮小して引数に渡す。
		ampEnvParamsComponent.setBounds(upperArea.removeFromLeft(240).reduced(PANEL_MARGIN));

		// FILTERパネルを配置する。"upperArea"が保持する矩形領域をマージン分縮小して引数に渡す。
		filterParamsComponent.setBounds(upperArea.reduced(PANEL_MARGIN));
	}

	// ⑦"bounds"変数に保持されている矩形領域からSCOPEパネルなどを配置する最上段のエリアを取得する。
	Rectangle<int> lowerArea = bounds;
	{
		// "lowerArea"変数に保持されている矩形領域の左から右に向かって順番にパネルを配置する処理を繰り返す

		// SCOPEパネルを配置する。"lowerArea"の左から420ピクセルの幅の矩形領域を切り取ってマージン分縮小して引数に渡す。
		scopeComponent.setBounds(lowerArea.removeFromLeft(420).reduced(PANEL_MARGIN));

		// DRIVEパネルを配置する。"lowerArea"の左から100ピクセルの幅の矩形領域を切り取ってマージン分縮小して引数に渡す。
		driveParamsComponent.setBounds(lowerArea.removeFromLeft(100).reduced(PANEL_MARGIN));

		// REVERBパネルを配置する。"lowerArea"の左から280ピクセルの幅の矩形領域を切り取ってマージン分縮小して引数に渡す。
		reverbParamsComponent.setBounds(lowerArea.removeFromLeft(280).reduced(PANEL_MARGIN));

		// "その他"のパネルを配置する。"lowerArea"が保持する矩形領域をマージン分縮小して引数に渡す。
		miscParamsComponent.setBounds(lowerArea.reduced(PANEL_MARGIN));
	}
}
