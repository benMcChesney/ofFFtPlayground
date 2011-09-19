#include "testApp.h"

#include "ofxSimpleGuiToo.h"


void testApp::setup()
{
	plotHeight = 45;
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

    //get number of bins
    const int len = fft->getBinSize() ;

    //Controls
    
    //set a default FftRange
    FftRange r = FftRange( 0, 256, 256 );

    for ( int i = 0 ; i < 4 ; i++ )
    {
        //Generate a random color if none is found in XML
        ofColor color ;
        color.setHex( ofRandom( 0x111111 , 0xFFFFFFF ) ) ;
        visuals.push_back( RadialFft( 50.0f, 300.0f, 6.0f, ofVec2f( ofGetWidth() /2 , ofGetHeight()/2 ) , r ,color ) ) ;
    }

    /*
     This uses Memo Akten's ( https://github.com/memo , http://memo.tv )  ofxSimpleGuiToo
     I really like how easy it is to use and that it automatically saves all the values to an XML file
     
     Below we create a GUI tool page for each of our visulizers so we can tweak parameters
    */
    
    for ( int i = 0 ; i < visuals.size() ; i++ )
    {
        //Set the page name + xml name
        gui.addPage("Visual"+ofToString(i)).setXMLName("visual"+ofToString(i)+".xml" ) ;
        
        //Syntax for adding a float slider is:
        //gui.addSlider ( string title , float targetFloat , float minValue, float maxValue ) ; 
        gui.addTitle("FftRange");
        gui.addSlider("range_start",visuals[i].range.startIndex , 0 , len );
        gui.addSlider("range_end", visuals[i].range.endIndex , 1 , len-1 );

        gui.addTitle( "RadialBar Settings") ;
        gui.addSlider ( "centerOffset" , visuals[i].globalCenterOffset , 0.0f , 250.0f ) ;
        gui.addSlider ( "extrustionScale" , visuals[i].globalExtrusion , 1.0f , 1200.0f ) ;
        gui.addSlider ( "maxExtrustion" , visuals[i].range.maxExtrusion , 20.0f , 300.0f ) ;
        gui.addSlider ( "barWidth" , visuals[i].globalBarWidth , 1.0f , 20.0f ) ;

        //Create a new column
        gui.addTitle( "Position + Color" ).setNewColumn(true);
        gui.addSlider ( "x" , visuals[i].position.x , 0 , ofGetWidth() ) ;
        gui.addSlider ( "y" , visuals[i].position.y , 0 , ofGetHeight() ) ;
        
        //We can also add toggles
        //gui.addToggle ( string title , bool targetBool ) ; 
        gui.addToggle ( "Fill" , visuals[i].doFill ) ;
        gui.addToggle ( "drawBothSides" , visuals[i].drawBothSides ) ;
        
        //gui.addColorPicker("BG Color", &aColor.r);
        //I couldn't quite get the colorPicker to work, below is a workaround
        gui.addTitle ( "Color" ) ;
        gui.addSlider ( "red" , visuals[i].red , 0 , 255 ) ;
        gui.addSlider ( "green" , visuals[i].green, 0 , 255 ) ;
        gui.addSlider ( "blue" , visuals[i].blue , 0 , 255 ) ;

        //This didn't work so well, produced rainbow colors when dragged due to rawHex changes
        //gui.addSlider ( "hexColor" , visuals[i].hexColor , 0x111111 , 0xFFFFFF ) ;
    }
    
    gui.loadFromXML();
    gui.show() ;

    ofSetFrameRate( 60 ) ;
    ofSetCircleResolution( 180 ) ;
}


void testApp::update()
{
    //Get amplitudes and pass them on to visuals
    float * amplitudes = fft->getAmplitude() ;

    for ( int i = 0 ; i < visuals.size() ; i++ )
    {
        visuals[i].update( amplitudes ) ;
    }
}


void testApp::draw()
{
    int graphYOffset = 10 ;
	ofSetHexColor(0xffffff);
	ofPushMatrix();
	ofTranslate( ( ofGetWidth() +- bufferSize )/2 , ofGetHeight() + plotHeight * -3 + -3 * graphYOffset );
	ofDrawBitmapString("Time Domain", 0, 0);
	plot(audioInput, bufferSize, plotHeight / 2, 0);
	ofTranslate(0, plotHeight + graphYOffset );
	ofDrawBitmapString("Frequency Domain", 0, 0);

	//Draw Range
	const float binLength = (float)(fft->getBinSize()) ;
	ofEnableAlphaBlending() ;
	ofSetColor ( 255 , 0 , 0 , 125 ) ;
	ofFill() ;

    //for int i = 0 , for each range
    float alpha = 255.0f / (float)visuals.size()  ;
    float ySpacing = plotHeight / (float)visuals.size() ;
    for ( int i = 0 ; i < visuals.size() ; i++ )
    {
        ofSetColor ( visuals[i].red , visuals[i].green , visuals[i].blue , 255 ) ;
        ofRect ( (float)visuals[i].range.startIndex/binLength * bufferSize , ySpacing * i , (float)visuals[i].range.numIndicies/binLength * bufferSize , ySpacing ) ;
    }

	ofDisableAlphaBlending() ;

	ofSetColor ( 255 , 255 , 255 ) ;
	ofFill() ;
	fft->draw(0, 0, fft->getSignalSize(), plotHeight);
	ofTranslate( 0 ,  plotHeight + graphYOffset );
	spectrogram.update();
	spectrogram.draw(0, 0, bufferSize , plotHeight );
	ofPopMatrix();
	string msg = ofToString((int) ofGetFrameRate()) + " fps";

    ofSetColor ( 255.0f , 255.0f , 255.0f ) ;

    ofEnableSmoothing() ;

    for ( int i = 0 ; i < visuals.size() ; i++ )
    {
        visuals[i].draw( ) ;
    }

    ofSetColor ( 255 , 255 , 255 )  ;
	gui.draw() ;
    ofPushMatrix() ; 
        ofTranslate( 15 , ofGetHeight() +- 75 ) ; 
        ofDrawBitmapString( "G : Toggle GUI " , 0 , 0 ) ; 
        ofTranslate( 0 , 15 ) ; 
        ofDrawBitmapString( "A , M , N , S change the FFT Input " , 0 , 0 ) ; 
        string curInput ;
        if (mode == MIC) {	curInput = "Microphone" ; }
        else if (mode == NOISE) { curInput = "Noise" ; }
        else if (mode == SINE) { curInput = "Mouse" ;}
        else if ( mode == ANIMATE ) {  curInput = "Time" ; }
        ofTranslate( 0 , 15 ) ; 
        ofDrawBitmapString( "Current Input is: " + curInput , 0 , 0 ) ; 
    ofPopMatrix() ; 
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
	} else if ( mode == ANIMATE ) {
        for (int i = 0; i < bufferSize; i++)
			audioInput[i] = sinf(PI * i * ( sin( ofGetElapsedTimef() * .5f ) * appWidth )    / appWidth);
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

    if(key>='0' && key<='9')
    {
		gui.setPage(key - '0');
		gui.show();
	}
	else
	{
        switch (key)
        {
            //switching pages
            case ' ': gui.toggleDraw(); break;
            case '[': gui.prevPage(); break;
            case ']': gui.nextPage(); break;
            case 'p': gui.nextPageWithBlank(); break;

            //FFT Mode
            case 'm': mode = MIC; break;
            case 'n': mode = NOISE; break;
            case 's': mode = SINE; break;
            case 'a': mode = ANIMATE ; break ;

            case 'g' :
            case 'G' : gui.toggleDraw() ; break ;
        }
	}
}
