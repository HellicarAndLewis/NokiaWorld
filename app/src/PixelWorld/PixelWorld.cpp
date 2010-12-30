/*
 *  PixelWorld.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "PixelWorld.h"

	//--------------------------------------------------------------
void PixelWorld::setup(){
	mixer.setup();
}

	//this captures all our control panel events - unless its setup differently in testApp::setup
	//--------------------------------------------------------------
	//void testApp::eventsIn(guiCallbackData & data){
	//
	//	//lets send all events to our logger
	//	if( !data.isElement( "events logger" ) ){
	//		string logStr = data.getXmlName();
	//		
	//		for(int k = 0; k < data.getNumValues(); k++){
	//			logStr += " - " + data.getString(k);
	//		}
	//		
	//	}
	//	
	//	// print to terminal if you want to 
	//	//this code prints out the name of the events coming in and all the variables passed
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
	//}

	//--------------------------------------------------------------
void PixelWorld::update(){	
	mixer.update();
}

	//--------------------------------------------------------------
void PixelWorld::draw(){
	mixer.draw();
}


	//--------------------------------------------------------------
void PixelWorld::keyPressed  (int key){
	mixer.keyPressed(key);
}

	//--------------------------------------------------------------
void PixelWorld::keyReleased  (int key){
	mixer.keyReleased(key);
}

	//--------------------------------------------------------------
void PixelWorld::mouseMoved(int x, int y ){
	mixer.mouseMoved(x,y);
}

	//--------------------------------------------------------------
void PixelWorld::mouseDragged(int x, int y, int button){
	mixer.mouseDragged(x, y, button);
}

	//--------------------------------------------------------------
void PixelWorld::mousePressed(int x, int y, int button){
	mixer.mousePressed(x, y, button);
	
}

	//--------------------------------------------------------------
void PixelWorld::mouseReleased(){
	mixer.mouseReleased();
}

void PixelWorld::audioReceived(float * input, int bufferSize, int nChannels){
	mixer.audioReceived(input, bufferSize, nChannels);
}

