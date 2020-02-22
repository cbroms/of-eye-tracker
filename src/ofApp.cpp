#include <string> 
#include <sstream>
#include <chrono>
#include <ctime>
#include "ofApp.h"

using namespace std;

void ofApp::setup() {

	// load the font
	font.load("fonts/Roboto-Regular.ttf", 24);
	startButton.loadFont("fonts/Roboto-Regular.ttf", 24);

	ofSetCircleResolution(60);

	// image setup 
	for (int i = 0; i < numImages; i++) {
		string imgName = "images/" + to_string(i + 1) + ".jpg";
		ofImage img;
		img.load(imgName);
		imgs[i] = img;
	}

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
	
	// set the position of elements on screen, including images
	resetElementsPos();

	data = json::array();

	// start the step timer
	stepStartTime = chrono::steady_clock::now();
	currentStep = 0;


	ofSetFrameRate(30);
}


void ofApp::startPressed() {


	// start the timer 
	recordingStartTime = chrono::high_resolution_clock::now();

	// start the next step and the timer for the next step
	currentStep++;
	stepStartTime = recordingStartTime;
}


void ofApp::update() {

	mEyeX.update();
}

void ofApp::drawCurrentImage(int elapsedS, int numSecondsLooking) {

	// for the first 5 seconds, draw the image and track/save fixation and gaze points
	if (elapsedS < numSecondsLooking) {
		imgs[currentImage].draw(imgX, imgY, imgWidth, imgHeight);

		// record the eye pos points to a file
		recordGazeStream(mEyeX.getGazePoint(), mEyeX.getFixationPoint());
	}
	else {
		// the timer has finished, move on to the next screen
		saveGazeStreamToFile(imgs[currentImage]);
		currentStep++;
		currentImage++;
		resetElementsPos();
	}
}


void ofApp::draw() {

	ofBackground(30);

	int numSecondsLooking = 5;

	int h = ofGetWindowHeight();
	int w = ofGetWindowWidth();

	// get the time the current step has been going
	auto now = chrono::steady_clock::now();
	auto dur = now - stepStartTime;
	auto elapsedS = chrono::duration_cast<chrono::seconds>(dur).count();

	switch (currentStep)
	{
	case 0: // for the first step, set up the screen with an intro message
		
		drawInstructionText("Welcome! You'll be shown a series of images, each for 30 seconds.\nLook at each image as if it were on a wall in front of you.", w, h);
		startButton.draw();
		break;

	case 1: // show an image and track the gaze 
		drawCurrentImage(elapsedS, numSecondsLooking);
		break;

	case 2: // show the second question and next button
		drawInstructionText("Done! Continue to the next image.", w, h);
		startButton.draw();
		break;

	case 3: // show second image and track the gaze
		drawCurrentImage(elapsedS, numSecondsLooking);
		break;
	
	case 4: // show the third question and next button

		drawInstructionText("Done! Continue to the next image.", w, h);
		startButton.draw();
		break;

	case 5: // show third image and track the gaze
		drawCurrentImage(elapsedS, numSecondsLooking);
		break;

	case 6: // complete text

		drawInstructionText("All done!", w, h);
		break;

	default:
		cout << "Invalid step!";
		break;
	}

	if (debug) {
		// draw gaze point 
		ofSetColor(255, 255, 255);
		ofDrawCircle(mEyeX.getGazePointData().X, mEyeX.getGazePointData().Y, 30);
		ofSetColor(255, 0, 0);
		ofDrawCircle(mEyeX.getFixationPoint(), 20);
		ofSetColor(255, 255, 255);
	}
	
}

void ofApp::drawInstructionText(string message, int width, int height) {

	int textX = width / 2 - font.stringWidth(message) / 2;
	int textY = height / 2 - 350;
	font.drawString(message, textX, textY);
}

// center the button and images 
void ofApp::resetElementsPos() {

	int h = ofGetWindowHeight();
	int w = ofGetWindowWidth();

	// set button position
	startButton.setPosition(w/2 - startButton.getWidth()/2 , h/2 - startButton.getHeight()/2);

	// set image position
	if (currentImage < numImages) {
		float ih = imgs[currentImage].getHeight();
		float iw = imgs[currentImage].getWidth();

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
}


// save the eye positions to json
void ofApp::recordGazeStream(ofPoint gaze, ofPoint fixation) {

	// calculate the number of ms since start 
	auto now = chrono::high_resolution_clock::now();
	auto dur = now - recordingStartTime;
	auto ms = chrono::duration_cast<chrono::milliseconds>(dur).count();

	// create an object to store the fixation point and time
	json gazeObj = json::object({ {"time", ms}, {"gazeX", gaze.x}, {"gazeY", gaze.y}, {"fixationX", fixation.x}, {"fixationY", fixation.y} });
	data.push_back(gazeObj);
}

void ofApp::saveGazeStreamToFile(ofImage img) {

	// calculate the number of ms since start 
	auto now = chrono::high_resolution_clock::now();
	auto dur = now - recordingStartTime;
	auto ms = chrono::duration_cast<chrono::milliseconds>(dur).count();

	// create an object to hold the general image info and json data collected
	// save the image's height, width, and position
	json completeObj = json::object({ { "h", img.getHeight() }, { "w", img.getWidth() }, {"x", imgX }, { "y", imgY }, { "totalTime", ms }, {"data", data } });

	// create the file name (time-currentStep) if it hasn't been created already
	if (filename.length() == 0) {
		auto tVal = chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
		long t = tVal.count();
		filename = to_string(t);
	}

	string path = "C:/Users/Christian Broms/Documents/EyeTracker2/EyeTracker2/bin/data/results/" + filename + "-" + to_string(currentImage + 1) + ".json";
	cout << path << endl;

	// save the data to a file
	ofstream o(path);
	o << setw(4) << completeObj << endl;
	o.close();

	// clear out the data array for the next image
	data = json::array();
}


void ofApp::keyPressed(int key)
{
	if (key == 'f') {
		ofToggleFullscreen();
		resetElementsPos();
	}

	if (key == 'd') {
		debug = !debug;
	}
}

void ofApp::exit() {

	startButton.removeListener(this, &ofApp::startPressed);
	mEyeX.unregisterGazePointEventHandler();
	mEyeX.unregisterEyePositionEventHandler();
	mEyeX.unregisterFixationEventHandler();
	mEyeX.close();
}