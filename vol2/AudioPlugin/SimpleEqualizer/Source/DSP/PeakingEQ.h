/*
  ==============================================================================

    PeakingEQ.h
    Created: 21 Sep 2016 9:35:57pm
    Author:  COx2

  ==============================================================================
*/

#ifndef PEAKINGEQ_H_INCLUDED
#define PEAKINGEQ_H_INCLUDED

class PeakingEQ {
public:
	PeakingEQ();
	~PeakingEQ();
	
	void SetParameter(float samplerate, float freq, float bw, float gain);
	void DoProcess(float* bufferPtr, int bufferSize);
	float DoProcess(float input);

private:

	float a0, a1, a2, b0, b1, b2;

	float out1, out2;
	
	float in1, in2;

	float output;
};

#endif  // PEAKINGEQ_H_INCLUDED
