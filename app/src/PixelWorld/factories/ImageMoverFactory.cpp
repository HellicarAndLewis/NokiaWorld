/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "ImageMoverFactory.h"

void ImageMoverFactory::setup(int aChannelNumber){
	
	factoryName = "ImageMoverFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));		
	
	panel.setup("FACTORY"+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 500, 400);
	panel.addPanel("visuals", 2, false);
	
	panel.setWhichPanel("visuals");

	PixelFactory::addPixelFactoryStandardGUIElements();
	vector <string> drawModeNames;
	drawModeNames.push_back("vertical");
	drawModeNames.push_back("horizontal");
	drawModeNames.push_back("vertical invert");
	drawModeNames.push_back("horizontal invert");
	panel.addTextDropDown("Draw Mode", "DRAW_MODE", 0, drawModeNames);	
	
	panel.setWhichColumn(1);
	
	lister.listDir("imagesToSlitAndStretch/");
	panel.addFileLister("Images", &lister, 200, 300);
	
	panel.loadSettings(factoryName + "Settings.xml");
	
	panel.setupEvents();
	panel.enableEvents();
	
	ofAddListener(panel.guiEvent, this, &ImageMoverFactory::eventsIn);	
	
	inited = false;
	
	textureToSlitStretchInto.allocate(settings.OUTPUT_CROP_W, settings.OUTPUT_CROP_H, GL_RGB);
	pixels = new unsigned char [settings.OUTPUT_CROP_W * settings.OUTPUT_CROP_H * 3];
	
	currentContentPath = "";
	currentImageMode = 0;	
}

	//this captures all our control panel events - unless its setup differently in testApp::setup
		//--------------------------------------------------------------
void ImageMoverFactory::eventsIn(guiCallbackData & data){
	
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
	
	if(data.getDisplayName() == "Draw Mode"){
		if(inited){
			int newMode = data.getInt(0);
			/*
			 drawModeNames.push_back("vertical");
			 drawModeNames.push_back("horizontal");
			 drawModeNames.push_back("vertical invert");
			 drawModeNames.push_back("horizontal invert");
			 */
			
			currentImageMode = newMode;
			
			doImageResize();			
		}
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

void ImageMoverFactory::doImageResize(){
	if(inited){
		image.loadImage(currentContentPath);
		
		float resizeRatio = 1.f;
		float currentHeight = image.height;
		float currentWidth = image.width;
		
//		cout << "Starting with: " << currentHeight << " x " << currentWidth << endl;		
		
		switch (currentImageMode){
			case 0:{ //vertical
				resizeRatio = (float)settings.OUTPUT_CROP_W/currentWidth;
				break;
			}
			case 1:{ //horizontal
				resizeRatio = (float)settings.OUTPUT_CROP_H/currentHeight;
				break;
			}
			case 2:{ //vertical invert
				resizeRatio = (float)settings.OUTPUT_CROP_W/currentWidth;
				break;
			}
			case 3:{ //horizontal invert
				resizeRatio = (float)settings.OUTPUT_CROP_H/currentHeight;			
				break;
			}
			default:{
				resizeRatio = (float)settings.OUTPUT_CROP_W/currentWidth;
				break;			
			}
		}
		
		image.resize(currentWidth * resizeRatio, currentHeight * resizeRatio);
		
//		cout << "Resizing to: " << currentWidth * resizeRatio << " x " << currentHeight * resizeRatio << endl;
	}
}

void ImageMoverFactory::update(){
	panel.update();
	
	if(inited){
		int LEDwidth = settings.OUTPUT_CROP_W;
		int LEDheight = settings.OUTPUT_CROP_H;
		
		unsigned char * pixelsFromImage = image.getPixels();
		
		switch (currentImageMode){
			case 0:{ //vertical
				int startHeight = level*image.height;
				
				if(startHeight > (image.height - LEDheight)){ //if the peak is so high that it goes closer than the led height
					startHeight = image.height - LEDheight; //so it's safe
				}
				
				int startPos = startHeight*image.width*3;
				int endPos = startPos + (LEDheight*image.width*3);
				
				int pixIndex = 0;
				
				for(int i = startPos; i < endPos; i++){
					pixels[pixIndex] = pixelsFromImage[i];
					pixIndex++;
				}
				
				break;
			}
			case 1:{ //horizontal
				int startWidth = level*image.width;
				
				if(startWidth > (image.width - LEDwidth)){
					startWidth = image.width - LEDwidth; //so it's safe
				}
				
				int startPos = startWidth;
				int endPos = startPos + LEDwidth;
				
				int pixIndex = 0;
				
				for(int j = 0; j < LEDheight; j++){
					for(int i = startPos; i < endPos; i++){
						int takeFrom = ((j*image.width)+i)*3;
						
						pixels[pixIndex] = pixelsFromImage[takeFrom];
						pixels[pixIndex+1] = pixelsFromImage[takeFrom+1];
						pixels[pixIndex+2] = pixelsFromImage[takeFrom+2];	
						pixIndex += 3;
					}
				}
				
				break;
			}
			case 2:{ //vertical invert
				int startHeight = (1-level)*image.height;
				
				if(startHeight > (image.height - LEDheight)){ //if the peak is so high that it goes closer than the led height
					startHeight = image.height - LEDheight; //so it's safe
				}
				
				int startPos = startHeight*image.width*3;
				int endPos = startPos + (LEDheight*image.width*3);
				
				int pixIndex = 0;
				
				for(int i = startPos; i < endPos; i++){
					pixels[pixIndex] = pixelsFromImage[i];
					pixIndex++;
				}
				
				break;
			}
			case 3:{ //horizontal invert
				int startWidth = (1.f-level)*image.width;
				
				if(startWidth > (image.width - LEDwidth)){
					startWidth = image.width - LEDwidth; //so it's safe
				}
				
				int startPos = startWidth;
				int endPos = startPos + LEDwidth;
				
				int pixIndex = 0;
				
				for(int j = 0; j < LEDheight; j++){
					for(int i = startPos; i < endPos; i++){
						int takeFrom = ((j*image.width)+i)*3;
						
						pixels[pixIndex] = pixelsFromImage[takeFrom];
						pixels[pixIndex+1] = pixelsFromImage[takeFrom+1];
						pixels[pixIndex+2] = pixelsFromImage[takeFrom+2];	
						pixIndex += 3;
					}
				}		
				break;
			}
		}
		
		textureToSlitStretchInto.loadData(pixels, LEDwidth, LEDheight, GL_RGB);	
	}	
}

void ImageMoverFactory::draw(){		
	if(inited){
		ofPushStyle();
		
		ofFill();
		ofSetColor(255, 255, 255);
		textureToSlitStretchInto.draw(0,0);	
		
		ofPopStyle();	
		
		drawTrails();			
	}
}


