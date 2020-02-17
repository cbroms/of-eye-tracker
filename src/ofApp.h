#pragma once

#include <chrono>
#include <nlohmann/json.hpp>
#include "ofMain.h"
#include "ofxTobiiEyeX.h"
#include "ofxGui.h"

// using https://github.com/nlohmann/json for JSON
using json = nlohmann::json;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void exit();
	void startPressed();
	void resetElementsPos();
	void recordGazeStream(ofPoint loc);

	int imgHeight;
	int imgWidth;
	int imgX;
	int imgY;

	json data;

	bool trackingInProgress;
	chrono::time_point<chrono::steady_clock> startTime;

	ofxButton startButton;
	ofxTobiiEyeX mEyeX;
	ofImage img1;
};