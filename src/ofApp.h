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
	void saveGazeStreamToFile(ofImage img);
	void drawInstructionText(string message, int width, int height);
	void drawCurrentImage(int elapsedS, int numSecondsLooking);
	void drawCurrentImageUntilComplete(bool complete);

	int imgHeight;
	int imgWidth;
	int imgX;
	int imgY;

	int currentStep;
	int numSecondsLooking = 40;

	bool debug = false;
	bool recording = false; 
	bool screenComplete = false;

	json data;

	string filename = "";

	chrono::time_point<chrono::high_resolution_clock> recordingStartTime;
	chrono::time_point<chrono::steady_clock> stepStartTime;

	ofxButton startButton;
	ofxTobiiEyeX mEyeX;

	ofTrueTypeFont font;

	int numImages = 4;
	int currentImage = 0;

	ofImage imgs[4];

};