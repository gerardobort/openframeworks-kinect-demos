#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofGLProgrammableRenderer.h"

//========================================================================
int main() {
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, ofGetWindowWidth(), ofGetWindowHeight(), OF_WINDOW);
	ofRunApp(new ofApp());
}
