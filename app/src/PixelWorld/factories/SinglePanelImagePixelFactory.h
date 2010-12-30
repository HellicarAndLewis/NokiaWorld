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

class SinglePanelImagePixelFactory : public PixelFactory {
	
public: 
	SinglePanelImagePixelFactory(){
		inited = false;
	}
	void setup(int aChannelNumber);
	void eventsIn(guiCallbackData & data);
	void update();
	void draw();
	
	int makeIndexSafe(int indexInQuestion);
	int getPanelIndex();
	
private:
	simpleFileLister lister;
	simpleFileLister content;
	vector <ofImage> images;	
	int imageIndexToUse;
	bool inited;
	int currentSequentialIndex;
};