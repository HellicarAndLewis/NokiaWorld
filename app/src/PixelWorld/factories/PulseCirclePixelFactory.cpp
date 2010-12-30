/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "PulseCirclePixelFactory.h"

void PulseCirclePixelFactory::setup(int aChannelNumber){
	
	wWidth = settings.OUTPUT_CROP_W;
	wHeight = settings.OUTPUT_CROP_H;	
	
	factoryName = "PCircle"+ofToString(aChannelNumber);
	
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

void PulseCirclePixelFactory::update(){
	panel.update();

	peak = ofMap(level, 0.0, 1.0, 0.0, (settings.OUTPUT_CROP_W/2.0) + 10); //extra to take it over the edge	
}

void PulseCirclePixelFactory::draw(){	
	ofPushStyle();
	
	float gradientVal;
	
	
	int gradR, gradG, gradB;
	
		//draw bg
	ofFill();
	ofSetColor(bgColor.x,	// red
			   bgColor.y,	// green
			   bgColor.z);	// blue
	ofRect(0,0,settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);
		
	ofSetColor(fgColor.x,	// red
			   fgColor.y,	// green
			   fgColor.z);	// blue
	
	ofCircle(settings.OUTPUT_CROP_W/2, settings.OUTPUT_CROP_H/2, peak);	
	
	ofSetColor(0,	// red
			   0,	// green
			   0);	// blue	
	ofRect(0,settings.OUTPUT_CROP_H ,settings.OUTPUT_CROP_W,ofGetHeight());	
	
	ofPopStyle();	
	
	drawTrails();		
}


