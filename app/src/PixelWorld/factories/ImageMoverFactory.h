/*
 *  PixelFactory.h
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"
#include "PixelFactory.h"

class ImageMoverFactory : public PixelFactory {
	
public: 
	ImageMoverFactory(){
		inited = false;
	}
	void setup(int aChannelNumber);
	void eventsIn(guiCallbackData & data);
	void update();
	void draw();
	
	void doImageResize();
private:
	simpleFileLister lister;
	ofImage image;
	ofTexture textureToSlitStretchInto;
	bool inited;
	
	unsigned char * pixels;	
	unsigned char * pixelsFromImage;		

	string currentContentPath;
	int currentImageMode;
};
