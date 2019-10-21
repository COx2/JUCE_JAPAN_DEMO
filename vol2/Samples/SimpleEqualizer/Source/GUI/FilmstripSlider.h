/*
 *  FilmstripSlider.h
 *  dRowAudio
 *
 *  Created by David Rowland on 19/07/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _FILMSTRIPSLIDER_H_
#define _FILMSTRIPSLIDER_H_

#include "../JuceLibraryCode/JuceHeader.h"

/**
	A slider class that takes a film strip image to display the graphics of it.
 */
class FilmstripSlider : public Slider
{
public:
	/**
		Constructor.
		You need to supply the image file and the number of frames it represents for correct image display.
		Optional arguments show or hide the default number box and if the filmstrip image is horizontal or vertical.
	 */
	FilmstripSlider(const String& componentName = String::empty);
		
	///	Destructor.
	~FilmstripSlider();

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

#endif //_FILMSTRIPSLIDER_H_