/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "SlitStretchFactory.h"

void SlitStretchFactory::setup(int aChannelNumber){
	
	factoryName = "SlitStretchFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));		
	
	panel.setup("FACTORY"+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 500, 400);
	panel.addPanel("visuals", 2, false);
	
	panel.setWhichPanel("visuals");

	PixelFactory::addPixelFactoryStandardGUIElements();

	panel.addToggle("Vertical? Else Horizontal", "SLIT_VERTICAL", true);
	
	panel.setWhichColumn(1);
	
	lister.listDir("imagesToSlitAndStretch/");
	panel.addFileLister("Images", &lister, 200, 300);
	
	panel.loadSettings(factoryName + "Settings.xml");
	
	panel.setupEvents();
	panel.enableEvents();
	
	ofAddListener(panel.guiEvent, this, &SlitStretchFactory::eventsIn);	
	
	inited = false;	
	
	textureToStretchVertically.allocate(settings.OUTPUT_CROP_W, 1, GL_RGB);
	textureToStretchHorizontally.allocate(1, settings.OUTPUT_CROP_H, GL_RGB);	

	pixelsHorizontalStretchVertically = new unsigned char [settings.OUTPUT_CROP_W * 3];
	pixelsVerticalStretchHorizontally = new unsigned char [settings.OUTPUT_CROP_H * 3];	
	
	currentContentPath = "";
	currentImageMode = 0;	
}

	//this captures all our control panel events - unless its setup differently in testApp::setup
		//--------------------------------------------------------------
void SlitStretchFactory::eventsIn(guiCallbackData & data){
	
	if(data.getDisplayName() == "Images"){
		inited = false;
		std::ostringstream localOSS;
		
		string contentFilename = data.getString(2).c_str();
		
		localOSS << "imagesToSlitAndStretch/" << contentFilename;
		
		string contentPath = localOSS.str();
		
		currentContentPath = contentPath;

		inited = true;		
		
		doImageResize();
	}
	
//		// print to terminal if you want to 
//		//this code prints out the name of the events coming in and all the variables passed
//	printf("testApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
//	if( data.getDisplayName() != "" ){
//		printf(" element name is %s \n", data.getDisplayName().c_str());
//	}
//	for(int k = 0; k < data.getNumValues(); k++){
//		if( data.getType(k) == CB_VALUE_FLOAT ){
//			printf("%i float  value = %f \n", k, data.getFloat(k));
//		}
//		else if( data.getType(k) == CB_VALUE_INT ){
//			printf("%i int    value = %i \n", k, data.getInt(k));
//		}
//		else if( data.getType(k) == CB_VALUE_STRING ){
//			printf("%i string value = %s \n", k, data.getString(k).c_str());
//		}
//	}
//	
//	printf("\n");	
}

void SlitStretchFactory::doImageResize(){
	if(inited){
		image.loadImage(currentContentPath);
		
		image.resize(settings.OUTPUT_CROP_W, settings.OUTPUT_CROP_H);
	}
}

void SlitStretchFactory::update(){
	panel.update();
	
	if(inited){
		int LEDwidth = settings.OUTPUT_CROP_W;
		int LEDheight = settings.OUTPUT_CROP_H;
		
		unsigned char * pixelsFromImage = image.getPixels();
		
		bool vertical = panel.getValueB("SLIT_VERTICAL");
		
		if(vertical){
				//make vertical bars, so take a horizontal strip out of the image
			int verticalPosition = (int)(level*(float)(LEDheight-1));
			int offsetToStartInSource = (verticalPosition*image.width);
			
			for(int i=0; i<LEDwidth; i++){
				int currentPos = i*3;
				int currentPosInSource = (offsetToStartInSource + i)*3;
				
				pixelsHorizontalStretchVertically[currentPos] = pixelsFromImage[currentPosInSource];
				pixelsHorizontalStretchVertically[currentPos+1] = pixelsFromImage[currentPosInSource+1];
				pixelsHorizontalStretchVertically[currentPos+2] = pixelsFromImage[currentPosInSource+2];				
			}
			
			textureToStretchVertically.loadData(pixelsHorizontalStretchVertically, LEDwidth, 1, GL_RGB);	
		}else{
				//make horizontal bars, so take a vertical strip out of the image
			int horizontalPosition = (int)(level*(float)(LEDwidth-1));
			
			for(int i=0; i<LEDheight; i++){
				int currentPos = i*3;
				int currentPosInSource = (i*LEDwidth + horizontalPosition)*3;
				pixelsVerticalStretchHorizontally[currentPos] = pixelsFromImage[currentPosInSource];
				pixelsVerticalStretchHorizontally[currentPos+1] = pixelsFromImage[currentPosInSource+1];
				pixelsVerticalStretchHorizontally[currentPos+2] = pixelsFromImage[currentPosInSource+2];				
			}
			
			textureToStretchHorizontally.loadData(pixelsVerticalStretchHorizontally, 1, LEDheight, GL_RGB);
		}
	}	
}

void SlitStretchFactory::draw(){		
	if(inited){
		ofPushStyle();
		
		ofFill();
		ofSetColor(255, 255, 255);
		
		bool vertical = panel.getValueB("SLIT_VERTICAL");
		
		if(vertical){
				//make vertical bars, so take a horizontal strip out of the image
			textureToStretchVertically.draw(0,0, settings.OUTPUT_CROP_W, settings.OUTPUT_CROP_H);
		}else{
				//make horizontal bars, so take a vertical strip out of the image
			textureToStretchHorizontally.draw(0,0, settings.OUTPUT_CROP_W, settings.OUTPUT_CROP_H);			
		}
		
		ofPopStyle();	
		
		drawTrails();			
	}
}


