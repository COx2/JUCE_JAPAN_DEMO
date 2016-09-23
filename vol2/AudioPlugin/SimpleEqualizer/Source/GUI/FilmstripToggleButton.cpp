/*
*  FilmstripToggleButton.h
*  COx2
*
*  Created by Tatsuya Shiozawa on 23/03/2016.
*  Copyright 2016 COx2. All rights reserved.
*
*/

#include "FilmstripToggleButton.h"

// constructor with copies
FilmstripToggleButton::FilmstripToggleButton(const String &componentName)
	:   ToggleButton(componentName),
        numFrames (0),
        isHorizontal (true),
        frameWidth (0),
        frameHeight (0)
{
}

void FilmstripToggleButton::setImage (Image image, int numFrames_, bool isHorizontal_)
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

FilmstripToggleButton::~FilmstripToggleButton()
{
}

// draw image process.(automated call back)
void FilmstripToggleButton::paint(Graphics& g)
{
	if (filmStripImage.isValid())
	{
		bool value = getToggleState();
		int imageHeight = getWidth();;
		int imageWidth = getHeight();

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
