/*
 *  PixelMixer.h
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "AppSettings.h"

	//from theo ofxcontrol panel

#include "ofxControlPanel.h"

	//from avsys/analysis2/aubioExample

#include "aubioAnalyzer.h"

	//from avsys/analysis2/fft

#include "fft.h"
#include "FFTOctaveAnalyzer.h"

#define MIXER_BUFFER_SIZE 512

#include "PixelChannel.h"

class PixelMixer{
	
public:
	PixelMixer(){		
		mixerName = "unnamedMixer";  
	}
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
		//from avsys/analysis2/aubioExample
	
	void audioReceived(float * input, int bufferSize, int nChannels);
	void updateAudio();
	void drawAudio();
	
public:
	ofxControlPanel mixerGui;
	
	float elapsedTime;
	int appFrameCount;
	float appFrameRate;
	
		//from avsys/analysis2/aubioExample
	float * pitchAmplitudeLeft;
	float * pitchAmplitudeRight;
	
	float * FFTLeft;
	float * FFTRight;	
	
	aubioAnalyzer AA;
	
		//from avsys/analysis2/fft
	
	FFTOctaveAnalyzer FFTanalyzer;
	
	float mixerAudioLevel;  //decayed, live or manual level is dropped into here....
	
private:	
	int 	bufferCounter;
	fft		myfft;
	
	float magnitude[MIXER_BUFFER_SIZE];
	float phase[MIXER_BUFFER_SIZE];
	float power[MIXER_BUFFER_SIZE];
	float freq[MIXER_BUFFER_SIZE/2];
	
	float decayedAudioLevel;
	float loudestAudioLevelRecieved;
	float rawAudioLevel;
	float scaledAmplitude;
	
public:
	PixelChannel *	channels[2];
	string mixerName;
	int numberOfChannels;
	bool spaceBar;
	vector <float> fftLevels;
};

	