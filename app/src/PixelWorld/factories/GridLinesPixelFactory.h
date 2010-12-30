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

class GridLinesPixelFactory : public PixelFactory {
	
public: 
	
	void setup(int aChannelNumber);
	void update();
	void draw();
	
private:
	int tWidth;
	int tHeight;
	
	float xDivisions;
	float yDivisions;
	float xSpacing;
	float ySpacing;
	
	float xRotation;
	float yRotation;
	float zRotation;
	
	float xLength;
	float yLength;
	float thickness;
	
	bool filled;
};