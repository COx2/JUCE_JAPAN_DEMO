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
    float sineWave(float angle);
    float sawWave(float angle);
    float triangleWave(float angle);
    float squareWave(float angle);
    float whiteNoise();

private:
    Random noiseSource;
};