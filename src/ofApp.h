#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void exit();

	    void drawPointCloud();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        
        ofxKinect kinect;

	    ofxCvGrayscaleImage depthImage; // grayscale depth image
	    ofxCvGrayscaleImage depthThreshNear; // the near thresholded image
	    ofxCvGrayscaleImage depthThreshFar; // the far thresholded image

        int kinectAnglePitch;
        float cameraAnglePitch;
        float cameraAngleYaw;
        float cameraZoom;

        int nearThreshold;
        int farThreshold;

        int lastMouseX;
        int lastMouseY;

        // used for viewing the point cloud
        ofEasyCam easyCam;
        ofShader shader;

        ofFbo fbo1;
};
