/*
 *  PixelMixer.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "PixelMixer.h"

//--------------------------------------------------------------
void PixelMixer::setup(){
	
	spaceBar = false;
	
	numberOfChannels = 2;
	
		//from avsys/analysis2/aubioExample
	
		// 0 output channels, 
		// 2 input channels
		// 44100 samples per second
		// 256 samples per buffer
		// 4 num buffers (latency)
	
		//	//setup of sound input - in testapp now
		//	ofSoundStreamSetup(0, 2, this, 44100, BUFFER_SIZE, 4);	
	
	pitchAmplitudeLeft = new float[MIXER_BUFFER_SIZE];
	pitchAmplitudeRight = new float[MIXER_BUFFER_SIZE];
	
	FFTLeft = new float[MIXER_BUFFER_SIZE];
	FFTRight = new float[MIXER_BUFFER_SIZE];
	
	AA.setup();
	
		//	dinFont.loadFont("DIN.otf", 50);
	
		//from avsys/analysis2/fft
	
		// 0 output channels, 
		// 2 input channels
		// 44100 samples per second
		// BUFFER_SIZE samples per buffer
		// 4 num buffers (latency)
	
		//	ofSoundStreamSetup(0,2,this, 44100, BUFFER_SIZE, 4);	
		//	
		//	left = new float[BUFFER_SIZE];
		//	right = new float[BUFFER_SIZE];
	
	ofSetColor(0x666666);
	
	FFTanalyzer.setup(44100, MIXER_BUFFER_SIZE/2, 2);
	
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.95f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies
	
	decayedAudioLevel = 0.f;
	loudestAudioLevelRecieved = 0.f;	
	rawAudioLevel = 0.f;
	scaledAmplitude = 0.f;	
	
		//doing the audio first above, so that can add the fft bits (number of averages) programmatically
	
		//also set up the vector of channels
	
	channels[0] = new PixelChannel(); //HAVE TO DO THIS 
	channels[1] = new PixelChannel();	
	
	for(int i=0; i < numberOfChannels; i++){
		channels[i]->setup(i);
	}
	
	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 60, 200));
	ofxControlPanel::setTextColor(simpleColor(240, 50, 50, 255));

	mixerGui.loadFont("Monaco.TTF", 8);		
	mixerGui.setup("MIXER Nokia World 2010 by Hellicar&Lewis", 0, 0, 500, 500);
	mixerGui.addPanel("Mixer", 4, false);

		//--------- PANEL 1
	mixerGui.setWhichPanel(0);

	mixerGui.setWhichColumn(0);
	
	mixerGui.addSlider("Mix", "MIX", 0.f, 0.f, 1.f, false);	

		//some dummy vars we will update to show the variable lister object
	elapsedTime		= ofGetElapsedTimef();
	appFrameCount	= ofGetFrameNum();	
	appFrameRate	= ofGetFrameRate();
	mixerAudioLevel = 0.f;

	vector <guiVariablePointer> vars;
	vars.push_back( guiVariablePointer("elapsed time", &elapsedTime, GUI_VAR_FLOAT, 2) );
	vars.push_back( guiVariablePointer("elapsed frames", &appFrameCount, GUI_VAR_INT) );
	vars.push_back( guiVariablePointer("app fps", &appFrameRate, GUI_VAR_FLOAT, 2) );
	vars.push_back( guiVariablePointer("app audio level", &mixerAudioLevel, GUI_VAR_FLOAT, 2) );
				   
	mixerGui.addVariableLister("app vars", vars);
	
	mixerGui.addChartPlotter("Audio Chart", guiStatVarPointer("app audio level", &mixerAudioLevel, GUI_VAR_FLOAT, true, 2), 200, 100, 200, 0, 1);

	mixerGui.addChartPlotter("FPS Chart", guiStatVarPointer("app fps", &appFrameRate, GUI_VAR_FLOAT, true, 2), 200, 100, 200, 40, 80);

	mixerGui.setWhichColumn(2);
	mixerGui.addSlider("minium audio level", "AUDIO_MIN", 0.01f, 0.f, 0.1f, false);	
	mixerGui.addSlider("maximum audio level", "AUDIO_MAX", 0.05f, 0.f, 0.3f, false);
	mixerGui.addSlider("audio decay rate", "AUDIO_DECAY", 0.95f, 0.7f, 1.f, false);	
	mixerGui.addSlider("manual audio level", "AUDIO_MANUAL_LEVEL", 0.f, 0.f, 1.f, false);
	mixerGui.addSlider("trigger audio level", "AUDIO_TRIGGER_LEVEL", 0.5f, 0.f, 1.f, false);
	mixerGui.addSlider("fft band", "AUDIO_FFT_BAND", 0, 0, 	(FFTanalyzer.nAverages) - 1, true); //1 as we are indexing from 0 as per normal
	vector <string> audioModeNames;
	audioModeNames.push_back("normal audio");
	audioModeNames.push_back("decay audio");
	audioModeNames.push_back("manual audio");
	audioModeNames.push_back("fft audio");
	audioModeNames.push_back("fft peaks");	
	mixerGui.addTextDropDown("audio mode", "AUDIO_MODE", 0, audioModeNames);	

	//SETTINGS AND EVENTS

	//load from xml!
	mixerGui.loadSettings("PixelMixerSettings.xml");

	//if you want to use events call this after you have added all your gui elements
	mixerGui.setupEvents();
	mixerGui.enableEvents();	
	
	fftLevels.resize(FFTanalyzer.nAverages);
}

//--------------------------------------------------------------
void PixelMixer::update(){	
	updateAudio();
	
	for(int i = 0; i < numberOfChannels; i++){
		channels[i]->setLevelLevelPeakSpace(mixerAudioLevel,mixerGui.getValueF("AUDIO_TRIGGER_LEVEL"),spaceBar, fftLevels);
		channels[i]->update();
	}
	
	mixerGui.update();
	
	spaceBar = false;
}

//--------------------------------------------------------------
void PixelMixer::draw(){
	
	float mixAmount = 1.f-mixerGui.getValueF("MIX");
	
		//hacky drawing back, hey zach did it first!
	for(int i = 0; i < numberOfChannels; i++){
		channels[i]->draw(mixAmount);  //this is where the textures get grabbed back from the screen
		mixAmount = 1.f-mixAmount;
	}
	
	mixAmount = 1.f-mixerGui.getValueF("MIX");
	
		//previews
	for(int i = 0; i < numberOfChannels; i++){
		ofRectangle positionToDraw;
		
		positionToDraw.width = settings.OUTPUT_CROP_W / 2.f;
		positionToDraw.height = settings.OUTPUT_CROP_H / 2.f;	
		positionToDraw.x = 0;
		positionToDraw.y = 200 + (i*positionToDraw.height);		
		
		channels[i]->drawViaTexture(1.f, positionToDraw); //full alpha
		
			//and in the top left...
		positionToDraw.width = settings.OUTPUT_CROP_W;
		positionToDraw.height = settings.OUTPUT_CROP_H;	
		positionToDraw.x = 0;
		positionToDraw.y = 0;
		channels[i]->drawViaTexture(mixAmount, positionToDraw); //full alpha	
		mixAmount = 1.f-mixAmount;
	}		
	
	drawAudio();
	
		//this will be an if statement depending on what mode the gui is in, or have a few of them displaying at the same time
	
		//just draw all of them at the moment
	for(int i = 0; i < numberOfChannels; i++){
		channels[i]->drawGUI(); 
	}	
	
		//just and their factory guis...
	for(int i = 0; i < numberOfChannels; i++){
		channels[i]->drawFactoryGUI(); 
	}		
	
	ofSetColor(0xffffff);
	mixerGui.draw();	
	
		//drawing last for safety so we don't get an gui overlaps
	
		//final output....
		//no fading yet.... or alpha...
	
	mixAmount = 1.f-mixerGui.getValueF("MIX");
	
	for(int i = 0; i < numberOfChannels; i++){
		ofRectangle positionToDraw;
		
		positionToDraw.x = settings.GUI_W;
		positionToDraw.y = 0; //SJAHKHDJHAS LKDLKA JKS momng
		positionToDraw.width = settings.OUTPUT_CROP_W;
		positionToDraw.height = settings.OUTPUT_CROP_H;
		
		channels[i]->drawViaTexture(mixAmount, positionToDraw);
		
		mixAmount = 1.f - mixAmount; //hacky, but we only have two channels for now
	}
}

//--------------------------------------------------------------
void PixelMixer::keyPressed(int key){
	    //if you need to adjust the camera properties
	switch (key){
//		case 'f':{
//			ofToggleFullscreen();
//			break;				
//		}
//		case 'h':{
//			gui.toggleView();	
//			break;
//		}
		case 'r':{
			loudestAudioLevelRecieved = 0.f;
		}
		case ' ':{
			spaceBar = true;
		}
		default:{
			break;
		}
	}	
}

//--------------------------------------------------------------
void PixelMixer::keyReleased(int key){
}

//--------------------------------------------------------------
void PixelMixer::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void PixelMixer::mouseDragged(int x, int y, int button){
	mixerGui.mouseDragged(x, y, button);
	
	
	for(int i = 0; i < numberOfChannels; i++){
		channels[i]->mouseDragged(x, y, button);
	}	
}

//--------------------------------------------------------------
void PixelMixer::mousePressed(int x, int y, int button){
	mixerGui.mousePressed(x, y, button);	
	
	
	for(int i = 0; i < numberOfChannels; i++){
		channels[i]->mousePressed(x, y, button);
	}	
}

//--------------------------------------------------------------
void PixelMixer::mouseReleased(){
	mixerGui.mouseReleased();
	
	
	for(int i = 0; i < numberOfChannels; i++){
		channels[i]->mouseReleased();
	}	
}
				   
//from avsys/analysis2/aubioExample
//--------------------------------------------------------------
void PixelMixer::audioReceived (float * input, int bufferSize, int nChannels){	
	   // samples are "interleaved"
   for (int i = 0; i < bufferSize; i++){
	   pitchAmplitudeLeft[i]	= input[i*2];
	   FFTLeft[i]				= input[i*2];
	   pitchAmplitudeRight[i]	= input[i*2+1];
	   FFTRight[i]				= input[i*2+1];	
   }
   
   AA.processAudio(pitchAmplitudeLeft, bufferSize);
}

void PixelMixer::updateAudio(){
		//audio level
	
	float levelToUse = 0.5f;
	
	rawAudioLevel = AA.amplitude;
	
	if(rawAudioLevel > loudestAudioLevelRecieved){
		loudestAudioLevelRecieved = rawAudioLevel;
	}
	
	scaledAmplitude = ofNormalize(rawAudioLevel, mixerGui.getValueF("AUDIO_MIN"), mixerGui.getValueF("AUDIO_MAX"));
	
	if(scaledAmplitude > decayedAudioLevel){
		decayedAudioLevel = scaledAmplitude;
	}else{
			//decay it
		decayedAudioLevel = decayedAudioLevel * mixerGui.getValueF("AUDIO_DECAY");
	}
	
	decayedAudioLevel = ofNormalize(decayedAudioLevel, mixerGui.getValueF("AUDIO_MIN"), 1.f);
	
	int audioMode = mixerGui.getValueI("AUDIO_MODE");
	
	if(audioMode == 0){
			//normal mode
		levelToUse = scaledAmplitude;
	}else if(audioMode == 1){
			//decay mode
		levelToUse = decayedAudioLevel;
	}else if(audioMode == 2){
			//manual mode
		levelToUse = mixerGui.getValueF("AUDIO_MANUAL_LEVEL");
	}else if(audioMode == 3){
			//fft mode
		levelToUse =  FFTanalyzer.averages[mixerGui.getValueI("AUDIO_FFT_BAND")];
		levelToUse = ofNormalize(levelToUse, 0.f, 48.f);
	}else if(audioMode == 4){
			//fft peak mode
		levelToUse =  FFTanalyzer.peaks[mixerGui.getValueI("AUDIO_FFT_BAND")];
		levelToUse = ofNormalize(levelToUse, 0.f, 48.f);
	}
	
	mixerAudioLevel = levelToUse;	
	
	for(int i=0; i < fftLevels.size(); i++){
		float peak = FFTanalyzer.peaks[i];		
		fftLevels[i] = ofNormalize(peak, 0.f, 48.f);
	}	
	
		//from avsys/analysis2/fft
	
	static int index=0;
	float avg_power = 0.0f;	
	
	/* do the FFT	*/
	myfft.powerSpectrum(0,(int)MIXER_BUFFER_SIZE/2, FFTLeft,MIXER_BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
	
	for (int i = 0; i < (int)(MIXER_BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	
	FFTanalyzer.calculate(freq);	
}

void PixelMixer::drawAudio(){
	ofPushStyle();

 	
	ofFill();	
		
		//from avsys/analysis2/aubioExample
	
		// draw the pitchAmplitudeLeft:
//	ofSetColor(0x333333);
//	ofRect(0,0,MIXER_BUFFER_SIZE,200);
//	ofSetColor(0xFFFFFF);
//	for (int i = 0; i < MIXER_BUFFER_SIZE; i++){
//		ofLine(i,100,i,100+pitchAmplitudeLeft[i]*200);
//	}
	
	ofSetColor(255,0,0);
	
	ofDrawBitmapString("pitch is : " + ofToString((int)AA.pitch), 50, 300);
	ofDrawBitmapString("raw audio is : " + ofToString(rawAudioLevel, 3), 50, 320);
	ofDrawBitmapString("scaled audio is : " + ofToString(scaledAmplitude, 3), 50, 340);
	ofDrawBitmapString("decayed audio is : " + ofToString(decayedAudioLevel, 3), 50, 360);	
	ofDrawBitmapString("loudest raw  audio is : " + ofToString(loudestAudioLevelRecieved, 3), 50, 380);	
	ofDrawBitmapString("used audio is : " + ofToString(mixerAudioLevel, 3), 50, 400);	
	
		//from avsys/analysis2/fft
	
	ofSetColor(0xffffff);
	for (int i = 0; i < (int)(MIXER_BUFFER_SIZE/2 - 1); i++){
			//ofRect(200+(i*4),600,4,-freq[i]*10.0f);
	}
	
		//for highlighting the selected FFT
	int audioMode = mixerGui.getValueI("AUDIO_MODE");
	
	for (int i = 0; i < FFTanalyzer.nAverages; i++){
		if((audioMode == 3 || audioMode == 4) && (i == mixerGui.getValueI("AUDIO_FFT_BAND"))){
			ofPushStyle();
			ofSetColor(255, 0, 0);
			ofRect(i*20,600,20,-FFTanalyzer.averages[i] * 6);
			ofPopStyle();
		}else{
			ofRect(i*20,600,20,-FFTanalyzer.averages[i] * 6);			
		}
	}
	
	ofSetColor(0xff0000);
	for (int i = 0; i < FFTanalyzer.nAverages; i++){
		ofRect(i*20,600-FFTanalyzer.peaks[i] * 6,20,-4);
	}
	
	ofPopStyle();
}
