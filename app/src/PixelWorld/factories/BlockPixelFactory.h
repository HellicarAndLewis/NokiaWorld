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

#define SCREEN_HORIZ_RATIO 15
#define SCREEN_VERT_RATIO 1

class BlockPixelFactory : public PixelFactory {
	
public: 
	
	void setup(int aChannelNumber);
	void update();
	void draw();
	
private:
	int tWidth;
	int tHeight;
	
	int numDivisions;
	
	float xDivisions;
	float yDivisions;
	float xSpacing;
	float ySpacing;
	
	float size;
	
	bool filled;
};