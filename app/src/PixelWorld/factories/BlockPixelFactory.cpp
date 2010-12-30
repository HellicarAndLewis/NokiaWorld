/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "BlockPixelFactory.h"

void BlockPixelFactory::setup(int aChannelNumber){
	
	tWidth = settings.OUTPUT_CROP_W;
	tHeight = settings.OUTPUT_CROP_H;
	
	numDivisions = 4;

	filled = true;
	
	factoryName = "Block"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));	
	
	panel.setup("FACTORY "+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 300, 350);
	panel.addPanel("visuals", 1, false);
	panel.setWhichPanel("visuals");
	
	PixelFactory::addPixelFactoryStandardGUIElements();
	
	panel.addSlider("divisions", "VIS_DIV", 4, 1, 10, true);
	panel.addToggle("Filled", "VIS_FILLED", true);	
	
	panel.loadSettings(factoryName + "Settings.xml");
}

void BlockPixelFactory::update(){
	panel.update();
	
	size = ofMap(level, 0.0, 1.0, 0.0, xSpacing);
}

void BlockPixelFactory::draw(){	
	numDivisions = panel.getValueF("VIS_DIV");
	filled = panel.getValueB("VIS_FILLED");
	xSpacing = tWidth / ((float)SCREEN_HORIZ_RATIO * numDivisions);
	ySpacing = tHeight / ((float)SCREEN_VERT_RATIO * numDivisions);	
	
	ofPushStyle();
	
		//draw bg
	ofFill();
	ofSetColor(bgColor.x,	// red
			   bgColor.y,	// green
			   bgColor.z);	// blue
	ofRect(0,0,settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);
		
	ofSetColor(fgColor.x,	// red
			   fgColor.y,	// green
			   fgColor.z);	// blue
	
	if(filled)
		ofFill();
	else
		ofNoFill();
	
	for(int i = 0; i < (numDivisions * SCREEN_HORIZ_RATIO); i++){
		for(int j = 0; j < (numDivisions * SCREEN_VERT_RATIO); j++){
			if(ofRandom(0.0, 1.0) < level){
				ofRect((i+0.5)*xSpacing-size/2, (j+0.5)*ySpacing-size/2, size, size);
			}
		}
	}
	
	ofPopStyle();	
	
	drawTrails();		
}


