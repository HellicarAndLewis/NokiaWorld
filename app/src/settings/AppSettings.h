/*
 *  AppGlobals.h
 *  emptyExample
 *
 *  Created by Todd Vanderlin on 3/30/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofxXmlSettings.h"

class AppSettings {

protected:
	ofxXmlSettings xml;
public:
	
	int GUI_W;
	int GUI_H;
	int PANEL_W;
	int PANEL_H;
	int OUTPUT_OVERALL_W;
	int OUTPUT_OVERALL_H;
	int OUTPUT_CROP_W;
	int OUTPUT_CROP_H;

	AppSettings() {
	}
	
	void loadSettings(string file) {
		xml.loadFile(file);
		
		GUI_W = xml.getValue("GUI_W", 1440);
		GUI_H = xml.getValue("GUI_H", 900);
		PANEL_W = xml.getValue("PANEL_W", 640);
		PANEL_H = xml.getValue("PANEL_H", 768);
		OUTPUT_OVERALL_W = xml.getValue("OUTPUT_OVERALL_W", 1280);
		OUTPUT_OVERALL_W = xml.getValue("OUTPUT_OVERALL_W", 720);
		OUTPUT_CROP_W = xml.getValue("OUTPUT_CROP_W", 960);
		OUTPUT_CROP_H = xml.getValue("OUTPUT_CROP_H", 64);		
	}
};


// so other apps can see it...
extern AppSettings settings;
