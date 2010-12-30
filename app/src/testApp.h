#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "AppSettings.h"

#include "PixelWorld.h"

class testApp : public ofBaseApp{

	public:
		
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
	
public:
	
	PixelWorld thePixelWorld;
};

#endif	