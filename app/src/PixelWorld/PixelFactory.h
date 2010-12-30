/*
 *  PixelFactory.h
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

/*
 *  based on baseScene.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "AppSettings.h"

class PixelFactory{
	
public: 
	
	PixelFactory(){		
		factoryName = "unnamed";  
		
		meAsTexture.allocate(settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H, GL_RGB); //just to start with
		
		level = 0.f;
		peakLevel = 0.f;
		spaceBar = false;
	}
	
	virtual void setup(int aChannelNumber){}
	virtual void update(){}
	virtual void draw(){}
	
	void mouseDragged( int x, int y, int button ){
		panel.mouseDragged(x, y, button);
	}
	
	void mousePressed( int x, int y, int button ){
		panel.mousePressed(x, y, button);
	}
	void mouseReleased(){
		panel.mouseReleased();
	}
	
	void drawGUI(){
		panel.draw();
	}
	
	void updatePanel(){
	}
	
	void readBackIntoTexture(){	
		ofPushStyle();
		ofEnableAlphaBlending();		
		
		meAsTexture.loadScreenData(0,0,settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);
		
		ofSetColor(0, 0, 0, 255); //black is zeros wolly
		ofFill();
		ofRect(0, 0, settings.OUTPUT_CROP_W, settings.OUTPUT_CROP_H);	
		
		ofDisableAlphaBlending();
		ofPopStyle();		
	}
	
	virtual void drawViaTexture(float alpha, ofRectangle pos){
//		ofPushStyle();
//		ofEnableAlphaBlending();
//			
//		ofSetColor(255, 255, 255, (int)(255.f*alpha));
//		
//		meAsTexture.draw(pos.x, pos.y, pos.width, pos.height);
//		
//		ofDisableAlphaBlending();
//		ofPopStyle();
		
		float imageScale = 1.f;
		
		if(panel.getValueB("AUDIO_SCALE_ON_LEVEL")){
			imageScale = level;
		}
		
		if(panel.getValueB("AUDIO_BRIGHTNESS_ON_LEVEL")){
			alpha *= level;  //scale the alpha by the audio level
		}
		
		ofRectangle newPosition;
				
		newPosition.width = pos.width * imageScale;
		newPosition.height = pos.height * imageScale;
		
		newPosition.x = pos.x + (((1.f-imageScale)*pos.width)/2.f);
		newPosition.y = pos.y + (((1.f-imageScale)*pos.height)/2.f);
		
		ofPushStyle();
		ofEnableAlphaBlending();
		ofTranslate(newPosition.x, newPosition.y, 0);
		
		ofSetColor(255, 255, 255, (int)(255.f*alpha));
		
		meAsTexture.draw(0, 0, newPosition.width, newPosition.height);	
		
		ofTranslate(-newPosition.x, -newPosition.y, 0);
		ofDisableAlphaBlending();
		ofPopStyle();		
	}
	
	void setLevelLevelPeakSpace(float _level, float  _levelPeak, bool _space,  vector <float> _fftLevels){
		level = _level;
		peakLevel = _levelPeak;
		spaceBar = _space;
		fftLevels.resize(_fftLevels.size());
		fftLevels =  _fftLevels;
	}
	
	void addPixelFactoryStandardGUIElements(){
		panel.addToggle("Brightness on Audio?", "AUDIO_BRIGHTNESS_ON_LEVEL", 0);
		panel.addToggle("Scale on Audio?", "AUDIO_SCALE_ON_LEVEL", 0);
		panel.addToggle("trails", "VIS_TRAILS", false);		
		panel.addDrawableRect("Image", &meAsTexture, settings.OUTPUT_CROP_W/4, settings.OUTPUT_CROP_H/4);			
	}
	
	void drawTrails(){
		bool bTrails = panel.getValueB("VIS_TRAILS"); 
		
		if(bTrails){
			ofRectangle scaledOutput;
			
			scaledOutput.x = -5;
			scaledOutput.y = -5;
			scaledOutput.width = settings.OUTPUT_CROP_W+10;
			scaledOutput.height = settings.OUTPUT_CROP_H+10;
			
			drawViaTexture(220.f/255.f, scaledOutput);			
		}
	}	

	ofTexture			meAsTexture;
	string				factoryName;
	ofxControlPanel		panel;
	ofPoint				fgColor;
	ofPoint				bgColor;
	float	level;	// single value, 0-1 ?
	float	peakLevel;
	bool	spaceBar;
	vector <float> fftLevels;
};