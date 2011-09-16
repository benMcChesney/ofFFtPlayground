#include "testApp.h"

#include "ofxSimpleGuiToo.h"


void testApp::setup() {
	plotHeight = 65;
	bufferSize = 512;

    drawBothSides = false ; 
	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
	// To use FFTW, try:
	// fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);

	spectrogram.allocate(bufferSize, fft->getBinSize(), OF_IMAGE_GRAYSCALE);
	memset(spectrogram.getPixels(), 0, (int) (spectrogram.getWidth() * spectrogram.getHeight()) );
	spectrogramOffset = 0;

	audioInput = new float[bufferSize];
	memset(audioInput, 0, sizeof(float) * bufferSize);

	// 0 output channels,
	// 1 input channel
	// 44100 samples per second
	// [bins] samples per buffer
	// 4 num buffers (latency)

	ofSoundStreamSetup(0, 1, this, 44100, bufferSize, 4);

	mode = SINE;
	appWidth = ofGetWidth();
	appHeight = ofGetHeight();

	ofBackground(0, 0, 0);

    const int len = fft->getBinSize() ;

    //Gui Stuff
    FftRange r = FftRange( 0, 256, 256 ); 
    treble = RadialFft( 50.0f, 300.0f, 6.0f, ofVec2f( ofGetWidth() /2 , ofGetHeight()/2 ) , r ) ;
    
    gui.addTitle("FftRange");
    //gui.addPage("Custom Page").setXMLName("settings.xml");
    gui.addFPSCounter();
    //gui.addSlider("myInt2", myInt2, 3, 8);
	gui.addSlider("range_start", treble.range.startIndex , 0 , len );
	gui.addSlider("range_end", treble.range.endIndex , 1 , len-1 );


	  ofVec3f position ;          //position
    float theta ;               //angle from center
    float startDistance ;       //distance to start drawing from center
    float extrusion ;

    //globalCenterOffset = radius ;
    //globalExtrusion = 50.0f ;
	gui.addTitle( "RadialBar Settings") ;
	gui.addSlider ( "centerOffset" , treble.globalCenterOffset , 0.0f , 250.0f ) ;
	gui.addSlider ( "extrustionScale" , treble.globalExtrusion , 1.0f , 1200.0f ) ;
	gui.addSlider ( "maxExtrustion" , treble.range.maxExtrusion , 20.0f , 300.0f ) ;
	gui.addSlider ( "barWidth" , treble.globalBarWidth , 1.0f , 20.0f ) ;
    gui.addToggle ( "drawBothSides" , drawBothSides ) ; 
	gui.loadFromXML();
	gui.show() ;

    ofSetFrameRate( 60 ) ; 
}


void testApp::update()
{
    float * amplitudes = fft->getAmplitude() ;
    treble.update( amplitudes ) ; 
}


void testApp::draw() 
{
	ofSetHexColor(0xffffff);
	ofPushMatrix();
	ofTranslate(35, ofGetHeight() + plotHeight * -2.75 );
	ofDrawBitmapString("Time Domain", 0, 0);
	plot(audioInput, bufferSize, plotHeight / 2, 0);
	ofTranslate(0, plotHeight + 16);
	ofDrawBitmapString("Frequency Domain", 0, 0);

	//Draw Range
	const float binLength = (float)(fft->getBinSize()) ;
	ofEnableAlphaBlending() ;
	ofSetColor ( 255 , 0 , 0 , 125 ) ;
	ofFill() ;

    //for int i = 0 , for each range
	ofRect ( (float)treble.range.startIndex/binLength * bufferSize , (float)treble.range.numIndicies/binLength * bufferSize , plotHeight) ;
	ofDisableAlphaBlending() ;

	ofSetColor ( 255 , 255 , 255 ) ;

	//ofNoFill() ;
	ofFill() ;
	fft->draw(0, 0, fft->getSignalSize(), plotHeight);
	ofTranslate( bufferSize + 5 , -plotHeight - 16 );
	spectrogram.update();
	spectrogram.draw(0, 0, bufferSize , plotHeight );
	ofPopMatrix();
	string msg = ofToString((int) ofGetFrameRate()) + " fps";

    ofSetColor ( 255.0f , 255.0f , 255.0f ) ;

    ofEnableSmoothing() ;
    treble.draw( drawBothSides ) ; 

    //ofFill() ;
    ofSetColor ( 255 , 255 , 255 )  ;
	gui.draw() ;
}


void testApp::plot(float* array, int length, float scale, float offset) {
	ofNoFill();
	ofRect(0, 0, length, plotHeight);
	glPushMatrix();
	glTranslatef(0, plotHeight / 2 + offset, 0);
	ofBeginShape();
	for (int i = 0; i < length; i++)
		ofVertex(i, array[i] * scale);
	ofEndShape();
	glPopMatrix();
}

void testApp::audioReceived(float* input, int bufferSize, int nChannels) {
	if (mode == MIC) {
		// store input in audioInput buffer
		memcpy(audioInput, input, sizeof(float) * bufferSize);
	} else if (mode == NOISE) {
		for (int i = 0; i < bufferSize; i++)
			audioInput[i] = ofRandom(-1, 1);
	} else if (mode == SINE) {
		for (int i = 0; i < bufferSize; i++)
			audioInput[i] = sinf(PI * i * mouseX / appWidth);
	}

	fft->setSignal(audioInput);

	float* curFft = fft->getAmplitude();
	int spectrogramWidth = (int) spectrogram.getWidth();
	unsigned char* pixels = spectrogram.getPixels();
	for(int i = 0; i < fft->getBinSize(); i++)
		pixels[i * spectrogramWidth + spectrogramOffset] = (unsigned char) (255. * curFft[i]);
	spectrogramOffset = (spectrogramOffset + 1) % spectrogramWidth;
}

void testApp::keyPressed(int key) {
	switch (key) {
	case 'm':
		mode = MIC;
		break;
	case 'n':
		mode = NOISE;
		break;
	case 's':
		mode = SINE;
		break;

    case 'g' :
    case 'G' :
        gui.toggleDraw() ;
        break ;
	}
}
