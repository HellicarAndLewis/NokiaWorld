/*
 *  PixelFactory.cpp
 *  NokiaWorld
 *
 *  Created by Joel Gethin Lewis on 07/09/2010.
 *  Copyright 2010 Hellicar&Lewis. All rights reserved.
 *
 */

#include "FFTPixelFactory.h"

void FFTPixelFactory::setup(int aChannelNumber){
	
	factoryName = "FFTPixelFactory"+ofToString(aChannelNumber);
	
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(50, 50, 240, 255));	
	
	panel.setup("FACTORY "+factoryName, ((settings.GUI_W/4.f) * aChannelNumber) + (aChannelNumber * 20.f) , settings.GUI_H/2.f, 500, 400);
	panel.addPanel("visuals", 2, false);
	panel.setWhichPanel("visuals");
	panel.setWhichColumn(0);
	
	PixelFactory::addPixelFactoryStandardGUIElements();
	
	panel.addSlider("Line Thickness", "VIS_THICKNESS", 10, 1, 20, true);

	panel.setWhichColumn(1);
	gradientModes.push_back("Solid");
	gradientModes.push_back("Bars");
	gradientModes.push_back("Single Line");
	gradientModes.push_back("Single Curve");	
	gradientModes.push_back("Mirror Solid");	
	gradientModes.push_back("Mirrored Bars");		
	gradientModes.push_back("Mirrored Line");
	gradientModes.push_back("Mirrored Curve");	
	panel.addMultiToggle("Effect", "VIS_EFFECT_NAME", 0, gradientModes);	
	
	panel.loadSettings(factoryName + "Settings.xml");
}

void FFTPixelFactory::update(){
	panel.update();
	
	thickness = panel.getValueI("VIS_THICKNESS");
}

void FFTPixelFactory::draw(){	
	ofPushStyle();
	
	int gradR, gradG, gradB;
	
		//draw bg
	ofFill();
	
	ofSetColor(bgColor.x,	// red
			   bgColor.y,	// green
			   bgColor.z);	// blue
	ofRect(0,0,settings.OUTPUT_CROP_W,settings.OUTPUT_CROP_H);
	
	ofSetColor(fgColor.x, 
			   fgColor.y, 
			   fgColor.z);
	
	float panelWidth = settings.OUTPUT_CROP_W/5.f; //
	float panelHeight = settings.OUTPUT_CROP_H;
	
	float barWidth = panelWidth/fftLevels.size();
	float barHeight = panelHeight;
	
	float sizeOfBar = panel.getValueI("VIS_THICKNESS");
	
	int effectNo = panel.getValueI("VIS_EFFECT_NAME");
	
	switch (effectNo){			
		case 0:{ //normal
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight;
					
					ofRect(barX, barY, barWidth, currentHeight);						
				}
					
			}
		
			break;			
		}
		case 1:{ //bars
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight-sizeOfBar;
					
					ofRect(barX, barY, barWidth, sizeOfBar);						
				}
				
			}	
			
			break;			
		}
		case 2:{ //single line
			
			
			ofNoFill();
			ofSetLineWidth(thickness);
			ofBeginShape();
		
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight-sizeOfBar;
					
					ofVertex(barX,barY);
				}
				
			}	
			
			ofEndShape();	
			
			break;			
		}	
		case 3:{ //single curve
			
			
			ofNoFill();
			ofSetLineWidth(thickness);
			ofBeginShape();
			
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight-sizeOfBar;
					
					ofCurveVertex(barX,barY);
				}
				
			}	
			
			ofEndShape();	
			
			break;			
		}			
		case 4:{ //mirrored normal
			float mirrorHorizOffSet = panelWidth/2.f;
			
			barWidth = barWidth/2.f; //getting ready for mirroring
			
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight;
					
					ofRect(barX, barY, barWidth, currentHeight);						
				}
				
			}
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[(fftLevels.size()-1)-j]);
					
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight;
					
					ofRect(barX+mirrorHorizOffSet, barY, barWidth, currentHeight);						
				}
				
			}			
			
			break;			
		}
		case 5:{ //mirrored bars
			float mirrorHorizOffSet = panelWidth/2.f;
			
			barWidth = barWidth/2.f; //getting ready for mirroring			
			
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight-sizeOfBar;
					
					ofRect(barX, barY, barWidth, sizeOfBar);						
				}
				
			}
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[(fftLevels.size()-1)-j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight-sizeOfBar;
					
					ofRect(barX+mirrorHorizOffSet, barY, barWidth, sizeOfBar);						
				}
				
			}				
			
			break;			
		}
		case 6:{ //mirrored single line
			float mirrorHorizOffSet = panelWidth/2.f;
			
			barWidth = barWidth/2.f; //getting ready for mirroring					
			
			ofNoFill();
			ofSetLineWidth(thickness);
			ofBeginShape();
			
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight-sizeOfBar;
					
					ofVertex(barX,barY);
				}
			}
			
			ofEndShape();
			ofBeginShape();
			
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[(fftLevels.size()-1)-j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight-sizeOfBar;
					
					ofVertex(barX+mirrorHorizOffSet,barY);
				}
			}			
			
			ofEndShape();	
			
			break;			
		}	
		case 7:{ //mirrored single curve
			float mirrorHorizOffSet = panelWidth/2.f;
			
			barWidth = barWidth/2.f; //getting ready for mirroring					
			
			ofNoFill();
			ofSetLineWidth(thickness);
			ofBeginShape();
			
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight-sizeOfBar;
					
					ofCurveVertex(barX,barY);
				}
				
			}	
			
			ofEndShape();
			ofBeginShape();			
			
			for(int i=0; i<5; i++){
				for(int j=0; j<fftLevels.size(); j++){
					float currentHeight = barHeight*(fftLevels[(fftLevels.size()-1)-j]);
					float barX = (i*panelWidth) + (j*barWidth);
					float barY = barHeight-currentHeight-sizeOfBar;
					
					ofCurveVertex(barX+mirrorHorizOffSet,barY);
				}
				
			}				
			
			ofEndShape();	
			
			break;			
		}				

	}		
	
	ofPopStyle();	
	
	drawTrails();		
}


