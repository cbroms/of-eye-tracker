#pragma once

#include "ofMain.h"
#include "ofxTobiiEyeX.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void exit();
	void startPressed();
	void resetElementsPos();

	int imgHeight;
	int imgWidth;

	ofxButton startButton;
	ofxTobiiEyeX mEyeX;
	ofImage img1;
};