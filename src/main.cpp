#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1280 , 1280 * .75f , OF_WINDOW);
	ofRunApp(new testApp());
}
