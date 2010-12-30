/*
 *  PixelWorld.h
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#pragma once

#include "PixelMixer.h"

#include "ofMain.h"

class PixelWorld{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	void audioReceived(float * input, int bufferSize, int nChannels);
	
private:
	PixelMixer mixer;
};