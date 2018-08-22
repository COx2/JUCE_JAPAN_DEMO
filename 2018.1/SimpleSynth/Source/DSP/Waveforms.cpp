/*
  ==============================================================================

    Waveforms.cpp
    Created: 22 Aug 2018 10:13:35pm
    Author:  COx2

  ==============================================================================
*/

#include "Waveforms.h"

float Waveforms::sineWave(float angle)
{
    if(angle > MathConstants<float>::twoPi)
    {
        angle = fmodf(angle, MathConstants<float>::twoPi);
    }

    return sinf(angle);
}

float Waveforms::sawWave(float angle)
{
    if(angle > MathConstants<float>::twoPi)
    {
        angle = fmodf(angle, MathConstants<float>::twoPi);
    }


    if (angle <= MathConstants<float>::pi)
    {
        return (angle / MathConstants<float>::pi);
    }
    else
    {
        return -1.0f + ((angle - MathConstants<float>::pi) / MathConstants<float>::pi) ;
    }
}

float Waveforms::triangleWave(float angle)
{
    if(angle > MathConstants<float>::twoPi)
    {
        angle = fmodf(angle, MathConstants<float>::twoPi);
    }

    if (angle <= MathConstants<float>::halfPi)
    {
        return (angle / MathConstants<float>::halfPi);
    }
    else if(angle > MathConstants<float>::halfPi && angle <= (MathConstants<float>::pi + MathConstants<float>::halfPi))
    {
        return 1.0f - (2.0f * ((angle - MathConstants<float>::halfPi) / MathConstants<float>::pi));
    }
    else
    {
        return -1.0f + ((angle - MathConstants<float>::pi - MathConstants<float>::halfPi) / MathConstants<float>::halfPi);
    }
}

float Waveforms::squareWave(float angle)
{

    if(angle > MathConstants<float>::twoPi)
    {
        angle = fmodf(angle, MathConstants<float>::twoPi);
    }

    if (angle <= MathConstants<float>::pi)
    {
        return 1.0f;
    }
    else
    {
        return -1.0f;
    }
}

float Waveforms::whiteNoise()
{
    return (noiseSource.nextFloat() * 2.0f - 1.0f);
}