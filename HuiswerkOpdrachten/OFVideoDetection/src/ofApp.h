#pragma once

#include "ofMain.h"
#include "ofxOpenCV.h"
#include "ofxCV.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
        ofImage image;
        ofVideoGrabber grabber;
        ofxCvHaarFinder haarFinder;
    
};
