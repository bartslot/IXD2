#include "ofApp.h"
#define GRABBER_WIDTH 1280
#define GRABBER_HEIGHT 720

void ofApp::setup(){
    grabber.setup(GRABBER_WIDTH, GRABBER_HEIGHT);
    
    haarFinder.setup("haarcascade_eyes.xml");
    
    //Get yourself some girly eyes
    image.load("eyes.png");
}

void ofApp::update(){
    grabber.update();
    
    //new frame
    if (grabber.isFrameNew()){
        haarFinder.findHaarObjects(grabber.getPixels(), 75, 75);
    }
}
    //Drawing the eyes
void ofApp::draw(){
    ofSetColor(ofColor:: white);
    grabber.draw(0, 0);

    ofNoFill();
    ofSetColor(ofColor:: white);
    for (int i = 0;  i < haarFinder.blobs.size(); i++) {
        ofRectangle boundingBox = haarFinder.blobs[i].boundingRect;
        ofDrawRectangle(boundingBox);
    
        image.draw(boundingBox);
    }
}

void ofApp::keyPressed(int key){

}
