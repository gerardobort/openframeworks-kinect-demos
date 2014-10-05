#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

    kinect.setRegistration(true);
    kinect.init();
    kinect.open();        // opens first available kinect

    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }

    depthImage.allocate(kinect.width, kinect.height);
	depthThreshFar.allocate(kinect.width, kinect.height);

    ofSetFrameRate(60);

    // zero the tilt on startup
    kinectAnglePitch = 0;
    kinect.setCameraTiltAngle(kinectAnglePitch);

	nearThreshold = 1200;
	farThreshold = 450;

    cameraZoom = 1000;


#ifdef TARGET_OPENGLES
    shader.load("shadersES2/shader");
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("shadersGL3/shader");
    }else{
        shader.load("shadersGL2/shader");
    }
#endif
}

//--------------------------------------------------------------
void ofApp::update(){

	ofBackground(0, 0, 0);
	kinect.update();

	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {

		// load depthscale depth image from the kinect source
		depthImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);

        //depthThreshNear = depthImage;
        //depthThreshFar = depthImage;
        //depthThreshNear.threshold(nearThreshold, true);
        //depthThreshFar.threshold(farThreshold);
        //cvAnd(depthThreshNear.getCvImage(), depthThreshFar.getCvImage(), depthImage.getCvImage(), NULL);

        depthImage.flagImageChanged();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofEnableDepthTest();

    //depthImage.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    shader.begin();
    drawPointCloud();
    shader.end();

	ofSetColor(0, 255, 0);
	stringstream reportStream;
	reportStream
        << "-----------------------------------" << endl
        << "gerardobort@gmail.com - kinect demo" << endl
        << "-----------------------------------" << endl
        << "kinectAnglePitch " << kinectAnglePitch << endl
        << "-----------------------------------" << endl
        << "cameraZoom " << cameraZoom << endl
        << "cameraAngleYaw " << cameraAngleYaw << endl
        << "cameraAnglePitch " << cameraAnglePitch << endl
        << "-----------------------------------" << endl
        << "nearThreshold " << nearThreshold << endl
        << "farThreshold " << farThreshold << endl
        << "-----------------------------------" << endl;
	ofDrawBitmapString(reportStream.str(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::drawPointCloud() {
    int w = kinect.width;
    int h = kinect.height;

    ofMesh mesh;
    ofColor colorA, colorB;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    int step = 1;
    for (int y = 0; y+step < h; y += step) {
        for (int x = 0; x+step < w; x += step) {
            if (kinect.getDistanceAt(x, y) > 0) {
                float depth = kinect.getDistanceAt(x, y);
                if (nearThreshold > depth && depth > farThreshold) {
                    colorA = kinect.getColorAt(x,y);
                    colorB = ofColor(0, colorB.getBrightness(), 0, 255);
                    colorB.a = 255 - depth * (255.0 / nearThreshold);
                    mesh.addColor(colorB);
                    mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
                }
            }
        }
    }
    glPointSize(2);
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofRotateX(cameraAnglePitch);
    ofRotateY(cameraAngleYaw);
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2, cameraZoom); // center the points a bit
    ofScale(1, 1, -1);
    glEnable(GL_DEPTH_TEST);
    mesh.drawVertices();
    glDisable(GL_DEPTH_TEST);
    ofPopMatrix();
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
			cameraZoom-=10;
			break;

		case OF_KEY_RIGHT:
			cameraZoom+=10;
			break;

		case 'i':
			nearThreshold-=10;
            farThreshold-=10;
			break;

		case 'o':
			nearThreshold+=10;
            farThreshold+=10;
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
    cameraAngleYaw += (lastMouseX - x) * 0.5;
    cameraAnglePitch += (lastMouseY - y) * 0.5;
    lastMouseX = x;
    lastMouseY = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    lastMouseX = x;
    lastMouseY = y;
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
}
