/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "ImagePixelFactory.h"

void ImagePixelFactory::setup(int aChannelNumber){
	
	images.resize(0);
	imageIndexToUse = 0;
	
	factoryName = "ImagePixelFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));		
	
	panel.setup("FACTORY"+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 500, 400);
	panel.addPanel("visuals", 2, false);
	
	panel.setWhichPanel("visuals");
	
	panel.setWhichPanel(0);
	panel.setWhichColumn(0);

	PixelFactory::addPixelFactoryStandardGUIElements();
	
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
	
	ofAddListener(panel.guiEvent, this, &ImagePixelFactory::eventsIn);	
}

	//this captures all our control panel events - unless its setup differently in testApp::setup
		//--------------------------------------------------------------
void ImagePixelFactory::eventsIn(guiCallbackData & data){
	
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

void ImagePixelFactory::update(){
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
	
}

void ImagePixelFactory::draw(){		
	if(inited){
		ofPushStyle();
		
		ofFill();
		ofSetColor(255, 255, 255);
		images[imageIndexToUse].draw(0, 0, settings.OUTPUT_CROP_W, settings.OUTPUT_CROP_H);
		
		ofPopStyle();	
		
		drawTrails();			
	}
}


