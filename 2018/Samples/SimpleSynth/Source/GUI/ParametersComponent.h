/*
  ==============================================================================

    ParametersComponent.h
    Created: 16 May 2018 1:55:55am
    Author:  COx2

  ==============================================================================
*/

// 二重インクルード防止のコンパイルオプション
#pragma once

// ①JUCEライブラリのヘッダーとDSP部に定義した機能区ごとのパラメータセットを定義する構造体の宣言をインクルードする。
#include "JuceHeader.h"
#include "../DSP/SimpleSynthParameters.h"

// ②クラス宣言。juce::Componentクラス, juce::Slider::Listenerクラス, juce::Timerクラスを継承する。
class OscillatorParametersComponent : public Component, Slider::Listener, private Timer
{
public:
    // ③引数付きコンストラクタを宣言する。
    OscillatorParametersComponent(OscillatorParameters* oscParams);
    virtual ~OscillatorParametersComponent();

    // ④juce::Componentクラスの描画時に実行される関数をオーバーライド宣言する。
    virtual void paint(Graphics& g) override;

    // ⑤juce::Componentクラスのサイズ変更時に実行される関数をオーバーライド宣言する。
    virtual void resized() override;

private:
    // デフォルトコンストラクタをprivateにすることで呼び出せないようにする。
    OscillatorParametersComponent();

    // ⑥juce::Slider::Listenerクラスの仮想関数（コールバック関数）をオーバーライド宣言する。
    virtual void sliderValueChanged(Slider* slider) override;

    // ⑦juce::Timerクラスの仮想関数（コールバック関数）をオーバーライド宣言する。
    virtual void timerCallback() override;

    // ⑧OSC MIX用のパラメータセットのオブジェクトのポインタを宣言する。
    OscillatorParameters* _oscParamsPtr;

    // ⑨パラメータ変更用スライダーに用いるスライダーコンポーネントを宣言する。
    Slider sineWaveLevelSlider;                             // 正弦波音量レベル用スライダー
    Slider sawWaveLevelSlider;                              // ノコギリ波音量レベル用スライダー
    Slider triWaveLevelSlider;                              // 三角波音量レベル用スライダー
    Slider squareWaveLevelSlider;                           // 矩形波音量レベル用スライダー
    Slider noiseLevelSlider;                                // ノイズ音量レベル用スライダー

    // ⑩パラメータ名表示用テキストラベルに用いるラベルコンポーネントを宣言する。
    Label sineWaveLevelLabel;                               // 正弦波用テキストラベル
    Label sawWaveLevelLabel;                                // ノコギリ波用テキストラベル
    Label triWaveLevelLabel;                                // 三角波用テキストラベル
    Label squareWaveLevelLabel;                             // 矩形波用テキストラベル
    Label noiseLevelLabel;                                  // ノイズ用テキストラベル
};


class AmpEnvelopeParametersComponent : public Component, Slider::Listener, private Timer
{
public:
    AmpEnvelopeParametersComponent(AmpEnvelopeParameters* ampEnvParams);
    virtual ~AmpEnvelopeParametersComponent();

    virtual void paint(Graphics& g) override;
    virtual void resized() override;

private:
    AmpEnvelopeParametersComponent();

    virtual void timerCallback() override;
    virtual void sliderValueChanged(Slider* slider) override;

    AmpEnvelopeParameters* _ampEnvParamsPtr;

    Slider attackSlider;
    Slider decaySlider;
    Slider sustainSlider;
    Slider releaseSlider;

    Label attackLabel;
    Label decayLabel;
    Label sustainLabel;
    Label releaseLabel;
};


class LfoParametersComponent : public Component, Slider::Listener, ComboBox::Listener, private Timer
{
public:
    LfoParametersComponent(LfoParameters* lfoParams);
    virtual ~LfoParametersComponent();
    virtual void paint(Graphics& g) override;
    virtual void resized() override;

private:
    LfoParametersComponent();
    virtual void timerCallback() override;
    virtual void sliderValueChanged(Slider* slider) override;
    virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

    LfoParameters* _lfoParamsPtr;

    ComboBox targetSelector;
    ComboBox waveTypeSelector;

    Slider amountSlider;
    Slider speedSlider;

    Label targetLabel;
    Label waveTypeLabel;
    Label amountLabel;
    Label speedLabel;
};

class FilterParametersComponent : public Component, Slider::Listener, ComboBox::Listener, private Timer
{
public:
    FilterParametersComponent(FilterParameters* filterParams);
    virtual ~FilterParametersComponent();
    virtual void paint(Graphics& g) override;
    virtual void resized() override;

private:
    FilterParametersComponent();
    virtual void timerCallback() override;
    virtual void sliderValueChanged(Slider* slider) override;
    virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

    FilterParameters* _filterParamsPtr;

    ComboBox typeSelector;

    Slider frequencySlider;
    Slider qSlider;

    Label typeLabel;
    Label frequencyLabel;
    Label qLabel;
};

class DriveParametersComponent : public Component, Slider::Listener, private Timer
{
public:
    DriveParametersComponent(AudioParameterFloat* driveParam);
    virtual ~DriveParametersComponent();
    virtual void paint(Graphics& g) override;
    virtual void resized() override;

private:
    DriveParametersComponent();
    virtual void timerCallback() override;
    virtual void sliderValueChanged(Slider* slider) override;

    AudioParameterFloat* _driveParamPtr;

    Slider gainSlider;

    Label gainLabel;
};

class ReverbParametersComponent : public Component, Slider::Listener, private Timer
{
public:
    ReverbParametersComponent(ReverbParameters* reverbParams);
    virtual ~ReverbParametersComponent();
    virtual void paint(Graphics& g) override;
    virtual void resized() override;

private:
    ReverbParametersComponent();
    virtual void timerCallback() override;
    virtual void sliderValueChanged(Slider* slider) override;

    ReverbParameters* _reverbParamsPtr;
    Slider roomSizeSlider;
    Slider dampingSlider;
    Slider wetLevelSlider;
    Slider dryLevelSlider;
    Slider widthSlider;
    Slider freezeModeSlider;
    Label roomSizeLabel;
    Label dampingLabel;
    Label wetLevelLabel;
    Label dryLevelLabel;
    Label widthLabel;
    Label freezeModeLabel;
};

class MiscParametersComponent : public Component, Slider::Listener, Button::Listener, private Timer
{
public:
    MiscParametersComponent(AudioParameterFloat* masterVolumeParam,
                            AudioParameterInt* voiceSizeParam,
                            AudioParameterBool* velocitySenseParam);
    virtual ~MiscParametersComponent();
    virtual void paint(Graphics& g) override;
    virtual void resized() override;

private:
    MiscParametersComponent();
    virtual void timerCallback() override;
    virtual void sliderValueChanged(Slider* slider) override;
    virtual void buttonClicked(Button* button) override;

    AudioParameterFloat* _masterVolumeParamPtr;
    AudioParameterInt* _voiceSizeParamPtr;
    AudioParameterBool* _velocitySenseParamPtr;

    Slider masterVolumeSlider;
    Slider voiceSizeSlider;
    ToggleButton velocitySenseButton;
    Label masterVolumeLabel;
    Label voiceSizeLabel;
};
