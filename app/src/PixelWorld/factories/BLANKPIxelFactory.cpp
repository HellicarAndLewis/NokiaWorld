/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "ParticlePixelFactory.h"

void ParticlePixelFactory::setup(int aChannelNumber){
	
	factoryName = "ParticlePixelFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));	
	
	panel.setup("FACTORY "+factoryName, (int)ofRandom(10.f, 200.f), 600, settings.PANEL_W, settings.PANEL_H);
	panel.addPanel("visuals", 1, false);
	panel.setWhichPanel("visuals");
	
	PixelFactory::addPixelFactoryStandardGUIElements();

	
	
	panel.loadSettings(factoryName + "Settings.xml");
}

void ParticlePixelFactory::update(){
	panel.update();
}

void ParticlePixelFactory::draw(){	
	ofPushStyle();
	
	ofPopStyle();	
	
	drawTrails();		
}


