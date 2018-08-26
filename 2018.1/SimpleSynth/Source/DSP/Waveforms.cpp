/*
  ==============================================================================

    Waveforms.cpp
    Created: 22 Aug 2018 10:13:35pm
    Author:  COx2

  ==============================================================================
*/

#include "Waveforms.h"

namespace {
	const float HALF_PI = MathConstants<float>::halfPi;
	const float ONE_PI = MathConstants<float>::pi;
	const float TWO_PI = MathConstants<float>::twoPi;
}

float Waveforms::sine(float angle)
{
    if(angle > TWO_PI)
    {
        angle = fmodf(angle, TWO_PI);
    }

    return sinf(angle);
}

float Waveforms::saw(float angle)
{
    if(angle > TWO_PI)
    {
        angle = fmodf(angle, TWO_PI);
    }

    if (angle <= ONE_PI)
    {
        return (angle / ONE_PI);
    }
    else
    {
        return -2.0f + (angle / ONE_PI) ;
    }
}

float Waveforms::square(float angle)
{

	if (angle > TWO_PI)
	{
		angle = fmodf(angle, TWO_PI);
	}

	if (angle <= ONE_PI)
	{
		return 1.0f;
	}
	else
	{
		return -1.0f;
	}
}


float Waveforms::triangle(float angle)
{
    if(angle > TWO_PI)
    {
        angle = fmodf(angle, TWO_PI);
    }

    if (angle <= HALF_PI)
    {
        return (angle / HALF_PI);
    }
    else if(angle > HALF_PI && angle <= (ONE_PI + HALF_PI))
    {
        return 2.0f - (2.0f * angle / ONE_PI);
    }
    else
    {
        return -4.0f + (angle / HALF_PI);
    }
}

float Waveforms::noise()
{
    return (whiteNoise.nextFloat() * 2.0f - 1.0f);
}