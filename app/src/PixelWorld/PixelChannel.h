/*
 *  PixelChannel.h
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "AppSettings.h"

#include "PixelFactory.h"

#include "SinglePeakPixelFactory.h"
#include "ImagePixelFactory.h"
#include "GridLinesPixelFactory.h"
#include "StringPixelFactory.h"
#include "StrobePixelFactory.h"
#include "PulseCirclePixelFactory.h"
#include "BlockPixelFactory.h"
#include "ParticlePixelFactory.h"
#include "ImageManipulationPixelFactory.h"
#include "SinglePanelImagePixelFactory.h"
#include "NoisePixelFactory.h"
//#include "ImageMoverFactory.h"
#include "SlitStretchFactory.h";
#include "FFTPixelFactory.h"

#define FACTORIES_COUNT 13

class PixelChannel{
public:
	PixelChannel(){		
		channelName = "unnamed"; 
	}	
	
	void setup(int aChannelNumber);
	void update();
	void draw(float alpha);
	void drawViaTexture(float alpha, ofRectangle pos);
	
	void setColor(ofPoint color, bool bFg);
	void setLevelLevelPeakSpace(float _level, float  _levelPeak, bool _space, vector <float> fftLevels);
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();	
	
	void drawGUI();
	
	ofxControlPanel channelGUI;
	
	void drawFactoryGUI();
	
public:
	PixelFactory *	factories[FACTORIES_COUNT];
	int				factoriesCount;
	int				currentFactory;
	int				channelNumber;
	string			channelName;
};
