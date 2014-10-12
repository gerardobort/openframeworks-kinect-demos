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

	nearThreshold = 850;
	farThreshold = 1800;

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
        //depthThreshNear.threshold(farThreshold, true);
        //depthThreshFar.threshold(nearThreshold);
        //cvAnd(depthThreshNear.getCvImage(), depthThreshFar.getCvImage(), depthImage.getCvImage(), NULL);

        depthImage.flagImageChanged();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofEnableDepthTest();

    //depthImage.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    drawPointCloud();

    /*
	ofSetColor(0, 255, 0);
	stringstream reportStream;
	reportStream
        << "-----------------------------------------------" << endl
        << "gerardobort@gmail.com - kinect demo - " << ofGetElapsedTimef() << endl
        << "-----------------------------------------------" << endl
        << "kinectAnglePitch " << kinectAnglePitch << endl
        << "-----------------------------------" << endl
        << "cameraZoom " << cameraZoom << endl
        << "cameraAngleYaw " << cameraAngleYaw << endl
        << "cameraAnglePitch " << cameraAnglePitch << endl
        << "-----------------------------------" << endl
        << "farThreshold " << farThreshold << endl
        << "nearThreshold " << nearThreshold << endl
        << "-----------------------------------" << endl;
	ofDrawBitmapString(reportStream.str(), 20, 20);
    */
}

//--------------------------------------------------------------
void ofApp::drawPointCloud() {
    int w = kinect.width;
    int h = kinect.height;

    shader.begin();
    shader.setUniform1i("u_worldWidth", ofGetWindowWidth());
    shader.setUniform1i("u_worldHeight", ofGetWindowHeight());
    shader.setUniform1i("u_farThreshold", farThreshold);
    shader.setUniform1f("u_time", ofGetElapsedTimef());
    shader.end();

    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    int step = 6;
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
    glPointSize(2);
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofRotateX(cameraAnglePitch);
    ofRotateY(cameraAngleYaw);
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2, cameraZoom); // center the points a bit
    ofScale(1, 1, -1);
    glEnable(GL_DEPTH_TEST);
    mesh.drawFaces();
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
