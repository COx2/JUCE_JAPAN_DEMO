/*
  ==============================================================================

    SimpleSynthParameters.cpp
    Created: 10 May 2018 12:28:18am
    Author:  COx2

  ==============================================================================
*/

// ヘッダファイルをインクルードする。
#include "SimpleSynthParameters.h"

// ①引数付きコンストラクタ。初期化指定子でメンバ変数に初期値を代入する。
OscillatorParameters::OscillatorParameters(AudioParameterFloat * sineWaveLevel, AudioParameterFloat * sawWaveLevel,
											AudioParameterFloat * triWaveLevel, AudioParameterFloat * squareWaveLevel,
											AudioParameterFloat * noiseLevel)
	:SineWaveLevel(sineWaveLevel), SawWaveLevel(sawWaveLevel),
	 TriWaveLevel(triWaveLevel), SquareWaveLevel(squareWaveLevel),
	 NoiseLevel(noiseLevel)
{}

// ②当クラスが保持するパラメータ群をAudioProcessorクラスのパラメータとして登録する。
void OscillatorParameters::addAllParameters(AudioProcessor& processor)
{
	// 引数で受け取ったAudioProcessorクラスの参照を介してaddParameter関数を実行してパラメータを追加する。
	processor.addParameter(SineWaveLevel);
	processor.addParameter(SawWaveLevel);
	processor.addParameter(TriWaveLevel);
	processor.addParameter(SquareWaveLevel);
	processor.addParameter(NoiseLevel);
}

// ③各パラメータの値をXMLフォーマットのデータにセーブする関数。
void OscillatorParameters::saveParameters(XmlElement& xml)
{
	// 引数で受け取ったXmlElementクラスの参照を介して各パラメータのIDと値のペアをXMLフォーマットで書き込む。
	xml.setAttribute(SineWaveLevel->paramID, (double)SineWaveLevel->get());
	xml.setAttribute(SawWaveLevel->paramID, (double)SawWaveLevel->get());
	xml.setAttribute(TriWaveLevel->paramID, (double)TriWaveLevel->get());
	xml.setAttribute(SquareWaveLevel->paramID, (double)SquareWaveLevel->get());
	xml.setAttribute(NoiseLevel->paramID, (double)NoiseLevel->get());
}

// ④各パラメータの値をXMLフォーマットのデータからロードする関数。
void OscillatorParameters::loadParameters(XmlElement& xml)
{
	// 引数で受け取ったXmlElementクラスの参照を介してXMLフォーマットのデータから各パラメータに対応する値を読み込んで代入する。
	*SineWaveLevel = (float)xml.getDoubleAttribute(SineWaveLevel->paramID, 1.0);
	*SawWaveLevel = (float)xml.getDoubleAttribute(SawWaveLevel->paramID, 1.0);
	*TriWaveLevel = (float)xml.getDoubleAttribute(TriWaveLevel->paramID, 1.0);
	*SquareWaveLevel = (float)xml.getDoubleAttribute(SquareWaveLevel->paramID, 1.0);
	*NoiseLevel = (float)xml.getDoubleAttribute(NoiseLevel->paramID, 0.0);
}

AmpEnvelopeParameters::AmpEnvelopeParameters(AudioParameterFloat* attack,
											 AudioParameterFloat* decay,
											 AudioParameterFloat* sustain,
											 AudioParameterFloat* release)
	: Attack(attack)
	, Decay(decay)
	, Sustain(sustain)
	, Release(release)
{}

void AmpEnvelopeParameters::addAllParameters(AudioProcessor& processor)
{
	processor.addParameter(Attack);
	processor.addParameter(Decay);
	processor.addParameter(Sustain);
	processor.addParameter(Release);
}

void AmpEnvelopeParameters::saveParameters(XmlElement & xml)
{
	xml.setAttribute(Attack->paramID, (double)Attack->get());
	xml.setAttribute(Decay->paramID, (double)Decay->get());
	xml.setAttribute(Sustain->paramID, (double)Sustain->get());
	xml.setAttribute(Release->paramID, (double)Release->get());
}

void AmpEnvelopeParameters::loadParameters(XmlElement & xml)
{
	*Attack = (float)xml.getDoubleAttribute(Attack->paramID, 0.01);
	*Decay = (float)xml.getDoubleAttribute(Decay->paramID, 0.01);
	*Sustain = (float)xml.getDoubleAttribute(Sustain->paramID, 1.0);
	*Release = (float)xml.getDoubleAttribute(Release->paramID, 0.01);
}

