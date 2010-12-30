/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "StringPixelFactory.h"

void StringPixelFactory::setup(int aChannelNumber){
	
	factoryName = "StringPixelFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));	
	
	panel.setup("FACTORY "+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 300, 400);
	panel.addPanel("visuals", 1, false);
	panel.setWhichPanel("visuals");
	
	PixelFactory::addPixelFactoryStandardGUIElements();
	
	panel.addSlider("level input", "VIS_LEVEL_INPUT", 0.3, 0, 1.0f, false);
	panel.addSlider("level dropoff", "VIS_LEVEL_DROPOFF", 0.8, 0, 1, false);
	panel.addToggle("colored bg", "VIS_COLORED_BACKGROUND", false);
	panel.addToggle("invert bg/fg", "VIS_INVERT", false);
	
	panel.loadSettings(factoryName + "Settings.xml");
	
	for (int i = 0; i < 40; i++){
		particle myParticle;
		myParticle.setInitialCondition(settings.OUTPUT_CROP_W/40 * i, settings.OUTPUT_CROP_H*0.75f,0,0);
		particles.push_back(myParticle);
		
		if (i == 0 || i == 39){
			particles[i].bFixed = true;
			if (i == 0) particles[i].pos.set(0,settings.OUTPUT_CROP_H);
			if (i == 39) particles[i].pos.set(settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);
		}
	}
	
		// change this to
		// for (int i = 0; i < 10; i++){
		// to see a chain
	
	for (int i = 0; i < particles.size()-1; i++){
		spring mySpring;
		mySpring.distance		= 30;
		mySpring.springiness	= 0.2f;
		mySpring.particleA = & (particles[i  ]);
		mySpring.particleB = & (particles[(i+1) % particles.size()]);
		springs.push_back(mySpring);
	}
	
	happiness = 0;	
}

void StringPixelFactory::update(){
	panel.update();
	
	float input = panel.getValueF("VIS_LEVEL_INPUT");
	float drop = panel.getValueF("VIS_LEVEL_DROPOFF");
	
	happiness += level * input;	
	happiness *= drop;
	
	if (ofRandom(0,1) > 0.99f){
		ptsForRepulsion.clear();
		int nNewPts = (int)ofRandom(2, 6);
		for (int i = 0; i < nNewPts; i++){
			ptsForRepulsion.push_back(ofPoint( ofRandom(80, settings.OUTPUT_CROP_W-80), ofRandom(settings.OUTPUT_CROP_H-600, settings.OUTPUT_CROP_H-100), 0));
		}
	}
	
		// on every frame 
		// we reset the forces
		// add in any forces on the particle
		// perfom damping and
		// then update
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}
	
	for (int i = 0; i < particles.size(); i++){
		
	    for (int j = 0; j < ptsForRepulsion.size(); j++){
			particles[i].addRepulsionForce(ptsForRepulsion[j].x, ptsForRepulsion[j].y, 300, 0.7f * happiness);
		}
		
		for (int j = 0; j < i; j++){
			particles[i].addRepulsionForce(particles[j],50 +80 * happiness, 0.05 + 0.3 * happiness);
		}
		
		particles[i].addForce(0,0.4f + happiness * -0.6f);
	}
	
	for (int i = 0; i < springs.size(); i++){
		springs[i].update();
	}
	
	
	for (int i = 0; i < particles.size(); i++){
		
		
		particles[i].bounceOffWalls();
		particles[i].addDampingForce();
		particles[i].update();
	}		
}

void StringPixelFactory::draw(){	
	ofPushStyle();

	bool bActive = panel.getValueB("VIS_COLORED_BACKGROUND");
	bool bInvert = panel.getValueB("VIS_INVERT");
	bool bTrails = panel.getValueB("VIS_TRAILS"); 
	
	if (bActive){
		
		if (bInvert) {	
			ofSetColor(fgColor.x, fgColor.y, fgColor.z, bTrails ? 100 : 255);
		} else {
			ofSetColor(bgColor.x, bgColor.y, bgColor.z, bTrails ? 100 : 255);
		}
		ofRect(0,0,settings.OUTPUT_CROP_W, settings.OUTPUT_CROP_H);
	}
	
	glPushMatrix();
	
	ofSetColor(0xffffff);
	
	if (bInvert) {	
		ofSetColor(bgColor.x, bgColor.y, bgColor.z, bTrails ? 190 : 255);
	} else {
		ofSetColor(fgColor.x, fgColor.y, fgColor.z, bTrails ? 190 : 255);
	}
	
	ofFill();
	ofBeginShape();
	ofVertex(particles[0].pos.x, particles[0].pos.y);
	for (int i = 0; i < particles.size(); i++){
			//particles[i].draw();
		ofCurveVertex(particles[i].pos.x, particles[i].pos.y);
	}
	ofCurveVertex(particles[particles.size()-1].pos.x, particles[particles.size()-1].pos.y);
	ofEndShape(true);
	
	glPopMatrix();
	
	ofPopStyle();	
	
	drawTrails();	
}


