#pragma once

#include "ofMain.h"
#include "ofxFft.h"
#include "RadialBar.h"
#include "FftRange.h"
#include "RadialFft.h"

#define MIC 0
#define NOISE 1
#define SINE 2

class testApp : public ofBaseApp {
public:
	void setup();
    void update() ;
	void plot(float* array, int length, float scale, float offset);
	void audioReceived(float* input, int bufferSize, int nChannels);
	void draw();
	void keyPressed(int key);

	int plotHeight, bufferSize;

	ofxFft* fft;

	float* audioInput;

	float appWidth;
	float appHeight;

	ofImage spectrogram;
	int spectrogramOffset;

	int mode;

//     vector<RadialBar> points;
//     FftRange range ;


//     float globalCenterOffset ;
//     float globalExtrusion ;
//     float globalBarWidth ;
    
     RadialFft treble ; 
    
     bool drawBothSides ; 


};
