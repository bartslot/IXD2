#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#define GRABBER_WIDTH 1280
#define GRABBER_HEIGHT 720

#define HUE_MARGIN 5

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
        void mousePressed(int x, int y, int button);
    
        ofVideoGrabber grabber;

        ofxCvColorImage rgbImage;
    
        ofxCvColorImage hsvImage;
    
        ofxCvGrayscaleImage hueImage;
        ofxCvGrayscaleImage satImage;
        ofxCvGrayscaleImage valImage;
    
        ofxCvGrayscaleImage filtered;
    
        int selectedHue;
    
        ofxCvContourFinder contours;
    
};
