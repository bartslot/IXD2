#include "ofApp.h"

void ofApp::setup(){
    grabber.setup(GRABBER_WIDTH, GRABBER_HEIGHT);
    
    rgbImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
    hsvImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
    hueImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
    satImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
    valImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
    filtered.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);


}

void ofApp::update(){
    grabber.update();
    if (grabber.isFrameNew()) {
        rgbImage.setFromPixels(grabber.getPixels());
        rgbImage.mirror(false,true);
        
        hsvImage = rgbImage;
        hsvImage.convertRgbToHsv();
        hsvImage.convertToGrayscalePlanarImages(hueImage, satImage,valImage);
        
        for (int i= 1; i <  GRABBER_HEIGHT*GRABBER_WIDTH; i++){
            if (ofInRange(hueImage.getPixels()[i],
                          selectedHue - HUE_MARGIN, selectedHue + HUE_MARGIN)) {
                filtered.getPixels()[i] = 255;
            } else {
                filtered.getPixels()[i] = 0;
            }
        }
        filtered.flagImageChanged();
        
        contours.findContours(filtered, 50, GRABBER_WIDTH * GRABBER_HEIGHT / 2, 1, false);
    }
}

void ofApp::draw(){
    rgbImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    hsvImage.draw(0, 0);
//    hueImage.draw(0, GRABBER_HEIGHT);
//    valImage.draw(GRABBER_WIDTH, GRABBER_HEIGHT);
//    satImage.draw(GRABBER_WIDTH * 2, GRABBER_HEIGHT);
    filtered.draw(0, GRABBER_HEIGHT);
    
    contours.draw(0,0, ofGetWidth(), ofGetHeight());
    
    ofSetColor(ofColor::blue, 100);
    for (int i = 0 ; i < contours.blobs.size(); i++) {
        ofDrawCircle(contours.blobs[i].centroid.x * ofGetWidth()/GRABBER_WIDTH,
                     contours.blobs[i].centroid.y * ofGetHeight()/GRABBER_HEIGHT,
                     20);
    }
    ofSetColor(ofColor::white);
}

void ofApp::keyPressed(int key){

}


void ofApp::mousePressed(int x, int y, int button) {
    selectedHue = hueImage.getPixels()[y * GRABBER_WIDTH + x];
    cout << "Selected: " << selectedHue << endl;
}

