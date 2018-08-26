/*
  ==============================================================================

    SimpleSynthParameters.h
    Created: 10 May 2018 12:28:18am
    Author:  COx2

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// ①複数のパラメータをまとめるクラス群の基底クラス。
class SynthParametersBase
{
public:
	// デストラクタ
	virtual ~SynthParametersBase() {};

	// ②継承クラス側で実装を必須とする関数を純粋仮想関数として宣言する。
	virtual void addAllParameters(AudioProcessor& processor) = 0;
	virtual void saveParameters(XmlElement& xml) = 0; 
	virtual void loadParameters(XmlElement& xml) = 0;
};

// ③OSC MIXを操作するパラメータ群を管理するクラス。
class OscillatorParameters : public SynthParametersBase
{
public:
	// ④各波形の音量レベルを管理するパラメータのポインタ変数。
	AudioParameterFloat* SineWaveLevel;
	AudioParameterFloat* SawWaveLevel;
	AudioParameterFloat* TriWaveLevel;
	AudioParameterFloat* SquareWaveLevel;
	AudioParameterFloat* NoiseLevel;

	// ⑤引数付きコンストラクタ。PluginProcessor側で保持するパラメータのポインタ変数を受け取る。
	OscillatorParameters(AudioParameterFloat* sineWaveLevel,
		AudioParameterFloat* sawWaveLevel,
		AudioParameterFloat* triWaveLevel,
		AudioParameterFloat* squareWaveLevel,
		AudioParameterFloat* noiseLevel);

	// ⑥基底クラスで宣言されている純粋仮想関数をオーバーライドして実装する。
	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	// 引数無しコントラクタをprivate領域に置くことで、外から呼び出せないようにする。
	OscillatorParameters() {};
};

class AmpEnvelopePatameters : public SynthParametersBase
{
public:
	AudioParameterFloat* Attack;
	AudioParameterFloat* Decay;
	AudioParameterFloat* Sustain;
	AudioParameterFloat* Release;

	AmpEnvelopePatameters(AudioParameterFloat* attack,
		AudioParameterFloat* decay,
		AudioParameterFloat* sustain,
		AudioParameterFloat* release);

	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	AmpEnvelopePatameters() {};
};

class LfoParameters : public SynthParametersBase
{
public:
	AudioParameterChoice* LfoTarget;
	AudioParameterChoice* LfoWaveType;
	AudioParameterFloat*  LfoAmount;
	AudioParameterFloat*  LfoSpeed;

	LfoParameters(AudioParameterChoice* lfoTarget,
				AudioParameterChoice* lfoWaveType,
				AudioParameterFloat*  LfoAmount,
				AudioParameterFloat*  lfoSpeed);

	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	LfoParameters() {};
};

class FilterPatameters : public SynthParametersBase
{
public:
	AudioParameterChoice* Type;
	AudioParameterFloat*  Frequency;
	AudioParameterFloat*  Q;

	FilterPatameters(AudioParameterChoice* type,
		AudioParameterFloat*  frequency,
		AudioParameterFloat*  q);

	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	FilterPatameters() {};
};

class ReverbPatameters : public SynthParametersBase
{
public:
	AudioParameterFloat*  RoomSize;
	AudioParameterFloat*  Damping;
	AudioParameterFloat*  WetLevel;
	AudioParameterFloat*  DryLevel;
	AudioParameterFloat*  Width;
	AudioParameterFloat*  FreezeMode;

	ReverbPatameters(AudioParameterFloat*  roomSize,
				AudioParameterFloat*  damping,
				AudioParameterFloat*  wetLevel,
				AudioParameterFloat*  dryLevel,
				AudioParameterFloat*  width,
				AudioParameterFloat*  freezeMode);

	virtual void addAllParameters(AudioProcessor& processor) override;
	virtual void saveParameters(XmlElement& xml) override;
	virtual void loadParameters(XmlElement& xml) override;

private:
	ReverbPatameters() {};
};