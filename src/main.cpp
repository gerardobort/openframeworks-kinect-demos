#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
//#include "ofMesh.h"

//========================================================================
int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp(new ofApp());
}
