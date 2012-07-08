#pragma once

#include "ofMain.h"
#include "ofxFft.h"

#define MIC 0
#define NOISE 1
#define SINE 2

class testApp : public ofBaseApp {
public:
	void setup();
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
};
