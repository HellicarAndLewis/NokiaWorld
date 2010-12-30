#include "guiType2DSliderPixelChooser.h"

//------------------------------------------------
guiType2DSliderPixelChooser::guiType2DSliderPixelChooser(){
	knobSize = 6.0;
}

//------------------------------------------------
void guiType2DSliderPixelChooser::setup(string sliderName, float defaultVal1, float min1, float max1, float defaultVal2, float min2, float max2){
	value.addValue(defaultVal1, min1, max1);
	value.addValue(defaultVal2, min2, max2);
	
		//additions
	value.addValue(127, 0, 255);	// R/G/B
	value.addValue(127, 0, 255);
	value.addValue(127, 0, 255);
	
	value.addValue(0, 0, 9); //index to the images...
	
	string fileName;
	
	for (int i = 0; i < 10; i++){
		
		fileName = "palettes/palette0"; //TODO THIS IS ASKING FOR TROUBLE
		int pos = i + 1;
		if (pos < 10){
			fileName += "0";
		}
		fileName += ofToString(pos) + ".png";
		cout << "loading :  " << fileName << endl;
		myImage[i].loadImage(fileName);
		myImage[i].setImageType(OF_IMAGE_COLOR);
	}
	
	
		// let's give SOME reaasonable middle value to start...
	
	unsigned char * pixels = myImage[0].getPixels();
	int w = (myImage[0].width - 1) * 0.5;
	if (w < 0) w = 0;
	if (w > myImage[0].width - 1) w = myImage[0].width - 1;
	int h = (myImage[0].height - 1) * 0.5;
	if (h < 0) h = 0;
	if (h > myImage[0].height - 1) h = myImage[0].height - 1;
	float r = pixels[ (h * myImage[0].width + w)*3    ] / 255.0f;
	float g = pixels[ (h * myImage[0].width + w)*3 + 1] / 255.0f;
	float b = pixels[ (h * myImage[0].width + w)*3 + 2] / 255.0f;
	value.setValueAsPct( r, 2);
	value.setValueAsPct( g, 3);
	value.setValueAsPct( b, 4);
		//end
	
	name = sliderName;
	updateText();
}

//-----------------------------------------------.
void guiType2DSliderPixelChooser::updateGui(float x, float y, bool firstHit, bool isRelative){
	if( state == SG_STATE_SELECTED){
		float pct1 = ( x - ( hitArea.x ) ) / hitArea.width;
		value.setValueAsPct( pct1, 0);
		float pct2 = ( y - ( hitArea.y ) ) / hitArea.height;
		value.setValueAsPct( pct2, 1);
		updateText();
		//CB
		notify();
	}
}

//-----------------------------------------------
void guiType2DSliderPixelChooser::notify(){
	guiCallbackData cbVal;
	cbVal.setup(xmlName, name);
//	cbVal.addValueF(value.getValueF(0));
//	cbVal.addValueF(value.getValueF(1));
	cbVal.addValueI(value.getValueI(2)); //R
	cbVal.addValueI(value.getValueI(3));	//G
	cbVal.addValueI(value.getValueI(4)); //B
	ofNotifyEvent(guiEvent,cbVal,this);
}

//-----------------------------------------------
void guiType2DSliderPixelChooser::setKnobSize(float _knobSize){
	knobSize = _knobSize;
}

//-----------------------------------------------
void guiType2DSliderPixelChooser::setValue(float _value, int whichParam) {
	value.setValue(_value, whichParam);
}

//-----------------------------------------------.
void guiType2DSliderPixelChooser::updateValue(){
   //CB
   notify();
}

//-----------------------------------------------.
void guiType2DSliderPixelChooser::render(){
	int nImg = (int)(value.getValueF(5));
	nImg %= 10; 	
	
		// a hack, taked out of update: 
	
	float pct1 =  (value.getValueF(0) - value.getMin(0)) / (value.getMax(0) - value.getMin(0));
	float pct2 =  (value.getValueF(1) - value.getMin(1)) / (value.getMax(1) - value.getMin(1));
	
	unsigned char * pixels = myImage[nImg].getPixels();
	
	int w = (myImage[nImg].width - 1) * pct1;
	if (w < 0) w = 0;
	if (w > myImage[nImg].width - 1) w = myImage[nImg].width - 1;
	int h = (myImage[nImg].height - 1) * pct2;
	if (h < 0) h = 0;
	if (h > myImage[nImg].height - 1) h = myImage[nImg].height - 1;
	
	float r = pixels[ (h * myImage[0].width + w)*3    ] / 255.0f;
	float g = pixels[ (h * myImage[0].width + w)*3 + 1] / 255.0f;
	float b = pixels[ (h * myImage[0].width + w)*3 + 2] / 255.0f;
	
	value.setValueAsPct( r, 2);
	value.setValueAsPct( g, 3);
	value.setValueAsPct( b, 4);
	
	ofPushStyle();

		glPushMatrix();
		//glTranslatef(boundingBox.x, boundingBox.y, 0);
			guiBaseObject::renderText();

			//draw the background
			ofFill();
//			glColor4fv(bgColor.getColorF());
			ofSetColor(255, 255, 255);
	
			myImage[nImg].draw(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
	
		//ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			//draw the foreground

			float boxWidth  = knobSize;
			float boxHeight = knobSize;

			float bx = hitArea.x + (boxWidth * 0.5) + ( value.getPct(0) * (hitArea.width - (boxWidth)) );
			float by = hitArea.y + (boxHeight * 0.5) + ( value.getPct(1) * (hitArea.height - (boxHeight)) );

			glColor4fv(fgColor.getColorF());

			ofSetRectMode(OF_RECTMODE_CENTER);
			ofRect(bx, by, boxWidth, boxHeight);
			ofSetRectMode(OF_RECTMODE_CORNER);

			//draw the outline
			ofNoFill();
			glColor4fv(outlineColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

		glPopMatrix();

	ofPopStyle();
}

