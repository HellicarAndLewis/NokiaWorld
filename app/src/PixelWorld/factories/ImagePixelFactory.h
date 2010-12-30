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

class ImagePixelFactory : public PixelFactory {
	
public: 
	ImagePixelFactory(){
		inited = false;
	}
	void setup(int aChannelNumber);
	void eventsIn(guiCallbackData & data);
	void update();
	void draw();
	
private:
	simpleFileLister lister;
	simpleFileLister content;
	vector <ofImage> images;	
	int imageIndexToUse;
	bool inited;
};