/*
  ==============================================================================

    ParametricEQ.cpp
    Created: 27 Sep 2016 1:51:16am
    Author:  COx2

  ==============================================================================
*/

#include "ParametricEQ.h"
#include <Math.h>

// ‡@ƒRƒ“ƒXƒgƒ‰ƒNƒ^‚Æ•Ï”‚Ì‰Šú‰»
ParametricEQ::ParametricEQ()
	: a0(1.0f), a1(0.0f), a2(0.0f),
	b0(1.0f), b1(0.0f), b2(0.0f),
	in1(0.0f), in2(0.0f),
	out1(0.0f), out2(0.0f)
{
}

//	ƒfƒXƒgƒ‰ƒNƒ^
ParametricEQ::~ParametricEQ() {
}

// ‡AƒtƒBƒ‹ƒ^ƒpƒ‰ƒ[ƒ^‚ðŒvŽZ‚µ‚Ä•ÛŽ‚·‚éŠÖ”
// float samplerate c ƒTƒ“ƒvƒŠƒ“ƒOŽü”g”
// float freq c ’†SŽü”g”(Hz)
// float bw   c ‘Ñˆæ•(Octave)
// float gain c ‘•—Ê(dB)
// H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1)
void ParametricEQ::SetParameter(float samplerate, float frequency, float bw, float gain) {

	float omega = 2.0f * 3.14159265f *  frequency / samplerate;					//	ŠpŽü”g”ƒÖ‚ÌŒvŽZ
	float sinw = sin(omega);													//	sin(ƒÖ)‚ÌŒvŽZ
	float cosw = cos(omega);													//	cos(ƒÖ)‚ÌŒvŽZ
	float alpha = sinw * sinh(log(2.0f) / 2.0f * bw * omega / sinw);			//
	float A = pow(10.0f, (gain / 40.0f));										//

	a0 = 1.0f + alpha / A;
	a1 = -2.0f * cosw;
	a2 = 1.0f - alpha / A;
	b0 = 1.0f + alpha * A;
	b1 = -2.0f * cosw;
	b2 = 1.0f - alpha * A;
}

// ‡BƒI[ƒfƒBƒIƒoƒbƒtƒ@‚ÉƒtƒBƒ‹ƒ^‚ð“K—p‚·‚éˆ—
// float bufferPtr  c ƒI[ƒfƒBƒIƒoƒbƒtƒ@‚Ìƒ|ƒCƒ“ƒ^
// int  bufferSize c ƒI[ƒfƒBƒIƒoƒbƒtƒ@‚ÌƒTƒCƒY
void ParametricEQ::DoProcess(float* bufferPtr, int bufferSize) {
	// ƒoƒbƒtƒ@‚ÌƒTƒ“ƒvƒ‹–ˆ‚ÉƒtƒBƒ‹ƒ^ˆ—‚ðŒJ‚è•Ô‚µŽÀs‚·‚é
	for (int i = 0; i < bufferSize; i++) {
		// “ü—ÍM†‚ÉƒtƒBƒ‹ƒ^‚ð“K—p‚µAo—ÍM†‚Æ‚µ‚Ä‘‚«o‚·
		// H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1)
		float out0 = b0 / a0 * bufferPtr[i] + b1 / a0 * in1 + b2 / a0 * in2
			- a1 / a0 * out1 - a2 / a0 * out2;

		in2 = in1;           // 2‚Â‘O‚Ì“ü—ÍM†‚ðXV
		in1 = bufferPtr[i];  // 1‚Â‘O‚Ì“ü—ÍM†‚ðXV

		out2 = out1;         // 2‚Â‘O‚Ìo—ÍM†‚ðXV
		out1 = out0;         // 1‚Â‘O‚Ìo—ÍM†‚ðXV

		bufferPtr[i] = out0; // ƒoƒbƒtƒ@‚Éo—ÍM†‚ð‘‚«ž‚Þ
	}
}