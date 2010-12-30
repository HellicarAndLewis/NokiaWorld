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

class SlitStretchFactory : public PixelFactory {
	
public: 
	SlitStretchFactory(){
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
	ofTexture textureToStretchVertically;
	ofTexture textureToStretchHorizontally;	
	bool inited;
	
	unsigned char * pixelsHorizontalStretchVertically;	
	unsigned char * pixelsVerticalStretchHorizontally;		

	string currentContentPath;
	int currentImageMode;
};
