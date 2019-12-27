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

void FilmstripSlider::paint(Graphics& g) {
    if (filmStripImage.isValid()) {

        int value = (int)((getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (numFrames - 1));

        int imageHeight, imageWidth, _destX, _destY;

        switch (getTextBoxPosition()) {
        case TextBoxBelow:
            imageWidth = getWidth() - getTextBoxHeight();
            imageHeight = getHeight() - getTextBoxHeight();
            _destX = (getWidth() - imageWidth) * 0.5;
            _destY = (getHeight() - imageHeight) * 0.0;
            break;
        case TextBoxAbove:
            imageWidth = getWidth() - getTextBoxHeight();
            imageHeight = getHeight() - getTextBoxHeight();
            _destX = (getWidth() - imageWidth) * 0.5;
            _destY = (getHeight() - imageHeight) * 1.0;
            break;
        case TextBoxLeft:
            imageWidth = getWidth() - getTextBoxWidth();
            imageHeight = getHeight() - getTextBoxWidth();
            _destX = (getWidth() - imageWidth) * 1.0;
            _destY = (getHeight() - imageHeight) * 0.5;
            break;
        case TextBoxRight:
            imageWidth = getWidth() - getTextBoxWidth();
            imageHeight = getHeight() - getTextBoxWidth();
            _destX = (getWidth() - imageWidth) * 0.0;
            _destY = (getHeight() - imageHeight) * 0.5;
            break;
        default:
            imageWidth = getWidth();
            imageHeight = getHeight();
            _destX = 0;
            _destY = 0;
            break;
        }

        if (isHorizontal) {
            g.drawImage(filmStripImage, _destX, _destY, imageWidth, imageHeight,
                value * frameWidth, 0, frameWidth, frameHeight);
        }
        else {
            g.drawImage(filmStripImage, _destX, _destY, imageWidth, imageHeight,
                0, value * frameHeight, frameWidth, frameHeight);
        }
    }
}
