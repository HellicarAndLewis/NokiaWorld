#include "testApp.h"
#include "stdio.h"

extern "C" {
#include "macGlutfix.h"
}

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0,0,0);
	
		// load the settings
	settings.loadSettings("AppSettings.xml");	
	
	thePixelWorld.setup();
		
	ofSetVerticalSync(true);	
	
		//setup of sound input
	ofSoundStreamSetup(0, 2, this, 44100, MIXER_BUFFER_SIZE, 4);		//audio set up has to be done out here....
}

//--------------------------------------------------------------
void testApp::update(){	
	thePixelWorld.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	thePixelWorld.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	thePixelWorld.keyPressed(key);
	
	switch (key){
		case 'f':{
			ofToggleFullscreen();
			break;
		}
	}	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
	thePixelWorld.keyReleased(key);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	thePixelWorld.mouseMoved(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	thePixelWorld.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	thePixelWorld.mousePressed(x, y, button);

}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	thePixelWorld.mouseReleased();
}

	//from avsys/analysis2/aubioExample
	//--------------------------------------------------------------
void testApp::audioReceived (float * input, int bufferSize, int nChannels){	
	thePixelWorld.audioReceived(input, bufferSize, nChannels);
}

