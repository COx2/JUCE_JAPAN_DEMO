/*
  ==============================================================================

    PeakingEQ.cpp
    Created: 21 Sep 2016 9:35:57pm
    Author:  COx2

  ==============================================================================
*/

#include "PeakingEQ.h"
#include <Math.h>

PeakingEQ::PeakingEQ()
	:	a0(1.0f), a1(0.0f), a2(0.0f),
		b0(1.0f), b1(0.0f), b2(0.0f),
		in1(0.0f), in2(0.0f),
		out1(0.0f), out2(0.0f)
{
}

PeakingEQ::~PeakingEQ() {
}

/// <summary>
/// float samplerate �c �T���v�����O���g��
/// float freq �c ���S���g��(Hz)
/// float bw   �c �ш敝(Octave)
/// float gain �c ������(dB)
/// H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1)
/// </summary>
void PeakingEQ::SetParameter(float samplerate, float frequency, float bw, float gain) {

	float omega = 2.0f * 3.14159265f *  frequency / samplerate;
	float sinw = sin(omega);
	float cosw = cos(omega);
	float alpha = sinw * sinh(log(2.0f) / 2.0 * bw * omega / sinw);
	float A = pow(10.0f, (gain / 40.0f));

	a0 = 1.0f + alpha / A;
	a1 = -2.0f * cosw;
	a2 = 1.0f - alpha / A;
	b0 = 1.0f + alpha * A;
	b1 = -2.0f * cosw;
	b2 = 1.0f - alpha * A;
}

// float bufferPtr  �c �I�[�f�B�I�o�b�t�@�̃|�C���^
// int   bufferSize �c �I�[�f�B�I�o�b�t�@�̃T�C�Y
void PeakingEQ::DoProcess(float* bufferPtr, int bufferSize)
{
	// ���͐M���Ƀt�B���^��K�p���A�o�͐M���Ƃ��ď����o��
	for (int i = 0; i < bufferSize; i++)
	{
		// H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1)
		float out0 = b0 / a0 * bufferPtr[i] 
			+ b1 / a0 * in1 
			+ b2 / a0 * in2
			- a1 / a0 * out1 
			- a2 / a0 * out2;

		in2 = in1;           // 2�O�̓��͐M�����X�V
		in1 = bufferPtr[i];  // 1�O�̓��͐M�����X�V

		out2 = out1;         // 2�O�̏o�͐M�����X�V
		out1 = out0;         // 1�O�̏o�͐M�����X�V

		bufferPtr[i] = out0; // �o�b�t�@�ɏo�͐M������������
	}
}