LfoParameters::LfoParameters(AudioParameterChoice* lfoTarget, 
							 AudioParameterChoice* lfoWaveType,
							 AudioParameterFloat* lfoAmount,
							 AudioParameterFloat* lfoSpeed)
	: LfoTarget(lfoTarget)
	, LfoWaveType(lfoWaveType)
	, LfoAmount(lfoAmount)
	, LfoSpeed(lfoSpeed)
{
    LfoSpeed->range.symmetricSkew = false;
    LfoSpeed->range.skew = 0.5;
    
}

void LfoParameters::addAllParameters(AudioProcessor& processor)
{
	processor.addParameter(LfoTarget);
	processor.addParameter(LfoWaveType);
	processor.addParameter(LfoAmount);
	processor.addParameter(LfoSpeed);
}

void LfoParameters::saveParameters(XmlElement & xml)
{
	xml.setAttribute(LfoTarget->paramID, LfoTarget->getIndex());
	xml.setAttribute(LfoWaveType->paramID, LfoWaveType->getIndex());
	xml.setAttribute(LfoAmount->paramID, (double)LfoAmount->get());
	xml.setAttribute(LfoSpeed->paramID, (double)LfoSpeed->get());
}

void LfoParameters::loadParameters(XmlElement & xml)
{
	*LfoTarget = xml.getIntAttribute(LfoTarget->paramID, 0);
	*LfoWaveType = xml.getIntAttribute(LfoWaveType->paramID, 0);
	*LfoAmount = (float)xml.getDoubleAttribute(LfoAmount->paramID, 0.3);
	*LfoSpeed = (float)xml.getDoubleAttribute(LfoSpeed->paramID, 2.0);
}

FilterParameters::FilterParameters(AudioParameterChoice* type,
								AudioParameterFloat* frequency,
								AudioParameterFloat* q)
	: Type(type)
	, Frequency(frequency)
	, Q(q)
{
    Frequency->range.symmetricSkew = false;
    Frequency->range.skew = 0.3;
    
}

void FilterParameters::addAllParameters(AudioProcessor& processor)
{
	processor.addParameter(Type);
	processor.addParameter(Frequency);
	processor.addParameter(Q);
}

void FilterParameters::saveParameters(XmlElement & xml)
{
	xml.setAttribute(Type->paramID, Type->getIndex());
	xml.setAttribute(Frequency->paramID, (double)Frequency->get());
	xml.setAttribute(Q->paramID, (double)Q->get());
}

void FilterParameters::loadParameters(XmlElement & xml)
{
	*Type = xml.getIntAttribute(Type->paramID, 1);
	*Frequency = (float)xml.getDoubleAttribute(Frequency->paramID, 20000.0);
	*Q = (float)xml.getDoubleAttribute(Q->paramID, 1.0);
}

ReverbParameters::ReverbParameters(AudioParameterFloat* roomSize,
								AudioParameterFloat* damping,
								AudioParameterFloat* wetLevel,
								AudioParameterFloat* dryLevel,
								AudioParameterFloat* width,
								AudioParameterFloat* freezeMode)
	:RoomSize(roomSize)
	, Damping(damping)
	, WetLevel(wetLevel)
	, DryLevel(dryLevel)
	, Width(width)
	, FreezeMode(freezeMode)
{}

void ReverbParameters::addAllParameters(AudioProcessor& processor)
{
	processor.addParameter(RoomSize);
	processor.addParameter(Damping);
	processor.addParameter(WetLevel);
	processor.addParameter(DryLevel);
	processor.addParameter(Width);
	processor.addParameter(FreezeMode);
}

void ReverbParameters::saveParameters(XmlElement & xml)
{
	xml.setAttribute(RoomSize->paramID, (double)RoomSize->get());
	xml.setAttribute(Damping->paramID, (double)Damping->get());
	xml.setAttribute(WetLevel->paramID, (double)WetLevel->get());
	xml.setAttribute(DryLevel->paramID, (double)DryLevel->get());
	xml.setAttribute(Width->paramID, (double)Width->get());
	xml.setAttribute(FreezeMode->paramID, (double)FreezeMode->get());
}

void ReverbParameters::loadParameters(XmlElement & xml)
{
	*RoomSize = (float)xml.getDoubleAttribute(RoomSize->paramID, 0.3);
	*Damping = (float)xml.getDoubleAttribute(Damping->paramID, 0.3);
	*WetLevel = (float)xml.getDoubleAttribute(WetLevel->paramID, 0.0);
	*DryLevel = (float)xml.getDoubleAttribute(DryLevel->paramID, 1.0);
	*Width = (float)xml.getDoubleAttribute(Width->paramID, 0.3);
	*FreezeMode = (float)xml.getDoubleAttribute(FreezeMode->paramID, 0.0);
}
