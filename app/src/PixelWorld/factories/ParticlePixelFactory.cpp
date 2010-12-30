/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "ParticlePixelFactory.h"

bool deadParticle( CustomParticle &x )
{
	return x.dead;
}

void ParticlePixelFactory::setup(int aChannelNumber){
	
	factoryName = "Particle "+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));	
	
	panel.setup("FACTORY "+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 500, 500);
	panel.addPanel("visuals", 2, false);
	panel.setWhichPanel("visuals");
	
	PixelFactory::addPixelFactoryStandardGUIElements();
	
	panel.addSlider2D("emitter point", "PARTICLE_EMITTER", settings.OUTPUT_CROP_W/2, settings.OUTPUT_CROP_H/2, 0, settings.OUTPUT_CROP_W, 0, settings.OUTPUT_CROP_H, false);
	
	panel.setWhichColumn(1);
	
	panel.addSlider("min radius", "PARTICLE_MINR", 8, 4, 15, false);
	panel.addSlider("max radius", "PARTICLE_MAXR", 20, 8, 50, false);
	
	panel.addToggle("colored bg", "VIS_COLORED_BACKGROUND", false);
	panel.addToggle("invert bg/fg", "VIS_INVERT", false);	
	
	panel.addSlider2D("gravity", "PARTICLE_GRAVITY", 0,1, -1,1,-1,1, false);	
	
	panel.loadSettings(factoryName + "Settings.xml");
	
	bDrawLines  = false;
	bMouseForce = false;
	
	box2d.init();
	box2d.setGravity(0, -10); //TODO: change the floor setting so it's right...
		//box2d.createFloor();  ///no floor for now, so they can all appear
	box2d.checkBounds(true);
	box2d.setFPS(30.0);	
}

void ParticlePixelFactory::update(){
	float x = panel.getValueF("PARTICLE_EMITTER", 0);
	float y = panel.getValueF("PARTICLE_EMITTER", 1);
	
	float gx = panel.getValueF("PARTICLE_GRAVITY", 0);
	float gy = panel.getValueF("PARTICLE_GRAVITY", 1);
	
	float minr = panel.getValueF("PARTICLE_MINR");
	float maxr = panel.getValueF("PARTICLE_MAXR");	
	
	box2d.setGravity(10*gx, 10 * gy);
	
	if (ofRandom(0,1) < level && ofGetFrameNum() % 3 == 0 && level > peakLevel){ //changed to respond to peak level
		float r = ofRandom(minr, maxr);		// a random radius 4px - 20px
		CustomParticle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		
		float myx = x + ofRandom( -100*level, 100*level);
		float myy = y + ofRandom( -100*level, 100*level);
		
		myx = MIN(MAX(myx, 20*2), settings.OUTPUT_CROP_W-20*2);
		myy = MIN(MAX(myy, 20*2), settings.OUTPUT_CROP_H-20*2);	
		
		circle.setup(box2d.getWorld(), myx, myy, r);
		
		bool bInvert = panel.getValueB("VIS_INVERT");
		if (!bInvert){
			circle.color.r = fgColor.x + ofRandom(-10,10);
			circle.color.g = fgColor.y;
			circle.color.b = fgColor.z + ofRandom(-10,10);
		} else {
			circle.color.r = bgColor.x + ofRandom(-10,10);
			circle.color.g = bgColor.y;
			circle.color.b = bgColor.z + ofRandom(-10,10);
		}
		b2FilterData data;
		data.maskBits =  0x0004 | 0x0003;
		data.groupIndex = 1;
		circle.setFilterData(data);	
		customParticles.push_back(circle);
		
	}

	
	for(int i=0; i<customParticles.size(); i++) {
		if (customParticles[i].getPosition().x > settings.OUTPUT_CROP_W + 80 || customParticles[i].getPosition().x < -80){
			customParticles[i].destroyShape();
		}
		
		if (customParticles[i].getPosition().y > settings.OUTPUT_CROP_H + 80 || customParticles[i].getPosition().y < -80){
			customParticles[i].destroyShape();
		}
		
	}
	
	customParticles.erase(remove_if( customParticles.begin(), customParticles.end(),   deadParticle), customParticles.end());
	
	panel.update();
	
	box2d.update();
}

void ParticlePixelFactory::draw(){	
	ofPushStyle();

	bool bActive = panel.getValueB("VIS_COLORED_BACKGROUND");
	bool bInvert = panel.getValueB("VIS_INVERT"); 
	
	if (bActive){
		
		if (bInvert) {	
			ofSetColor(fgColor.x, fgColor.y, fgColor.z);
		} else {
			ofSetColor(bgColor.x, bgColor.y, bgColor.z);
		}
		ofRect(0,0,settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);
	}
	
	glPushMatrix();
	
	for(int i=0; i<customParticles.size(); i++) {
		customParticles[i].draw();
	}
	
	glPopMatrix();	
	
	ofPopStyle();	
	
	drawTrails();		
}


