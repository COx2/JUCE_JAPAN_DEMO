/*
  ==============================================================================

    SimpleSound.cpp
    Created: 20 Apr 2018 2:07:49am
    Author:  COx2

  ==============================================================================
*/

#include "SimpleSound.h"

// コンストラクタは初期化指定子を設定する以外は何も行わない.
SimpleSound::SimpleSound(const BigInteger& notes, const BigInteger& channels)
	: midiNotes(notes), midiChannels(channels)
{
}

// デストラクタ
SimpleSound::~SimpleSound()
{
}

// サウンド再生可能なMIDIノートの範囲を設定する
bool SimpleSound::appliesToNote(int midiNoteNumber)
{
	DBG(juce::String("midiNoteNumber: ") + juce::String(midiNoteNumber));
	return midiNotes[midiNoteNumber];
}

// 
bool SimpleSound::appliesToChannel(int midiChannel)
{
	DBG(juce::String("midiChannel: ") + juce::String(midiChannel));
	return midiChannels[midiChannel];
}
