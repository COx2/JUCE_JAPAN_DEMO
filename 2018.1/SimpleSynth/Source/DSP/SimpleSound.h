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

	SimpleSound(const BigInteger& notes);
	virtual ~SimpleSound();

	//==============================================================================
	/** Returns true if this sound should be played when a given midi note is pressed.

	The Synthesiser will use this information when deciding which sounds to trigger
	for a given note.
	*/
	virtual bool appliesToNote(int midiNoteNumber) override;

	/** Returns true if the sound should be triggered by midi events on a given channel.

	The Synthesiser will use this information when deciding which sounds to trigger
	for a given note.
	*/
	virtual bool appliesToChannel(int midiChannel) override;

private:
	BigInteger midiNotes;

};