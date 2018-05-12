/*
  ==============================================================================

    SimpleSound.cpp
    Created: 20 Apr 2018 2:07:49am
    Author:  COx2

  ==============================================================================
*/

#include "SimpleSound.h"

SimpleSound::SimpleSound(const BigInteger& notes)
	: midiNotes(notes)
{
}

SimpleSound::~SimpleSound()
{
}

bool SimpleSound::appliesToNote(int midiNoteNumber)
{
	return midiNotes[midiNoteNumber];
}

bool SimpleSound::appliesToChannel(int midiChannel)
{
	return true;
}