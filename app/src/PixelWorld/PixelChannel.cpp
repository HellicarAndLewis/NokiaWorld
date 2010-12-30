/*
 *  PixelChannel.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "PixelChannel.h"

void PixelChannel::setup(int aChannelNumber){
	
	channelNumber = aChannelNumber;
	
	currentFactory = 0;
	
	factories[0] = new SinglePeakPixelFactory();
	factories[1] = new ImagePixelFactory();	
	factories[2] = new GridLinesPixelFactory();
	factories[3] = new StringPixelFactory(); //hardcoding is safe
	factories[4] = new StrobePixelFactory();
	factories[5] = new PulseCirclePixelFactory();	
	factories[6] = new BlockPixelFactory();
	factories[7] = new ParticlePixelFactory();
	factories[8] = new ImageManipulationPixelFactory();	
	factories[9] = new SinglePanelImagePixelFactory();
	factories[10] = new NoisePixelFactory();
//	factories[11] = new ImageMoverFactory();
	factories[11] = new SlitStretchFactory();
	factories[12] = new FFTPixelFactory();	
	
	for (int i = 0; i < FACTORIES_COUNT; i++){
		factories[i]->setup(aChannelNumber);
	}
	
	channelName = "CHANNEL " + ofToString(channelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(30, 60, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 240, 50, 255));	
	
	channelGUI.loadFont("Monaco.TTF", 8);		
	channelGUI.setup(channelName+"NW2010" , 320 + aChannelNumber*20, 320 + aChannelNumber*20, 400, 500);
	channelGUI.addPanel(channelName, 4, false);
	channelGUI.setWhichPanel(0);
	channelGUI.setWhichColumn(0);
	channelGUI.addSlider("select factory", "FACTORY", 0, 0, FACTORIES_COUNT-1, true);	
	channelGUI.addSlider2DPixelChooser("foreground", channelName+"COLOR_FG", 320/2, 240/2, 0, 320, 0, 240, false);
	channelGUI.addSlider("fg palette", channelName+"PALETTE_FG", 0, 0, 9, true);
	channelGUI.addSlider2DPixelChooser("background", channelName+"COLOR_BG", 320/2, 240/2, 0, 320, 0, 240, false);
	channelGUI.addSlider("bg palette", channelName+"PALETTE_BG", 0, 0, 9, true);
	channelGUI.loadSettings(channelName + "Settings.xml");	
}

void PixelChannel::setLevelLevelPeakSpace(float _level, float  _levelPeak, bool _space, vector <float> _fftLevels){
	for (int i = 0; i < FACTORIES_COUNT; i++){
		factories[i]->setLevelLevelPeakSpace(_level,_levelPeak, _space, _fftLevels);
	}
}


void PixelChannel::setColor(ofPoint color, bool bFg){
	for (int i = 0; i < FACTORIES_COUNT; i++){
		if (bFg) factories[i]->fgColor = color;
		else factories[i]->bgColor = color;
	}
	
}

void PixelChannel::update(){
	channelGUI.update();
	
	currentFactory = channelGUI.getValueI("FACTORY");
	
	ofPoint fgColor;
	ofPoint bgColor;	
	
	int pal = channelGUI.getValueI(channelName+"PALETTE_FG");
	channelGUI.setValueF(channelName+"COLOR_FG", pal, 5);
	pal = channelGUI.getValueI(channelName+"PALETTE_BG");
	channelGUI.setValueF(channelName+"COLOR_BG", pal, 5);
	
	fgColor.x = channelGUI.getValueI(channelName+"COLOR_FG", 2);
	fgColor.y = channelGUI.getValueI(channelName+"COLOR_FG", 3);
	fgColor.z = channelGUI.getValueI(channelName+"COLOR_FG", 4);
	
	bgColor.x = channelGUI.getValueI(channelName+"COLOR_BG", 2);
	bgColor.y = channelGUI.getValueI(channelName+"COLOR_BG", 3);
	bgColor.z = channelGUI.getValueI(channelName+"COLOR_BG", 4);
	
	setColor(fgColor, true);
	setColor(bgColor, false);	
	
	for (int i = 0; i < FACTORIES_COUNT; i++){
		factories[i]->update();
	}
}

void PixelChannel::draw(float alpha){
	factories[currentFactory]->draw();
	factories[currentFactory]->readBackIntoTexture();
}

void PixelChannel::drawViaTexture(float alpha, ofRectangle pos){
	factories[currentFactory]->drawViaTexture(alpha,pos);	
}

	//--------------------------------------------------------------
void PixelChannel::keyPressed(int key){
}

	//--------------------------------------------------------------
void PixelChannel::keyReleased(int key){
}

	//--------------------------------------------------------------
void PixelChannel::mouseMoved(int x, int y ){
}

	//--------------------------------------------------------------
void PixelChannel::mouseDragged(int x, int y, int button){
	channelGUI.mouseDragged(x, y, button);
	factories[currentFactory]->mouseDragged(x, y, button);
}

	//--------------------------------------------------------------
void PixelChannel::mousePressed(int x, int y, int button){
	channelGUI.mousePressed(x, y, button);	
	factories[currentFactory]->mousePressed(x, y, button);	
}

	//--------------------------------------------------------------
void PixelChannel::mouseReleased(){
	channelGUI.mouseReleased();
	factories[currentFactory]->mouseReleased();
}

void PixelChannel::drawGUI(){
	channelGUI.draw();
}

void PixelChannel::drawFactoryGUI(){
	factories[currentFactory]->drawGUI();
}



