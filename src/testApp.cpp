#include "testApp.h"

void testApp::setup() {
	plotHeight = 128;
	bufferSize = 512;

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
    
    
    float barLength = 600.0f ; 
    float barOffset = ( ofGetWidth() +- barLength ) / 2 ;
    float xIncrement = barLength / (float) len ; 
    
    float radius = 65.0f ; 
    ofVec2f offset = ofVec2f( ofGetWidth() /2 , ofGetHeight() /2 ) ; 
    float thetaStep = TWO_PI / (float) len ; 
    
    
    //p.x = Math.cos(i * anglePer) * 550;
    //p.z = Math.sin(i * anglePer) * 550;
    
    
    for ( int i = 0 ; i < len ; i++ ) 
    {
        
        float theta = thetaStep * (float) i ; 
        ofVec3f newPoint = ofVec3f ( cos( theta ) * radius , sin ( theta ) * radius , 0 ) ; 
        RadialBar rb = RadialBar() ; 
        rb.position = newPoint ; 
        rb.theta = theta ; 
        rb.startDistance = radius ; 
        rb.extrusion = 50.0f ; 
        points.push_back( rb ) ; 
        // points.push_back ( ofVec3f( barOffset + xIncrement * i , 0 , 0 ) ) ; 
    }

}


void testApp::draw() {
	ofSetColor(0xffffff);
	ofPushMatrix();
	ofTranslate(16, 16);
	ofDrawBitmapString("Time Domain", 0, 0);
	plot(audioInput, bufferSize, plotHeight / 2, 0);
	ofTranslate(0, plotHeight + 16);
	ofDrawBitmapString("Frequency Domain", 0, 0);
	fft->draw(0, 0, fft->getSignalSize(), plotHeight);
	ofTranslate(0, plotHeight + 16);
	spectrogram.update();
	spectrogram.draw(0, 0);
	ofPopMatrix();
	string msg = ofToString((int) ofGetFrameRate()) + " fps";
    
    const int len = points.size() ; 
    ofSetColor ( 255.0f , 255.0f , 255.0f ) ; 
    
    ofEnableSmoothing() ; 
    
    
    
    for ( int i = 0 ; i < len ; i++ ) 
    {
        ofPushMatrix() ; 
        ofTranslate( ofGetWidth() / 2 + points[i].position.x , ofGetHeight() * .75 +  points[i].position.y ) ; 
        ofRotate( ofRadToDeg( points[i].theta ) - 90  ) ; 
        ofRect (  0 , 0 , 2.0f ,  points[i].extrusion ) ; 
        ofPopMatrix() ; 
    }   
    
    ofDrawBitmapString ( ofToString( (int) ofGetFrameRate() ) , 15, ofGetHeight() +- 35 ) ; 
    
	ofDrawBitmapString(msg, appWidth - 80, appHeight - 20);
}

void testApp::update(){
    
  //  const int len = points.size() ; 
    
    float * amplitudes = fft->getAmplitude() ;
    const int len = fft->getBinSize() ; 
    
	for (int i = 0; i < len ; i++)
    {
		//ofVertex(i, amplitudes[i]);
        points[i].extrusion = ofNoise ( ofGetElapsedTimef() + (float)i ) * 35.0f + amplitudes[i] * 40.0f ; 
    }
    
    
//    for ( int i = 0 ; i < len ; i++ ) 
//    {
//        points[i].extrusion =  ofNoise ( ofGetElapsedTimef() + (float)i ) * 75.0f ;
  //  }
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
	}
}
