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
	void recordGazeStream(ofPoint gaze, ofPoint fixation);

	int imgHeight;
	int imgWidth;
	int imgX;
	int imgY;

	int currentStep;

	json data;

	bool trackingInProgress;
	chrono::time_point<chrono::high_resolution_clock> recordingStartTime;
	chrono::time_point<chrono::steady_clock> stepStartTime;

	ofxButton startButton;
	ofxTobiiEyeX mEyeX;
	ofImage img1;
};