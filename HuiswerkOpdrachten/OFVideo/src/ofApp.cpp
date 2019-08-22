#include "ofApp.h"

void ofApp::setup(){
    grabber.listDevices();
    
    grabber.setDeviceID(0);
    grabber.initGrabber(CAM_WIDTH, CAM_HEIGHT);
    grabber.setDesiredFrameRate(30);
    
    workspace.allocate(CAM_WIDTH, CAM_HEIGHT, OF_IMAGE_COLOR);
    
}

void ofApp::update(){
    grabber.update();
    
    if(grabber.isFrameNew()){
        ofPixels& pixels = grabber.getPixels();
        for (int y = 0; y < pixels.getHeight(); y++) {
            for (int x = 0; x < pixels.getWidth(); x++){
                ofColor col = pixels.getColor(x,y);
                col.r = 100 - col.r;
                col.g = 100 - col.r;
                col.b = 60 - col.r;
                workspace.setColor(x,y,col);
                workspace.setColor(pixels.getWidth() - x, y, col);
//                workspace.setColor(pixels.getHeight() - x, y, col);

            }
        }
        image.setFromPixels(workspace);
//        image.mirror(false, false);
    }
}

void ofApp::draw(){
//    grabber.draw(0,0);
    image.draw(25,25);
}

void ofApp::keyPressed(int key){

}


