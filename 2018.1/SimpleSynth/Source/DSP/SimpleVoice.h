﻿/*
  ==============================================================================

    SimpleVoice.h
    Created: 20 Apr 2018 2:08:09am
    Author:  COx2

  ==============================================================================
*/

#pragma once

// ①各種ヘッダファイルをインクルードする。
#include "../JuceLibraryCode/JuceHeader.h"
#include "SimpleSynthParameters.h"
#include "Waveforms.h"
#include "AmpEnvelope.h"
#include "SimpleSound.h"

// ②クラス名宣言。SynthesiserVoiceクラスを継承する。
class SimpleVoice : public SynthesiserVoice
{
public:
	// ③引数付きコンストラクタ。
	SimpleVoice(OscillatorParameters* oscParams, LfoParameters* lfoParams, AmpEnvelopePatameters* ampEnvParams, AudioParameterBool* velocitySenseParam);

	// デストラクタ
	virtual ~SimpleVoice();

	// ④基底クラスで宣言された純粋仮想関数をオーバーライド宣言する。
	virtual bool canPlaySound(SynthesiserSound* sound) override;
	virtual void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;
	virtual void stopNote(float velocity, bool allowTailOff) override;
	virtual void pitchWheelMoved(int newPitchWheelValue) override;
	virtual void controllerMoved(int controllerNumber, int newControllerValue) override;
	virtual void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:
	// ⑤クラス内でのみ呼び出す関数をprivate領域に宣言する。
	float calcModulationFactor(float angle);

	// ⑥メンバ変数を宣言する。
	float currentAngle, lfoAngle, angleDelta;
	float level, lastLevel, levelDiff;
	float pitchBend;

	// VCOとVCAのオブジェクトを宣言する。
	Waveforms waveForms;
	AmpEnvelope ampEnv;

	// パラメータを管理するオブジェクトのポインタ変数。
	OscillatorParameters* _oscParamsPtr;
	LfoParameters* _lfoParamsPtr;
	AmpEnvelopePatameters* _ampEnvParamsPtr;
	AudioParameterBool* _velocitySenseParamPtr;
};
