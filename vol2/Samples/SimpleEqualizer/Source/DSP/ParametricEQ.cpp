/*
  ==============================================================================
    ParametricEQ.cpp
    Created: 27 Sep 2016 1:51:16am
    Author:  COx2
  ==============================================================================
*/

#include "ParametricEQ.h"
#include <Math.h>

// ①コンストラクタと変数の初期化
ParametricEQ::ParametricEQ()
	: a0(1.0f), a1(0.0f), a2(0.0f),
	b0(1.0f), b1(0.0f), b2(0.0f),
	in1(0.0f), in2(0.0f),
	out1(0.0f), out2(0.0f)
{
}

//	デストラクタ
ParametricEQ::~ParametricEQ() {
}

// ②フィルタパラメータを計算して保持する関数
// float samplerate … サンプリング周波数
// float freq … 中心周波数(Hz)
// float bw   … 帯域幅(Octave)
// float gain … 増幅量(dB)
// H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1)
void ParametricEQ::SetParameter(float samplerate, float frequency, float bw, float gain) {

	float omega = 2.0f * 3.14159265f *  frequency / samplerate;					//	角周波数ωの計算
	float sinw = sin(omega);													//	sin(ω)の計算
	float cosw = cos(omega);													//	cos(ω)の計算
	float alpha = sinw * sinh(log(2.0f) / 2.0 * bw * omega / sinw);				//
	float A = pow(10.0f, (gain / 40.0f));										//

	a0 = 1.0f + alpha / A;
	a1 = -2.0f * cosw;
	a2 = 1.0f - alpha / A;
	b0 = 1.0f + alpha * A;
	b1 = -2.0f * cosw;
	b2 = 1.0f - alpha * A;
}

// ③オーディオバッファにフィルタを適用する処理
// float bufferPtr  … オーディオバッファのポインタ
// int  bufferSize … オーディオバッファのサイズ
void ParametricEQ::DoProcess(float* bufferPtr, int bufferSize) {
	// バッファのサンプル毎にフィルタ処理を繰り返し実行する
	for (int i = 0; i < bufferSize; i++) {
		// 入力信号にフィルタを適用し、出力信号として書き出す
		// H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1)
		float out0 = b0 / a0 * bufferPtr[i] + b1 / a0 * in1 + b2 / a0 * in2
			- a1 / a0 * out1 - a2 / a0 * out2;

		in2 = in1;           // 2つ前の入力信号を更新
		in1 = bufferPtr[i];  // 1つ前の入力信号を更新

		out2 = out1;         // 2つ前の出力信号を更新
		out1 = out0;         // 1つ前の出力信号を更新

		bufferPtr[i] = out0; // バッファに出力信号を書き込む
	}
}