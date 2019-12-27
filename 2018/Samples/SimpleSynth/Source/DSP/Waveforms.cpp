/*
  ==============================================================================

    Waveforms.cpp
    Created: 22 Aug 2018 10:13:35pm
    Author:  COx2

  ==============================================================================
*/

// ヘッダファイルをインクルードする。
#include "Waveforms.h"

// ①無名名前空間に定数宣言を記述する。
namespace {
    const float HALF_PI = MathConstants<float>::halfPi;
    const float ONE_PI = MathConstants<float>::pi;
    const float TWO_PI = MathConstants<float>::twoPi;
}

// ②正弦波のサンプルデータを生成して返す関数
float Waveforms::sine(float angle)
{
    // 角度（ラジアン）の値が2πを超えている場合は、変数angleの値が0～2πの範囲内に収まるよう剰余を求める。
    if(angle > TWO_PI)
    {
        angle = fmodf(angle, TWO_PI);
    }

    // 角度（ラジアン）に対応する波形のサンプルデータを返す。
    return sinf(angle);
}

// ③鋸波のサンプルデータを生成して返す関数
float Waveforms::saw(float angle)
{
    if(angle > TWO_PI)
    {
        angle = fmodf(angle, TWO_PI);
    }

    if (angle <= ONE_PI)
    {
        return (angle / ONE_PI);
    }
    else
    {
        return -2.0f + (angle / ONE_PI) ;
    }
}

// ④方形波のサンプルデータを生成して返す関数
float Waveforms::square(float angle)
{

    if (angle > TWO_PI)
    {
        angle = fmodf(angle, TWO_PI);
    }

    if (angle <= ONE_PI)
    {
        return 1.0f;
    }
    else
    {
        return -1.0f;
    }
}

// ⑤三角波のサンプルデータを生成して返す関数
float Waveforms::triangle(float angle)
{
    if(angle > TWO_PI)
    {
        angle = fmodf(angle, TWO_PI);
    }

    if (angle <= HALF_PI)
    {
        return (angle / HALF_PI);
    }
    else if(angle > HALF_PI && angle <= (ONE_PI + HALF_PI))
    {
        return 2.0f - (2.0f * angle / ONE_PI);
    }
    else
    {
        return -4.0f + (angle / HALF_PI);
    }
}

// ⑥ホワイトノイズのサンプルデータを生成して返す関数
float Waveforms::noise()
{
    // juce::Randomオブジェクトのメンバ関数を実行してfloat型のサンプルデータを生成して返す。
    // nextFloat関数の返り値は0.0～1.0の範囲内なので、0.0～1.0の値を-1.0～1.0の範囲の値となるよう正規化する。
    return (whiteNoise.nextFloat() * 2.0f - 1.0f);
}
