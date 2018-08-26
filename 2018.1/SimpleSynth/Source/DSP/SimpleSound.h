/*
  ==============================================================================

    SimpleSound.h
    Created: 20 Apr 2018 2:07:49am
    Author:  COx2

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SimpleSound : public SynthesiserSound
{
public:
	// 
	SimpleSound(const BigInteger& notes, const BigInteger& channels);
	virtual ~SimpleSound();

	// 
	virtual bool appliesToNote(int midiNoteNumber) override;

	// 
	virtual bool appliesToChannel(int midiChannel) override;

private:
	// 
	BigInteger midiNotes;
	BigInteger midiChannels;

};