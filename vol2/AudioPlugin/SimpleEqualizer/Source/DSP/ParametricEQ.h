/*
  ==============================================================================

    ParametricEQ.h
    Created: 27 Sep 2016 1:51:16am
    Author:  COx2

  ==============================================================================
*/

// ‡@“ñdƒCƒ“ƒNƒ‹[ƒh–hŽ~‚Ì‚½‚ß‚ÌƒvƒŠƒvƒƒZƒbƒTiŽ©“®¶¬j
#ifndef PARAMETRICEQ_H_INCLUDED			//"PARAMETRICEQ_H_INCLUDED"‚ªdefine’è‹`‚³‚ê‚Ä‚¢‚È‚¯‚ê‚ÎAˆÈ‰º‚Ìˆ—‚ðŽÀs
#define PARAMETRICEQ_H_INCLUDED			//"PARAMETRICEQ_H_INCLUDED"‚ðdefine’è‹`‚·‚é

// ‡AƒNƒ‰ƒX–¼éŒ¾
class ParametricEQ {
public:								// ‡B•Ï”EŠÖ”‚ÌƒAƒNƒZƒXŒ ‚ðPublic‚ÉŽw’è‚·‚ééŒ¾
	ParametricEQ();					// ‡CƒRƒ“ƒXƒgƒ‰ƒNƒ^
	~ParametricEQ();				// ‡DƒfƒXƒgƒ‰ƒNƒ^
	void SetParameter(float samplerate, float freq, float bw, float gain);	// ‡EƒtƒBƒ‹ƒ^‚Ìƒpƒ‰ƒ[ƒ^‚ð•ÏX‚·‚éŠÖ”
	void DoProcess(float* bufferPtr, int bufferSize);						// ‡FƒfƒWƒ^ƒ‹ƒtƒBƒ‹ƒ^‚ð“K—p‚·‚éŠÖ”

private:
	float a0, a1, a2, b0, b1, b2;						// ‡G“`’BŠÖ”‚Ìƒpƒ‰ƒ[ƒ^‚ð•ÛŽ‚·‚é•Ï”
	float in1, in2;										// ‡HƒtƒBƒ‹ƒ^ŒvŽZ—p‚ÌˆêŽž•Û‘¶•Ï”i“ü—Í‘¤’x‰„Šíj
	float out1, out2;									// ‡HƒtƒBƒ‹ƒ^ŒvŽZ—p‚ÌˆêŽž•Û‘¶•Ï”io—Í‘¤’x‰„Šíj
};

// ‡@“ñdƒCƒ“ƒNƒ‹[ƒh–hŽ~‚Ì‚½‚ß‚ÌƒvƒŠƒvƒƒZƒbƒTiŽ©“®¶¬j
#endif  // PARAMETRICEQ_H_INCLUDED
