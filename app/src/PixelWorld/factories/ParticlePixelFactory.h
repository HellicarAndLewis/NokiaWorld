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

#include "ofxBox2d.h"

	// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	ofColor color;
	void draw() {
		float radius = getRadius();
		
		if (getPosition().x - radius > settings.OUTPUT_CROP_W) return;
		if (getPosition().x + radius < 0) return;
		if (getPosition().y - radius > settings.OUTPUT_CROP_H) return;
		if (getPosition().y + radius < 0) return;		
		
		ofSetColor(color.r, color.g, color.b);
		ofFill();
		ofCircle(getPosition().x, getPosition().y, radius);	
	}
};

class ParticlePixelFactory : public PixelFactory {
	
public: 
	
	void setup(int aChannelNumber);
	void update();
	void draw();
	
private:
	float px, py;
	bool							bDrawLines;
	bool							bMouseForce;
	
	ofxBox2d						box2d;			  //	the box2d world
	vector		<CustomParticle>	customParticles;  //	this is a custom particle the extends a cirlce
};