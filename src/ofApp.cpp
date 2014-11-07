#include "ofApp.h"
#include <math.h>
#define M_PI 3.1415926535897932384626433832795

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    music.loadSound("media/perfume-point.mp3");
    music.setLoop(true);
    music.play();
    music.setVolume(1.0);
    nBandsToGet = 1024;

    shader.load("gl3/massglow/shader");

    easyCam.setGlobalPosition(ofVec3f(0.0, 0.0, 1000.0));
    easyCam.setDistance(-200);
    easyCam.setTarget(ofVec3f(0.0, 0.0, 0.0));
}

//--------------------------------------------------------------
void ofApp::update(){

    ofSoundUpdate();
    fft = ofSoundGetSpectrum(nBandsToGet);

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofColor centerColor(100, 100, 100);
    ofColor edgeColor(40, 40, 40);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);

    ofEnableDepthTest();

    drawPointCloud();

	ofSetColor(0, 255, 0);
	stringstream reportStream;
	reportStream
        << "-----------------------------------------------" << endl
        << "gerardobort@gmail.com - music demo - " << ofGetElapsedTimef() << endl
        << "-----------------------------------------------" << endl
        << "spectrum [0 - 7]   " << fft[0] << endl
        << "                   " << fft[1] << endl
        << "                   " << fft[2] << endl
        << "                   " << fft[3] << endl
        << "                   " << fft[4] << endl
        << "                   " << fft[5] << endl
        << "                   " << fft[6] << endl
        << "                   " << fft[7] << endl
        << "-----------------------------------" << endl;
	ofDrawBitmapString(reportStream.str(), 20, 20);
}

//--------------------------------------------------------------
ofVec3f ofApp::mapBands(int band, int totalBands, int scanline, int scanlines) {
    ofVec3f v3;
    float scale = 200.0;
    float intensity = fft[band];
    float phi = ((float)band/totalBands) * 2.0 * M_PI;
    float theta = ((float)scanline/scanlines) * 1.0 * M_PI - M_PI/2.0;
    float rho = scale * (0.4 + 3*intensity);
/*
    if (!fmod((float)scanline, 5)) {
        phi += M_PI;
        theta -= M_PI;
    } else if (!fmod((float)scanline, 4)) {
        phi += 0;
        theta += 0;
    } else if (!fmod((float)scanline, 3)) {
        phi += 0;
        theta -= M_PI;
    } else if (!fmod((float)scanline, 2)) {
        phi -= M_PI;
        theta += M_PI;
    }
*/

    v3 = ofVec3f(
        rho*cos(phi)*cos(theta),
        rho*sin(phi)*cos(theta),
        rho*sin(theta)
    );

    return v3;
}
void ofApp::drawPointCloud() {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    float intensity;
    int scanlines = 60;
    for (int i = 0; i < nBandsToGet; i++) {
        intensity = fft[i];
        for (int j = 0; j < 180; j++) {
            ofColor c1 = ofColor(255*((i/nBandsToGet) + (0.5 + intensity)), 255*i/nBandsToGet, 255*j/scanlines, 255);
            mesh.addColor(c1);
            mesh.addVertex(mapBands(i, nBandsToGet, j, scanlines));
        }
    }


    easyCam.begin();
        //shader.begin();
            //glPointSize(4);
            mesh.drawWireframe();
        //shader.end();
    easyCam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case OF_KEY_LEFT:
            easyCam.setDistance(easyCam.getDistance() + 20);
			break;

		case OF_KEY_RIGHT:
            easyCam.setDistance(easyCam.getDistance() - 20);
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit() {
    music.unloadSound();
}
