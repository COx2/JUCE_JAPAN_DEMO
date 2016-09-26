/*
  ==============================================================================

    ParametricEQ.h
    Created: 27 Sep 2016 1:51:16am
    Author:  COx2

  ==============================================================================
*/

// ①二重インクルード防止のためのプリプロセッサ（自動生成）
#ifndef PARAMETRICEQ_H_INCLUDED			//"PARAMETRICEQ_H_INCLUDED"がdefine定義されていなければ、以下の処理を実行
#define PARAMETRICEQ_H_INCLUDED			//"PARAMETRICEQ_H_INCLUDED"をdefine定義する

// ②クラス名宣言
class ParametricEQ {
public:								// ③変数・関数のアクセス権をPublicに指定する宣言
	ParametricEQ();					// ④コンストラクタ
	~ParametricEQ();				// ⑤デストラクタ
	void SetParameter(float samplerate, float freq, float bw, float gain);	// ⑥フィルタのパラメータを変更する関数
	void DoProcess(float* bufferPtr, int bufferSize);						// ⑦デジタルフィルタを適用する関数

private:
	float a0, a1, a2, b0, b1, b2;						// ⑧伝達関数のパラメータを保持する変数
	float in1, in2;										// ⑨フィルタ計算用の一時保存変数（入力側遅延器）
	float out1, out2;									// ⑨フィルタ計算用の一時保存変数（出力側遅延器）
};

// ①二重インクルード防止のためのプリプロセッサ（自動生成）
#endif  // PARAMETRICEQ_H_INCLUDED
