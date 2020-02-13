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

	ofxButton startButton;
	ofxTobiiEyeX mEyeX;
};