#pragma once

#include "ofMain.h"

#define CAM_WIDTH 1024
#define CAM_HEIGHT 768


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
    private:
        ofVideoGrabber grabber;
    
        ofPixels workspace;
        ofImage image;

};
