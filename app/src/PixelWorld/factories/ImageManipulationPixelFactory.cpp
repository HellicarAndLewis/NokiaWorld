/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "ImageManipulationPixelFactory.h"

void ImageManipulationPixelFactory::setup(int aChannelNumber){
		//for manipulator
	
	width = settings.OUTPUT_CROP_W;
	height = settings.OUTPUT_CROP_H;
	
	pixels = new unsigned char [width * height * 3];
	pixelsAltered	= new unsigned char [width * height * 3];
	texture.allocate(width, height, GL_RGB);
	
	effectNames.push_back("blobber");
	effectNames.push_back("randomizer");
	effectNames.push_back("threshold");
	effectNames.push_back("TVsnow");	
	
	images.resize(0);
	imageIndexToUse = 0;
	
	factoryName = "ImageManipulationPixelFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));		
	
	panel.setup("FACTORY"+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 500, 400);
	panel.addPanel("visuals", 2, false);
	
	panel.setWhichPanel("visuals");

	PixelFactory::addPixelFactoryStandardGUIElements();
	
	panel.addMultiToggle("Effect", "VIS_EFFECT_NAME", 0, effectNames);	
	panel.addToggle("Manual Control of Effect?", "MANUAL_EFFECT", false);
	panel.addSlider("Manual Effect Level", "MANUAL_EFFECT_LEVEL", 0.f, 0.f, 1.f, false);
	
	vector <string> drawModeNames;
	drawModeNames.push_back("select on level");
	drawModeNames.push_back("next image on peak");
	drawModeNames.push_back("next image unless peak");
	drawModeNames.push_back("next image on space");
	drawModeNames.push_back("manual select");	
	panel.addTextDropDown("draw mode", "DRAW_MODE", 0, drawModeNames);	
	panel.addSlider("Manual Select", "MANUAL", 0.f, 0.f, 1.f, false);
	
	panel.setWhichColumn(1);
	
	lister.listDir("images/");
	panel.addFileLister("Images", &lister, 200, 300);
	
	panel.loadSettings(factoryName + "Settings.xml");
	
	panel.setupEvents();
	panel.enableEvents();
	
	ofAddListener(panel.guiEvent, this, &ImageManipulationPixelFactory::eventsIn);	
}

	//this captures all our control panel events - unless its setup differently in testApp::setup
		//--------------------------------------------------------------
void ImageManipulationPixelFactory::eventsIn(guiCallbackData & data){
	
	if(data.getDisplayName() == "Images"){
		std::ostringstream localOSS;
		
		string contentDirectoryName = data.getString(2).c_str();
		
		localOSS << "images/" << contentDirectoryName;
		
		string directoryToList = localOSS.str();		
		
		int numberOfImages = content.listDir(directoryToList);
		
		images.resize(numberOfImages); //resize the vector, maybe back to zero
		
		if(images.size() > 0){
			for(int i=0; i < images.size(); i++){
				std::ostringstream contentPathOSS;
				
				contentPathOSS << directoryToList << "/" << content.entries[i].filename;
				
				string contentPath = contentPathOSS.str();
				
				images[i].loadImage(contentPath);
			}
		}
		
		imageIndexToUse = 0;
		
		inited = true;
	}
}

void ImageManipulationPixelFactory::update(){
	panel.update();
	
	int drawMode = panel.getValueI("DRAW_MODE");
	
	if(drawMode == 0){
			//select on level
		imageIndexToUse = level*(images.size()-1);
	}else if(drawMode == 1){
			//next image on peak
		if(level > peakLevel){
			imageIndexToUse++;
		} 
		
		if (imageIndexToUse > (images.size()-1)) {
			imageIndexToUse = 0;
		}
	}else if(drawMode == 2){
			//next image unless peak
		if(!(level > peakLevel)){
			imageIndexToUse++;
		}
		
		if (imageIndexToUse > (images.size()-1)) {
			imageIndexToUse = 0;
		}
	}else if (drawMode == 3) {
			//next image on space
		if(spaceBar){
			imageIndexToUse++;
		}
		
		if (imageIndexToUse > (images.size()-1)) {
			imageIndexToUse = 0;
		}		
	}else if (drawMode == 4) {
			//next image on manual
		imageIndexToUse = panel.getValueF("MANUAL")*(images.size()-1);
	}
	
	if(inited)
		pixelsFromImage = images[imageIndexToUse].getPixels();

	effectNo = panel.getValueI("VIS_EFFECT_NAME");
}

