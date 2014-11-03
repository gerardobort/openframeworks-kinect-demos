#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();

    ofSetFrameRate(60);

    // zero the tilt on startup
    kinectAnglePitch = 0;
    kinect.setCameraTiltAngle(kinectAnglePitch);

	nearThreshold = 400;
	farThreshold = 1000;

    shaderPlasma.load("gl3/plasma/shader");
    shaderSpectralBody.load("gl3/spectralbody/shader");

    easyCam.setGlobalPosition(ofVec3f(0.0, 100.0, 1500));
    easyCam.setTarget(ofVec3f(0.0, 0.0, 0.0));

    music.loadSound("media/perfume-point.mp3");
    music.setLoop(true);
    music.play();
    nBandsToGet = 8;
}

//--------------------------------------------------------------
void ofApp::update(){

	ofBackground(0, 0, 0);
	kinect.update();

    ofSoundUpdate();
    fft = ofSoundGetSpectrum(nBandsToGet);

	if(kinect.isFrameNew()) {
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofEnableDepthTest();

    drawPointCloud();

	ofSetColor(0, 255, 0);
	stringstream reportStream;
	reportStream
        << "-----------------------------------------------" << endl
        << "gerardobort@gmail.com - kinect demo - " << ofGetElapsedTimef() << endl
        << "-----------------------------------------------" << endl
        << "farThreshold " << farThreshold << endl
        << "nearThreshold " << nearThreshold << endl
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
void ofApp::drawPointCloud() {
    int w = kinect.width;
    int h = kinect.height;

    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    int step = 4;
    for (int y = 0; y+2*step < h; y += step) {
        for (int x = 0; x+2*step < w; x += step) {
            if (kinect.getDistanceAt(x, y) > 0) {
                ofVec3f v1 = kinect.getWorldCoordinateAt(x, y);
                ofVec3f v2 = kinect.getWorldCoordinateAt(x+step, y);
                ofVec3f v3 = kinect.getWorldCoordinateAt(x, y+step);
                ofVec3f v4 = kinect.getWorldCoordinateAt(x+step, y+step);

                ofColor c1 = kinect.getColorAt(x, y);
                ofColor c2 = kinect.getColorAt(x+step, y);
                ofColor c3 = kinect.getColorAt(x, y+step);
                ofColor c4 = kinect.getColorAt(x+step, y+step);

                if (farThreshold > v1.z && v1.z > nearThreshold) {
                    //if (v1.distance(v2) > 10*step || v1.distance(v3) > 10*step) {
                    //    v1.z = (v2.z + v3.z)/2;
                    //}
                    if (v1.distance(v2) > 10*step) continue;
                    if (v1.distance(v3) > 10*step) continue;
                    mesh.addColor(c1);
                    mesh.addVertex(v1);
                    mesh.addColor(c2);
                    mesh.addVertex(v2);
                    mesh.addColor(c3);
                    mesh.addVertex(v3);
                }
                if (farThreshold > v4.z && v4.z > nearThreshold) {
                    //if (v4.distance(v2) > 10*step || v4.distance(v3) > 10*step) {
                    //    v4.z = (v2.z + v3.z)/2;
                    //}
                    if (v4.distance(v2) > 10*step) continue;
                    if (v4.distance(v3) > 10*step) continue;
                    mesh.addColor(c2);
                    mesh.addVertex(v2);
                    mesh.addColor(c3);
                    mesh.addVertex(v3);
                    mesh.addColor(c4);
                    mesh.addVertex(v4);
                }
            }
        }
    }

    ofMesh mesh2;
    mesh2.setMode(OF_PRIMITIVE_POINTS);
    step = 2;
    for (int y = 0; y+2*step < h; y += step) {
        for (int x = 0; x+2*step < w; x += step) {
            if (kinect.getDistanceAt(x, y) > 0) {
                ofVec3f v1 = kinect.getWorldCoordinateAt(x, y);
                ofColor c1 = kinect.getColorAt(x, y);
                if (farThreshold > v1.z && v1.z > nearThreshold) {
                    mesh2.addColor(c1);
                    mesh2.addVertex(v1);
                }
            }
        }
    }


    easyCam.begin();
        ofScale(-1, -1, -1);
        ofTranslate(0, 0, -1000);

        shaderSpectralBody.begin();
            shaderSpectralBody.setUniform1i("u_worldWidth", ofGetWindowWidth());
            shaderSpectralBody.setUniform1i("u_worldHeight", ofGetWindowHeight());
            shaderSpectralBody.setUniform1i("u_mapWidth", kinect.width);
            shaderSpectralBody.setUniform1i("u_mapHeight", kinect.height);
            shaderSpectralBody.setUniform1i("u_farThreshold", farThreshold);
            shaderSpectralBody.setUniform1i("u_nearThreshold", nearThreshold);
            shaderSpectralBody.setUniform1f("u_time", ofGetElapsedTimef());
            shaderSpectralBody.setUniformTexture("u_sampler2d", kinect.getTextureReference(), kinect.getTextureReference().getTextureData().textureID);
            shaderSpectralBody.setUniform1f("u_musicSpectrumBand0", fft[0]);
            shaderSpectralBody.setUniform1f("u_musicSpectrumBand1", fft[1]);
            shaderSpectralBody.setUniform1f("u_musicSpectrumBand2", fft[2]);
            shaderSpectralBody.setUniform1f("u_musicSpectrumBand3", fft[3]);
            mesh.drawWireframe();
        shaderSpectralBody.end();

        shaderPlasma.begin();
            shaderPlasma.setUniform1i("u_worldWidth", ofGetWindowWidth());
            shaderPlasma.setUniform1i("u_worldHeight", ofGetWindowHeight());
            shaderPlasma.setUniform1i("u_mapWidth", kinect.width);
            shaderPlasma.setUniform1i("u_mapHeight", kinect.height);
            shaderPlasma.setUniform1i("u_farThreshold", farThreshold);
            shaderPlasma.setUniform1i("u_nearThreshold", nearThreshold);
            shaderPlasma.setUniform1f("u_time", ofGetElapsedTimef());
            shaderPlasma.setUniformTexture("u_sampler2d", kinect.getTextureReference(), kinect.getTextureReference().getTextureData().textureID);
            shaderPlasma.setUniform1f("u_musicSpectrumBand0", fft[4]);
            shaderPlasma.setUniform1f("u_musicSpectrumBand1", fft[5]);
            shaderPlasma.setUniform1f("u_musicSpectrumBand2", fft[6]);
            shaderPlasma.setUniform1f("u_musicSpectrumBand3", fft[7]);
            glPointSize(4);
            mesh2.drawVertices();
        shaderPlasma.end();
    easyCam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;

		case 'e':
			kinect.setCameraTiltAngle(kinectAnglePitch); // go back to prev tilt
			kinect.open();
			break;

		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;

		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;

		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;

		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;

		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;

		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;

		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;

		case OF_KEY_UP:
			kinectAnglePitch++;
			if(kinectAnglePitch>30) kinectAnglePitch=30;
			kinect.setCameraTiltAngle(kinectAnglePitch);
			break;

		case OF_KEY_DOWN:
			kinectAnglePitch--;
			if(kinectAnglePitch<-30) kinectAnglePitch=-30;
			kinect.setCameraTiltAngle(kinectAnglePitch);
			break;

		case OF_KEY_LEFT:
            easyCam.setDistance(easyCam.getDistance() + 20);
			break;

		case OF_KEY_RIGHT:
            easyCam.setDistance(easyCam.getDistance() - 20);
			break;

		case 'i':
			farThreshold-=10;
            nearThreshold-=10;
			break;

		case 'o':
			farThreshold+=10;
            nearThreshold+=10;
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
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
    music.unloadSound();
}
