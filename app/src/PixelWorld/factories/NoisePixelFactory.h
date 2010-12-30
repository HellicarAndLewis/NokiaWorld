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

#include "ofxNoise.h"

class NoisePixelFactory : public PixelFactory {
	
public: 
	
	void setup(int aChannelNumber);
	void update();
	void draw();
	
private:
	ofxPerlin *noise;
	
	ofTexture 		texColour;
	
	int 			w, h;
	
	unsigned char 	* colourPixels;
};