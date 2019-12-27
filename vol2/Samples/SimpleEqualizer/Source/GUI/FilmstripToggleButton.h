/*
 *  FilmstripToggleButton.h
 *  COx2
 *
 *  Created by Tatsuya Shiozawa on 23/03/2016.
 *  Copyright 2016 COx2. All rights reserved.
 *
 */

#ifndef _FILMSTRIPTOGGLEBUTTON_H_
#define _FILMSTRIPTOGGLEBUTTON_H_

#include "../JuceLibraryCode/JuceHeader.h"

/**
    A toggle button class that takes a film strip image to display the graphics of it.
 */
class FilmstripToggleButton : public ToggleButton
{
public:
    /**
        Constructor.
        You need to supply the image file and the number of frames it represents for correct image display.
        Optional arguments show or hide the default number box and if the filmstrip image is horizontal or vertical.
     */
    FilmstripToggleButton(const String& componentName = String::empty);

    //  Destructor.
    ~FilmstripToggleButton();

    /** Sets the image to use, you need to supply the number of frames within the image.
     */
    void setImage (Image image, int numFrames, bool isHorizontal = true);

    /** Returns the width of one frame of the filmstrip in pixels.
     */
    int getFrameWidth() const       { return frameWidth;    }

    /** Returns the height of one frame of the filmstrip in pixels.
     */
    int getFrameHeight() const      { return frameHeight;   }

    /** @interal
     */
    void paint(Graphics& g);

private:
    Image filmStripImage;
    int numFrames;
    bool isHorizontal;
    int frameWidth, frameHeight;
};

#endif //_FILMSTRIPTOGGLEBUTTON_H_
