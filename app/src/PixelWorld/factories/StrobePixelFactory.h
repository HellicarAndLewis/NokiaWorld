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

class StrobePixelFactory : public PixelFactory {
	
public: 
	
	void setup(int aChannelNumber);
	void update();
	void draw();
	
private:
	unsigned long timeToStrobe;
	unsigned long strobeDelay;
	unsigned long oldStrobeIn;
	float fadePCT;
	bool bFiring;
	bool bPeakCycleStarted;
	
	int strobeR, strobeG, strobeB;
	
	int maxChangeTime, minChangeTime, defaultChangeTime;	
};