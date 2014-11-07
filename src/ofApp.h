#pragma once

#include "ofMain.h"

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
        ofVec3f mapBands(int band, int totalBands, int scanline, int scanlines);
        
        ofEasyCam easyCam;
        ofShader shader;

        ofSoundPlayer music;
        int nBandsToGet;
        float * fft;
};
