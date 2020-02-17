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
}


void ofApp::startPressed() {

	if (trackingInProgress) {
		trackingInProgress = false;
		cout << data.dump(4) << endl;
	}
	else {
		// start the timer 
		startTime = chrono::high_resolution_clock::now();
		trackingInProgress = true;
	}

}


void ofApp::update() {

	mEyeX.update();
}


void ofApp::draw() {
	ofBackground(30);

	// this stuff will change when there's a state 
	img1.draw(imgX, imgY, imgWidth, imgHeight);
	startButton.draw();

	// Draw eye position
	/*ofSetColor(0, 255, 0);
	ofFill();
	if (mEyeX.hasLeftEye())
	{
		ofPoint p = mEyeX.getLeftEyeNormalized();
		ofDrawCircle(
			p.x * ofGetWidth(),
			p.y * ofGetHeight(),
			(1. - p.z) * 80);
	}
	if (mEyeX.hasRightEye())
	{
		ofPoint p = mEyeX.getRightEyeNormalized();
		ofDrawCircle(
			p.x * ofGetWidth(),
			p.y * ofGetHeight(),
			(1. - p.z) * 80);
	}*/

	// Draw gaze point
	// probably don't want to do this when we're recording
	ofSetColor(255, 255, 255);
	ofDrawCircle(mEyeX.getGazePointData().X, mEyeX.getGazePointData().Y, 20);
	//ofDrawCircle(mEyeX.getFixationPoint(), 25);

	if (trackingInProgress) {
		// record the eye pos points to a file
		// we're using the gaze point rather than the fixation point because it is 
		// essentially the raw stream of data, not corrected for just the points where
		// the eyes fixate (there's no movement in the fixation points)
		recordGazeStream(mEyeX.getGazePoint());
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
void ofApp::recordGazeStream(ofPoint loc) {

	// calculate the number of ms since start 
	auto now = chrono::high_resolution_clock::now();
	auto dur = now - startTime;
	auto ms = chrono::duration_cast<chrono::milliseconds>(dur).count();

	// create an object to store the fixation point and time
	json fixation = json::object({ {"time", ms}, {"x", loc.x}, {"y", loc.y} });
	data.push_back(fixation);

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