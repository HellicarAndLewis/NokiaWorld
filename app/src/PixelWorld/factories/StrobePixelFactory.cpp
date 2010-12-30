/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "StrobePixelFactory.h"

void StrobePixelFactory::setup(int aChannelNumber){
	
	factoryName = "Strobe"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));	
	
	panel.setup("FACTORY "+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 300, 350);
	panel.addPanel("visuals", 1, false);
	panel.setWhichPanel("visuals");
	
	PixelFactory::addPixelFactoryStandardGUIElements();
	
	strobeDelay = 100;
	timeToStrobe = ofGetSystemTime() + strobeDelay;
	fadePCT = (timeToStrobe - ofGetSystemTime())/(strobeDelay*1.0);
	
	maxChangeTime = 1500;
	minChangeTime = 33;
	defaultChangeTime = 100;
	
	bFiring = false;
	bPeakCycleStarted = false;
	
	oldStrobeIn = 100;
	
	panel.addToggle("Manual Control", "VIS_MANUAL", false);
	panel.addSlider("Change Freq", "VIS_SPEED", defaultChangeTime, minChangeTime, maxChangeTime, true);
	
	panel.addToggle("Fire On Peak", "VIS_BFIREPEAK", false);
	
	panel.addToggle("Strobe From White", "VIS_bSTROBEWHITE", false);
	
	panel.loadSettings(factoryName + "Settings.xml");
}

void StrobePixelFactory::update(){
	panel.update();

	int strobeIn;
	float peakThreshold;
	
	if(panel.getValueB("VIS_bSTROBEWHITE")) {
		strobeR = 255;
		strobeG = 255;
		strobeB = 255;
	}
	else {
		strobeR = fgColor.x;
		strobeG = fgColor.y;
		strobeB = fgColor.z;
	}
	
	bool bManual = panel.getValueB("VIS_MANUAL");
	bool bFireOnPeak = panel.getValueB("VIS_BFIREPEAK");
	
	if(bFireOnPeak){
		if(!bPeakCycleStarted){
			bPeakCycleStarted = true;
			bFiring = true;
		}
	}
	else {
		bPeakCycleStarted = false;
		bFiring = false;
	}
	
	
	strobeIn = (maxChangeTime + minChangeTime) - panel.getValueI("VIS_SPEED");
	peakThreshold = peakLevel; //from the parent class now, not the panel itself in the diesel version, this is nokia...
	
	if(bManual){//user driven
		
		if(bFireOnPeak){
			if(!bFiring){
				if(level > peakThreshold){
					strobeDelay = strobeIn;
					timeToStrobe = ofGetSystemTime() + strobeDelay;
					fadePCT = (timeToStrobe - ofGetSystemTime())/(strobeDelay*1.0);
					bFiring = true;
						//cout << "level: " << level << " peakthreshold: " << peakThreshold << " firing: " << bFiring  << endl;
				}				
			}
			else { //bFiring
				if(timeToStrobe >= ofGetSystemTime()){
					fadePCT = (timeToStrobe - ofGetSystemTime())/(strobeDelay*1.0);
				}
				else {
					fadePCT = 0;
					bFiring = false;
				}
			}
			
		}
		
		else {
			if(strobeIn >= maxChangeTime)
				fadePCT = 0;
			else {
				strobeDelay = strobeIn;
				fadePCT = (timeToStrobe - ofGetSystemTime())/(strobeDelay*1.0);
			}	
			if(ofGetSystemTime() >= timeToStrobe){
				timeToStrobe = ofGetSystemTime() + strobeDelay;
			}
		}
	}
	else{ //level driven
		strobeDelay = (int)ofMap(1.0 - level, 0.0, 1.0, 300, 1000);
		fadePCT = (timeToStrobe - ofGetSystemTime())/(strobeDelay*1.0);	
		
		if(ofGetSystemTime() >= timeToStrobe){
			timeToStrobe = ofGetSystemTime() + strobeDelay;
		}
		
	}	
}

void StrobePixelFactory::draw(){	
	ofPushStyle();
	
	ofFill();
	ofSetColor(strobeR*fadePCT, strobeG*fadePCT, strobeB*fadePCT);
	ofRect(0,0,settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);	

	ofPopStyle();	
	
	drawTrails();		
}


