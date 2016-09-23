/*
 *  FilmstripSlider.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 19/07/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "FilmstripSlider.h"

FilmstripSlider::FilmstripSlider (const String &componentName)
	:   Slider(componentName),
        numFrames (0),
        isHorizontal (true),
        frameWidth (0),
        frameHeight (0)
{
}

void FilmstripSlider::setImage (Image image, int numFrames_, bool isHorizontal_)
{
    filmStripImage = image;

	if(filmStripImage.isValid())
	{
        numFrames = numFrames_;
        isHorizontal = isHorizontal_;
        
        if(isHorizontal)
        {
            frameHeight = filmStripImage.getHeight();
            frameWidth = filmStripImage.getWidth() / numFrames;
        }
        else
        {
            frameHeight = filmStripImage.getHeight() / numFrames;
            frameWidth = filmStripImage.getWidth();
        }
	}
    else
    {
        numFrames = 0;
    }

    repaint();
}

FilmstripSlider::~FilmstripSlider()
{
}

void FilmstripSlider::paint(Graphics& g)
{
	if (filmStripImage.isValid())
	{
		int value = (int) ((getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (numFrames - 1));

		int imageHeight; 
		int imageWidth;
        
		if (getTextBoxPosition() == TextBoxBelow)
        {
			imageWidth = getWidth() - getTextBoxHeight();
			imageHeight = getHeight() - getTextBoxHeight();
		}
		else
        {
			imageWidth = getWidth();
			imageHeight = getHeight();
		}
		
		if(isHorizontal)
        {
			g.drawImage(filmStripImage, (getWidth() - imageWidth) * 0.5, 0, imageWidth, imageHeight,
						value * frameWidth, 0, frameWidth, frameHeight);
        }
		else
        {
			g.drawImage(filmStripImage, 0, 0, imageWidth, imageHeight,
						0, value * frameHeight, frameWidth, frameHeight);
        }
	}
}
