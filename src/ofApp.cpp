#include "ofApp.h"
#include <string> 


void ofApp::setup() {
	ofSetCircleResolution(60);

	// image setup 
	img1.load("images/one.jpg");
	imgWidth = img1.getWidth();
	imgHeight = img1.getHeight();

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
}


void ofApp::startPressed() {
	cout << "Button pressed\n";
}


void ofApp::update() {

	mEyeX.update();
	
}


void ofApp::draw() {
	ofBackground(30);

	img1.draw(0, 0, imgWidth, imgHeight);

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
	ofSetColor(255, 255, 255);
	ofDrawCircle(mEyeX.getGazePointData().X, mEyeX.getGazePointData().Y, 20);

	

	// Draw fixation
	switch (mEyeX.getFixationData().FixationDataMode)
	{
	case TX_FIXATIONDATAEVENTTYPE_BEGIN:
		ofSetColor(255, 0, 0);
		break;
	case TX_FIXATIONDATAEVENTTYPE_END:
		ofSetColor(0, 0, 255);
		break;
	case TX_FIXATIONDATAEVENTTYPE_DATA:
		ofSetColor(255, 0, 255);
		break;
	}
	ofNoFill();
	ofDrawCircle(mEyeX.getFixationPoint(), 25);

	/*
	stringstream ss;
	ss << "Output information about your eyes" << endl;
	ss << "- Left Eye Position  : " << tx.getLeftEyePosition() << endl;
	ss << "- Right Eye Position : " << tx.getRightEyePosition() << endl;
	ss << "Output information about your eyes (Normalized)" << endl;
	ss << "- Left Eye Position  : " << tx.getLeftEyePosNorm() << endl;
	ss << "- Right Eye Position : " << tx.getRightEyePosNorm() << endl;
	ss << "Output information aboud your gaze position" << endl;
	ss << "- Gaze position: " << tx.getGaze() << endl;
	ofSetColor(255);
	ofDrawBitmapString(ss.str(), 20, 20);
	*/
}

// center the button and images 
void ofApp::resetElementsPos() {

	int h = ofGetWindowHeight();
	int w = ofGetWindowWidth();

	// set button position
	startButton.setPosition(w/2 - startButton.getWidth()/2 , h/2 - startButton.getHeight()/2);

	// set image position
	int ih = img1.getHeight();
	/*ofPixels pix = img1.getPixels();
	cout << std::to_string(pix.getHeight()) << " " << std::to_string(pix.getWidth()) << "\n";*/
	
	int iw = img1.getWidth();

	int ihr = ih / iw;
	int iwr = iw / ih;

	if (ihr > iwr) {
		// vertical image
		// get scale factor 
		// this isn't working ATM
		int scale = h / ih;
		int newW = iw * scale;
		imgHeight = h;
		imgWidth = newW;
	}
	else {
		// horizontal image
	}

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