/*
  ==============================================================================

    ParametricEQ.h
    Created: 27 Sep 2016 1:51:16am
    Author:  COx2

  ==============================================================================
*/

// �@��d�C���N���[�h�h�~�̂��߂̃v���v���Z�b�T�i���������j
#ifndef PARAMETRICEQ_H_INCLUDED			//"PARAMETRICEQ_H_INCLUDED"��define��`����Ă��Ȃ���΁A�ȉ��̏��������s
#define PARAMETRICEQ_H_INCLUDED			//"PARAMETRICEQ_H_INCLUDED"��define��`����

// �A�N���X���錾
class ParametricEQ {
public:								// �B�ϐ��E�֐��̃A�N�Z�X����Public�Ɏw�肷��錾
	ParametricEQ();					// �C�R���X�g���N�^
	~ParametricEQ();				// �D�f�X�g���N�^
	void SetParameter(float samplerate, float freq, float bw, float gain);	// �E�t�B���^�̃p�����[�^��ύX����֐�
	void DoProcess(float* bufferPtr, int bufferSize);						// �F�f�W�^���t�B���^��K�p����֐�

private:
	float a0, a1, a2, b0, b1, b2;						// �G�`�B�֐��̃p�����[�^��ێ�����ϐ�
	float in1, in2;										// �H�t�B���^�v�Z�p�̈ꎞ�ۑ��ϐ��i���͑��x����j
	float out1, out2;									// �H�t�B���^�v�Z�p�̈ꎞ�ۑ��ϐ��i�o�͑��x����j
};

// �@��d�C���N���[�h�h�~�̂��߂̃v���v���Z�b�T�i���������j
#endif  // PARAMETRICEQ_H_INCLUDED
