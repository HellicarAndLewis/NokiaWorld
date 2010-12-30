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

#include "particle.h"
#include "spring.h"

class StringPixelFactory : public PixelFactory {
	
public: 
	
	void setup(int aChannelNumber);
	void update();
	void draw();
	
private:
		// let's make a vector of them
	vector <particle> particles;
	vector <spring> springs;
	
	float happiness;
	
	vector <ofPoint> ptsForRepulsion;
};