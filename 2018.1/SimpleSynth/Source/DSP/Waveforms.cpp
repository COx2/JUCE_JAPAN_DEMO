/*
  ==============================================================================

    Waveforms.cpp
    Created: 22 Aug 2018 10:13:35pm
    Author:  COx2

  ==============================================================================
*/

#include "Waveforms.h"

float Waveforms::sine(float angle)
{
    if(angle > MathConstants<float>::twoPi)
    {
        angle = fmodf(angle, MathConstants<float>::twoPi);
    }

    return sinf(angle);
}

float Waveforms::saw(float angle)
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

float Waveforms::triangle(float angle)
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

float Waveforms::square(float angle)
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

float Waveforms::noise()
{
    return (whiteNoise.nextFloat() * 2.0f - 1.0f);
}