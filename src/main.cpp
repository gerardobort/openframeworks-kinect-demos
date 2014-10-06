#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofGLProgrammableRenderer.h"

//========================================================================
int main() {
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp(new ofApp());
}
