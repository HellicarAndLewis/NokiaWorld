/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "SinglePeakPixelFactory.h"

void SinglePeakPixelFactory::setup(int aChannelNumber){
	
	wWidth = settings.OUTPUT_CROP_W;
	wHeight = settings.OUTPUT_CROP_H;	
	
	factoryName = "SinglePeakPixelFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));	
	
	panel.setup("FACTORY "+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 400, 400);
	panel.addPanel("visuals", 1, false);
	panel.setWhichPanel("visuals");
	
	PixelFactory::addPixelFactoryStandardGUIElements();
	
	gradientModes.push_back("No Level");
	gradientModes.push_back("Gradient");
	gradientModes.push_back("Solid");
	gradientModes.push_back("Line - level thick");
	gradientModes.push_back("Line - slider thick");
	panel.addMultiToggle("Effect", "VIS_EFFECT_NAME", 0, gradientModes);
	panel.addSlider("Line Thickness", "VIS_THICKNESS", 10, 1, 20, true);
	
	panel.loadSettings(factoryName + "Settings.xml");
}

void SinglePeakPixelFactory::update(){
	panel.update();
	
	effectNo = panel.getValueI("VIS_EFFECT_NAME");
	switch (effectNo){
		case 0:
			break;			
			
		case 1:
			break;

		case 2:			
			break;
			
		case 3:
			thickness = ofMap(level, 0.0, 1.0, 1, 20);
			break;
			
		case 4:
			thickness = panel.getValueI("VIS_THICKNESS");
			break;
			
		default:
			break;
	}		
}

void SinglePeakPixelFactory::draw(){	
	ofPushStyle();
	
	float gradientVal;
	
	
	int gradR, gradG, gradB;
	
	peakHeight = (int)ofMap(level, 0.0, 1.0, 0.0, (wHeight-1.0));
	
		//draw bg
	ofFill();
	
	ofSetColor(bgColor.x,	// red
			   bgColor.y,	// green
			   bgColor.z);	// blue
	ofRect(0,0,settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);
	
	gradR = fgColor.x;
	gradG = fgColor.y;
	gradB = fgColor.z;
	
	switch (effectNo){
		case 0: //nothing, just the colour
			ofSetColor(fgColor.x, 
					   fgColor.y, 
					   fgColor.z);
			
			ofRect(0,0,settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);			
			
			break;			
			
		case 1: //gradient
			for(int i = (wHeight - peakHeight); i < wHeight; i++){
				gradientVal = 255.0 - ofMap(i, wHeight-peakHeight, wHeight, 0.0, 255.0);
				ofSetColor(gradR, 
						   gradG, 
						   gradB,
						   gradientVal);
				ofLine(0, i, wWidth, i);
			}			
			break;
		case 2:
			ofSetColor(fgColor.x, 
					   fgColor.y, 
					   fgColor.z);
			
			ofRect(0, settings.OUTPUT_CROP_H - peakHeight,settings.OUTPUT_CROP_W,peakHeight);				
			break;
			
		case 3:
			ofSetColor(gradR, 
					   gradG, 
					   gradB);
			
			ofRect(0, settings.OUTPUT_CROP_H - peakHeight -(thickness/2), wWidth, thickness);				
			break;
			
		case 4:
			ofSetColor(gradR, 
					   gradG, 
					   gradB);
			
			ofRect(0, settings.OUTPUT_CROP_H - peakHeight -(thickness/2), wWidth, thickness);		
			
			break;
			
		default:
			ofSetColor(fgColor.x, 
					   fgColor.y, 
					   fgColor.z);
			
			ofRect(0, settings.OUTPUT_CROP_H - peakHeight -(thickness/2), wWidth, thickness);				
			break;
	}		
	
	ofPopStyle();	
	
	drawTrails();		
}


