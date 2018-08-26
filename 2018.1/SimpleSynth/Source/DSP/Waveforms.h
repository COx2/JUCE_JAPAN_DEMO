/*
  ==============================================================================

    Waveforms.h
    Created: 22 Aug 2018 10:13:35pm
    Author:  COx2

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Waveforms
{
public:
    float sine(float angle);
    float saw(float angle);
	float square(float angle);
    float triangle(float angle);
    float noise();

private:
    Random whiteNoise;
};