void ImageManipulationPixelFactory::draw(){		
	if(inited){
		ofPushStyle();
		
		ofFill();
		ofSetColor(255, 255, 255);	

		switch (effectNo){
			case 0:{
				blobber();
				break;
			}
			case 1:{
				randomizer();
				break;
			}
			case 2:{
				thresholdImage();
				break;
			}
			case 3:{
				TVsnow();
				break;
			}
			default:
				break;
		}
		
		
		ofPopStyle();	
		
		drawTrails();			
	}
}

	// Image manipulation updaters
void ImageManipulationPixelFactory::blobber(){
	ofSetColor(255, 255, 255);
	
	float levelToUse;
	
	levelToUse = level;
	
	if(panel.getValueB("MANUAL_EFFECT")){
		levelToUse = panel.getValueF("MANUAL_EFFECT_LEVEL");
	}
	   
		// copy the pixels in and upload
	for (int i = 0; i < width; i+=5){
		for (int j = 0; j < height; j+=5){
			
			int pixIndex = (j * width + i)*3;
			int pixVal = (pixelsFromImage[pixIndex + 0] + pixelsFromImage[pixIndex + 1] + pixelsFromImage[pixIndex + 2])/3;
			float pct = (float)pixVal*(levelToUse+0.1) / 255.0f;
			ofSetColor(pixelsFromImage[pixIndex + 0], pixelsFromImage[pixIndex + 1], pixelsFromImage[pixIndex + 2]);
			
			float circleRadius = 20*pct;
			
			ofCircle(i, j, circleRadius);
		}
	}
}

void ImageManipulationPixelFactory::randomizer(){
	int randomRange;
	
	float levelToUse;
	
	levelToUse = level;
	
	if(panel.getValueB("MANUAL_EFFECT")){
		levelToUse = panel.getValueF("MANUAL_EFFECT_LEVEL");
	}	
	
	randomRange = ofMap(levelToUse, 0.0, 1.0, 0.0, 80);
	
		// copy the pixels in and upload
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			int pixIndex = (j * width + i)*3;
			int pixSource = ((j + (int)ofRandom(-randomRange, randomRange)) * width + (i +  (int)ofRandom(-randomRange, randomRange)))*3;
			
			if (pixSource < 0) pixSource = 0;
			if (pixSource >= width*height*3) pixSource = (width*height*3)-3;
			
			pixels[pixIndex + 0] = pixelsFromImage[pixSource + 0]; //red 
			pixels[pixIndex + 1] = pixelsFromImage[pixSource + 1]; //green 
			pixels[pixIndex + 2] = pixelsFromImage[pixSource + 2]; //blue 
		}
	}
	
	texture.loadData(pixels, width, height, GL_RGB);
	
	texture.draw(0,0);
}

void ImageManipulationPixelFactory::thresholdImage(){
	float levelToUse;
	
	levelToUse = level;
	
	if(panel.getValueB("MANUAL_EFFECT")){
		levelToUse = panel.getValueF("MANUAL_EFFECT_LEVEL");
	}		
	
	int threshold;
	
	threshold = ofMap(levelToUse, 0.0, 1.0, 0.0, 255.0);
	
	for(unsigned long i = 0; i < width * height * 3; i++)
		(pixelsFromImage[i] > threshold) ? pixels[i] = 255 : pixels[i] = 0; 
	
	texture.loadData(pixels, width, height, GL_RGB);
	
	texture.draw(0,0);	
}

void ImageManipulationPixelFactory::TVsnow(){
	float levelToUse;
	
	levelToUse = level;
	
	if(panel.getValueB("MANUAL_EFFECT")){
		levelToUse = panel.getValueF("MANUAL_EFFECT_LEVEL");
	}	
	
	for (int i = 0; i < width * height; i++){
		pixels[i] = (int)ofRandom(0,255*levelToUse);
	}
	   
	texture.loadData(pixels, width, height, GL_LUMINANCE);
	
	texture.draw(0,0);	
}
	   

