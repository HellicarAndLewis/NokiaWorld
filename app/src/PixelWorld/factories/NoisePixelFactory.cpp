/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "NoisePixelFactory.h"

float cloudfilter(float val) {
	return (1-pow(0.9f,MAX(val-0.55f,0)*255));
}

void NoisePixelFactory::setup(int aChannelNumber){
	
	noise = new ofxPerlin();
	
	w = settings.OUTPUT_CROP_W/4;
	h = settings.OUTPUT_CROP_H/4;
	
	texColour.allocate(w,h,GL_RGB);
	
	colourPixels = new unsigned char [w*h*3];	
	
	factoryName = "NoisePixelFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));	
	
	panel.setup("FACTORY "+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 500,400);
	panel.addPanel("visuals", 2, false);
	panel.setWhichPanel("visuals");
	
	PixelFactory::addPixelFactoryStandardGUIElements();
	
	panel.addToggle("monochrome?", "NOISE_MONOCHROME", true);
	panel.addSlider("red offset", "NOISE_RED_OFFSET", 1, 0.0, 20.0, false);	
	panel.addSlider("green offset", "NOISE_GREEN_OFFSET", 1, 0.0, 20.0, false);	
	panel.addSlider("blue offset", "NOISE_BLUE_OFFSET", 1, 0.0, 20.0, false);
	
	panel.setWhichColumn(1);
	
	panel.addSlider("ns", "NOISE_NOISE_SCALAR", 0.01, 0.0, 0.2, false);
	panel.addSlider("number of octaves", "NOISE_NUMBER_OF_OCTAVES", 8, 1,20, true);
	panel.addSlider("falloff", "NOISE_FALLOFF", 0.5, 0.0, 1.f, false);	
	
	panel.loadSettings(factoryName + "Settings.xml");
}

void NoisePixelFactory::update(){
	panel.update();
	
	int		numberOfOctaves;
	float	fallOff;
	
		//noise->noiseDetail(8,0.5);
	numberOfOctaves = panel.getValueI("NOISE_NUMBER_OF_OCTAVES");
	fallOff = panel.getValueF("NOISE_FALLOFF");
	
	noise->noiseDetail(numberOfOctaves,fallOff);
	
		//karsten help for workshop
	
	float red,green,blue;
	
		//	red = 100;
		//	green = 200;
		//	blue = 300;
	
	red = panel.getValueF("NOISE_RED_OFFSET");
	green = panel.getValueF("NOISE_GREEN_OFFSET");
	blue = panel.getValueF("NOISE_BLUE_OFFSET");	
	
	float ns = panel.getValueF("NOISE_NOISE_SCALAR"); //0.01f;
	float z=ofGetElapsedTimef()/10.f;
	
	if(panel.getValueB("NOISE_MONOCHROME")){
		for (int i = 0; i < w; i++){
			for (int j = 0; j < h; j++){ //how to colourise!!!!!! interactive this up...		
				colourPixels[(j*w+i)*3 + 0] = noise->noiseuf(i*ns+red,j*ns,z)*255;	// r
				colourPixels[(j*w+i)*3 + 1] = noise->noiseuf(i*ns+red,j*ns,z)*255;//noise->noiseuf(i*ns+green,j*ns,z)*255;
				colourPixels[(j*w+i)*3 + 2] = noise->noiseuf(i*ns+red,j*ns,z)*255;//noise->noiseuf(i*ns+blue,j*ns,z)*255;
			}
		}
	}else{
		for (int i = 0; i < w; i++){
			for (int j = 0; j < h; j++){
				colourPixels[(j*w+i)*3 + 0] = noise->noiseuf(i*ns+red,j*ns,z)*255;	// r
				colourPixels[(j*w+i)*3 + 1] = noise->noiseuf(i*ns+green,j*ns,z)*255;
				colourPixels[(j*w+i)*3 + 2] = noise->noiseuf(i*ns+blue,j*ns,z)*255;
			}
		}		
	}
	
	texColour.loadData(colourPixels, w,h, GL_RGB);
}

void NoisePixelFactory::draw(){	
	ofPushStyle();
	ofSetColor(fgColor.x, fgColor.y, fgColor.z);
	texColour.draw(0,0,settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);
	ofPopStyle();	
	
	drawTrails();		
}


