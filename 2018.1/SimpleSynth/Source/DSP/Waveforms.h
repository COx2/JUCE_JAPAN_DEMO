/*
  ==============================================================================

    Waveforms.h
    Created: 22 Aug 2018 10:13:35pm
    Author:  COx2

  ==============================================================================
*/

#pragma once

// ①JUCEライブラリのヘッダをインクルードする。
#include "../JuceLibraryCode/JuceHeader.h"

// ②クラス宣言
class Waveforms
{
public:
	// ③各種波形のサンプルデータを返す関数を宣言する。
    float sine(float angle);
    float saw(float angle);
	float square(float angle);
    float triangle(float angle);
    float noise();

private:
	// ④ホワイトノイズを生成するjuce::Randomクラスのオブジェクトを宣言する。
    Random whiteNoise;
};