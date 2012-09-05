#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1200 , 675 , OF_WINDOW);
	ofRunApp(new testApp());
}
