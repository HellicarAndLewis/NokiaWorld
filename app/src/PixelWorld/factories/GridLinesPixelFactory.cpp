/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "GridLinesPixelFactory.h"

void GridLinesPixelFactory::setup(int aChannelNumber){
	
	tWidth = settings.OUTPUT_CROP_W;
	tHeight = settings.OUTPUT_CROP_H;
	
	xDivisions = 8.0;
	yDivisions = 6.0;
	
	xSpacing = tWidth / xDivisions;
	ySpacing = tHeight / yDivisions;
	
	xRotation = 0.0;
	yRotation = 0.0;
	zRotation = 0.0;
	
	xLength = 0.0;
	yLength = 0.0;
	thickness = 1.0;
	
	filled = true;
	
	factoryName = "GridLinesPixelFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));	
	
	panel.setup("FACTORY "+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 500, 400);
	panel.addPanel("visuals", 2, false);
	panel.setWhichPanel("visuals");

	panel.setWhichColumn(0);
	
	PixelFactory::addPixelFactoryStandardGUIElements();
	
	panel.addSlider("horizontal segments", "VIS_XDIM", 8, 1, 16, true);
	panel.addToggle("horizonals visible", "VIS_BHOR", true);
	
	panel.addSlider("vertical segments", "VIS_YDIM", 6, 1, 12, true);
	panel.addToggle("verticals visible", "VIS_BVER", true);
	
	panel.setWhichColumn(1);
	
	panel.addToggle("manual thickness", "VIS_MANUALTHICK", false);
	panel.addSlider("thickness", "VIS_THICK", 10, 1, 20, true);
	panel.addToggle("filled", "VIS_FILL", true);
	
	panel.addToggle("manual v.offset", "VIS_BVOFFSET", true);
	panel.addSlider("vert offset", "VIS_VOFFSET", 0, -10, 10, true);
	panel.addToggle("manual h.offset", "VIS_BHOFFSET", true);
	panel.addSlider("horz offset", "VIS_HOFFSET", 0, -10, 10, true);	
	
	panel.loadSettings(factoryName + "Settings.xml");
}

void GridLinesPixelFactory::update(){
	panel.update();
}

void GridLinesPixelFactory::draw(){	
	ofPushStyle();
	
	int vOffset, hOffset;
	
	xDivisions = panel.getValueI("VIS_XDIM");
	xSpacing = tWidth / xDivisions;
	
	yDivisions = panel.getValueI("VIS_YDIM");
	ySpacing = tHeight / yDivisions;
	
	xLength = ofMap(level, 0.0, 1.0, 0.0, xSpacing);
	yLength = ofMap(level, 0.0, 1.0, 0.0, ySpacing);
	
	if(panel.getValueB("VIS_MANUALTHICK"))
		thickness = panel.getValueI("VIS_THICK");
	else
		thickness = yLength/5;
	
	if(!panel.getValueB("VIS_BHOR")) 
		xLength = 0;
	
	if(!panel.getValueB("VIS_BVER")) 
		yLength = 0;
	
	if(panel.getValueB("VIS_BVOFFSET"))
		vOffset = panel.getValueI("VIS_VOFFSET");
	else
		vOffset = (int)ofMap(level, 0.0, 1.0, -10.0, 10);
	
	if(panel.getValueB("VIS_BHOFFSET"))
		hOffset = panel.getValueI("VIS_HOFFSET");
	else
		hOffset = (int)ofMap(level, 0.0, 1.0, -10.0, 10);
	
	
	filled = panel.getValueB("VIS_FILL");
	
		//	fgColor.x
		//	fgColor.y
		//	fgColor.z
	ofFill();
		//	float red = panel.getValueF("COLORAB", 2); // 0 - 1 are x and y, 2,3,4 = RGB
	ofSetColor(bgColor.x,	// red
			   bgColor.y,	// green
			   bgColor.z);	// blue
	ofRect(0,0,settings.OUTPUT_CROP_W, settings.OUTPUT_CROP_H);
	
		//	ofSetColor(255, 255, 0);
	ofSetColor(fgColor.x,	// red
			   fgColor.y,	// green
			   fgColor.z);	// blue
	
	if(filled)
		ofFill();
	else
		ofNoFill();
	
		//horizontals
	if(xLength > 0){
		for(int i = 0; i < xDivisions; i++){
			for(int j = 0; j <yDivisions; j++){
				ofRect((i+0.5)*xSpacing-xLength/2+hOffset, (j+0.5)*ySpacing-thickness/2+vOffset, xLength, thickness);
			}
		}
	}
		//verticals
	if(yLength > 0){
		for(int i = 0; i < xDivisions; i++){
			for(int j = 0; j <yDivisions; j++){
				ofRect((i+0.5)*xSpacing-thickness/2-hOffset, (j+0.5)*ySpacing-yLength/2-vOffset, thickness, yLength);
			}
		}
	}	
	
	ofPopStyle();	
	
	drawTrails();	
}


