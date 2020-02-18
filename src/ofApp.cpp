#include <string> 
#include <chrono>
#include "ofApp.h"

using namespace std;

void ofApp::setup() {


	ofSetCircleResolution(60);

	// image setup 
	img1.load("images/one.jpg");
	imgWidth = img1.getWidth();
	imgHeight = img1.getHeight();
	imgX = 0;
	imgY = 0;

	// start button setup
	startButton.addListener(this, &ofApp::startPressed);
	startButton.setup("Start", 300, 100);
	
	// eye tracker setup
	mEyeX.setup();
	mEyeX.registerGazePointEventHandler();
	mEyeX.registerEyePositionEventHandler();
	mEyeX.registerFixationEventHandler();
	
	// set the position of elements on screen
	resetElementsPos();

	trackingInProgress = false;
	data = json::array();

	// start the step timer
	stepStartTime = chrono::steady_clock::now();
	currentStep = 0;
}


void ofApp::startPressed() {

	if (trackingInProgress) {
		trackingInProgress = false;
		cout << data.dump(4) << endl;
	}
	else {
		// start the timer 
		recordingStartTime = chrono::high_resolution_clock::now();
		trackingInProgress = true;

		// start the next step and the timer for the next step
		currentStep++;
		stepStartTime = recordingStartTime;
	}
}


void ofApp::update() {

	mEyeX.update();
}


void ofApp::draw() {

	ofBackground(30);

	// get the time the current step has been going
	auto now = chrono::steady_clock::now();
	auto dur = now - stepStartTime;
	auto elapsedS = chrono::duration_cast<chrono::seconds>(dur).count();


	switch (currentStep)
	{
	case 0: 
		// for the first step, set up the screen with an intro message
		ofDrawBitmapString("Welcome", 100, 100);
		
		// draw gaze point 
		ofSetColor(255, 255, 255);
		ofDrawCircle(mEyeX.getGazePointData().X, mEyeX.getGazePointData().Y, 20);
	//	ofDrawCircle(mEyeX.getFixationPoint(), 25);

		startButton.draw();
		break;
	case 1: 
		img1.draw(imgX, imgY, imgWidth, imgHeight);
		if (elapsedS > 5) {
			currentStep++;
		}
		break;
	case 2:
		ofDrawBitmapString("All done!", 100, 100);
		break;
	default:
		cout << "Invalid step!";
		break;
	}


	if (trackingInProgress) {
		// record the eye pos points to a file
		recordGazeStream(mEyeX.getGazePoint(), mEyeX.getFixationPoint());
	}
	
}

// center the button and images 
void ofApp::resetElementsPos() {

	int h = ofGetWindowHeight();
	int w = ofGetWindowWidth();

	// set button position
	startButton.setPosition(w/2 - startButton.getWidth()/2 , h/2 - startButton.getHeight()/2);

	// set image position
	float ih = img1.getHeight();
	float iw = img1.getWidth();

	float ihr = ih / iw;
	float iwr = iw / ih;

	// resize image to fill screen
	// this isn't going to work for images with an aspect ratio larger than the screen
	double newW = h * iwr;
	imgHeight = h;
	imgWidth = newW;
	imgY = 0;
	imgX = w / 2 - imgWidth / 2;
}


// save the eye positions to json
void ofApp::recordGazeStream(ofPoint gaze, ofPoint fixation) {

	// calculate the number of ms since start 
	auto now = chrono::high_resolution_clock::now();
	auto dur = now - recordingStartTime;
	auto ms = chrono::duration_cast<chrono::milliseconds>(dur).count();

	// create an object to store the fixation point and time
	json gazeObj = json::object({ {"time", ms}, {"x", gaze.x}, {"y", gaze.y} });
	data.push_back(gazeObj);
}

void ofApp::keyPressed(int key)
{
	if (key == 'f') {
		ofToggleFullscreen();
		resetElementsPos();
	}
}

void ofApp::exit() {

	startButton.removeListener(this, &ofApp::startPressed);
	mEyeX.unregisterGazePointEventHandler();
	mEyeX.unregisterEyePositionEventHandler();
	mEyeX.unregisterFixationEventHandler();
	mEyeX.close();
}