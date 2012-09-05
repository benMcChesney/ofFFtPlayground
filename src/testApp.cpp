#include "testApp.h"

//#include "ofxSimpleGuiToo.h"


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

   // for ( int i = 0 ; i < 4 ; i++ )
   // {
        //Generate a random color if none is found in XML
        ofColor color ;
        color.setHex( ofRandom( 0x111111 , 0xFFFFFFF ) ) ;
        //visuals.push_back( RadialFft( 50.0f, 300.0f, 6.0f, ofVec2f( ofGetWidth() /2 , ofGetHeight()/2 ) , r ,color ) ) ;
        radialFft = RadialFft( 50.0f, 300.0f, 6.0f, ofVec2f( ofGetWidth() /2 , ofGetHeight()/2 ) , r ,color ) ; 
   // }

    /*
     This uses Memo Akten's ( https://github.com/memo , http://memo.tv )  ofxSimpleGuiToo
     I really like how easy it is to use and that it automatically saves all the values to an XML file
     
     Below we create a GUI tool page for each of our visulizers so we can tweak parameters
    */
    
    
    //for ( int i = 0 ; i < visuals.size() ; i++ )
   // {
    //ofxUICanvas *fftGUI;
//	void fftGUIEvent(ofxUIEventArgs &e) ;
    float dim = 24;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 250-xInit;
    
        fftGUI = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());
        
        fftGUI->addWidgetDown(new ofxUILabel("FFT PARAMS", OFX_UI_FONT_LARGE));
        //fftGUI->addRangeSlider("RED", 0.0, 1 , len -1 , 0 , len , length-xInit,dim);
    
    ////gui.addSlider ( string title , float targetFloat , float minValue, float maxValue ) ;
    //    fftGUI->addWidgetDown(new ofxUIRangeSlider(length-xInit,dim, 0.0, len-2 , 0 , len-1 , "MAX RANGE"));
        fftGUI->addWidgetRight(new ofxUIRangeSlider(length-xInit,dim, 0.0, len-2 , 0 , len-1 , "FFT RANGE"));
        fftGUI->addWidgetRight(new ofxUISlider(length-xInit,dim , 0.0f , 450.0f , radialFft.globalCenterOffset , "CENTER OFFSET" ) ) ;
        fftGUI->addWidgetDown(new ofxUISlider(length-xInit,dim , 1.0f , 1200.0f , radialFft.globalExtrusion , "GLOBAL EXTRUSION" ) ) ;
        fftGUI->addWidgetRight(new ofxUISlider(length-xInit,dim , 20.0f , 300.0f , radialFft.range.maxExtrusion , "MAX EXTRUSION" ) ) ;
        fftGUI->addWidgetDown(new ofxUISlider(length-xInit,dim , 1.0f , 40.0f , radialFft.globalBarWidth , "BAR WIDTH" ) ) ;
    
        fftGUI->addWidgetDown(new ofxUI2DPad(length-xInit,120, ofPoint( 0 , ofGetWidth() ) , ofPoint ( 0 , ofGetHeight() )  , radialFft.position  , "X + Y"));
   
        fftGUI->addWidgetRight(new ofxUIToggle( 150 , 50 , radialFft.doFill , "DRAW FILL" ) ) ;
        fftGUI->addWidgetRight(new ofxUIToggle( 150 , 50 , radialFft.drawBothSides , "DRAW BOTH SIDES" ) ) ;
    
        fftGUI->addWidgetDown(new ofxUISlider(length-xInit,dim , 0.1f , 15.0f , radialFft.noiseTimeMultiplier , "NOISE TIME MULTIPLIER" ) ) ;
        fftGUI->addWidgetDown(new ofxUISlider(length-xInit,dim , 0.0f , 400.0f , radialFft.noiseStrength , "NOISE STRENGTH" ) ) ;
        fftGUI->addWidgetRight(new ofxUISlider(length-xInit,dim , 0.0f , 4.0f , radialFft.noiseIndexMultiplier , "NOISE INDEX MULTIPLIER" ) ) ;
        fftGUI->addWidgetDown(new ofxUISlider(length-xInit,dim , 0.0f , 40.0f , radialFft.amplitudeMultiplier , "AMPLITUDE MULTIPLIER" ) ) ;
    
    
        fftGUI->addWidgetDown(new ofxUISlider(length-xInit,dim , 0.0f , 360.0f , radialFft.hue , "HUE" ) ) ;
        fftGUI->addWidgetRight(new ofxUISlider(length-xInit,dim , 0.0f , 360.0f , radialFft.saturation, "SATURATION" ) ) ;
        fftGUI->addWidgetRight(new ofxUISlider(length-xInit,dim , 0.0f , 360.0f , radialFft.brightness , "BRIGHTNESS" ) ) ;
    
        fftGUI->addWidgetDown(new ofxUISlider(length-xInit,dim , 0.0f , 1000.0f , radialFft.hueFftMultiplier , "HUE FFT MULTIPLIER" ) ) ;
        fftGUI->addWidgetDown(new ofxUISlider(length-xInit,dim , 0.0f , 1000.0f , radialFft.amplitudeDiffMultiplier , "AMPLITUDE DIFF MULTIPLIER" ) ) ;
    
        fftGUI->addWidgetDown(new ofxUISlider(length-xInit,dim , 0.0f , 255.0f , clearAlpha , "CLEAR ALPHA" ) ) ;
        fftGUI->addWidgetRight(new ofxUISlider(length-xInit,dim , 0.0f , 1000.0f , amplitudeScale , "GLOBAL AMPLITUDE SCALE" ) ) ;

    
    /*
    
     if ( name == "GLOBAL AMPLITUDE SCALE")
     {
     ofxUISlider *slider = (ofxUISlider *) e.widget;
     ampltiudeScale = slider->getScaledValue() ;
     }
     

     
     if ( name == "GLOBAL AMPLITUDE SCALE")
     {
     ofxUISlider *slider = (ofxUISlider *) e.widget;
     ampltiudeScale = slider->getScaledValue() ;
     }
     
     */

        ofAddListener(fftGUI->newGUIEvent,this,&testApp::fftGUIEvent);
        fftGUI->loadSettings( "GUI/FFT_0.xml") ;
    

    ofSetFrameRate( 60 ) ;
    ofSetCircleResolution( 180 ) ;
    
    screenFbo.allocate(ofGetWidth(), ofGetHeight() , GL_RGBA ) ;
    screenFbo.begin() ;
    ofClear( 1 , 1 , 1 , 0 ) ;
    screenFbo.end() ; 
}

void testApp::fftGUIEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if ( name == "FFT RANGE" )
    {
        ofxUIRangeSlider *rslider = (ofxUIRangeSlider *) e.widget; 
        radialFft.range.startIndex = rslider->getScaledValueLow() ;
        radialFft.range.endIndex = rslider->getScaledValueHigh() ;
    }
    
    if ( name == "CENTER OFFSET" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.globalCenterOffset = slider->getScaledValue() ; 
    }
    
    if ( name == "GLOBAL EXTRUSION" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.globalExtrusion = slider->getScaledValue() ;
    }
    
    if ( name == "MAX EXTRUSION" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.range.maxExtrusion = slider->getScaledValue() ;
    }
    
    if ( name == "BAR WIDTH"  )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.globalBarWidth = slider->getScaledValue() ;
    }
    
    if ( name == "NOISE TIME MULTIPLIER"  )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.noiseTimeMultiplier = slider->getScaledValue() ;
    }
    
    if ( name == "NOISE STRENGTH" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.noiseStrength = slider->getScaledValue() ;
    }
    
    if ( name == "NOISE INDEX MULTIPLIER"  )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.noiseIndexMultiplier = slider->getScaledValue() ;
    }
    
    if ( name == "CENTER OFFSET" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.globalCenterOffset = slider->getScaledValue() ;
    }
    
    if ( name == "DRAW FILL" )
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        radialFft.doFill = toggle->getValue() ; 
    }

    
    if ( name == "DRAW BOTH SIDES" )
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        radialFft.drawBothSides = toggle->getValue() ;
    }
    
    if( name == "AMPLITUDE MULTIPLIER" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.amplitudeMultiplier = slider->getScaledValue() ; 
    }
    
    if(name == "X + Y")
    {
        ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
        radialFft.position = pad->getScaledValue() ;
    }
    
    if( name == "HUE" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.hue = slider->getScaledValue() ;
    }
    
    if( name == "SATURATION" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.saturation = slider->getScaledValue() ;
    }
    
    if( name == "BRIGHTNESS" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.brightness = slider->getScaledValue() ;
    }
    
    if( name == "HUE FFT MULTIPLIER" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.hueFftMultiplier = slider->getScaledValue() ;
        //radialFft.updateColor() ;
    }
    
    if ( name == "AMPLITUDE DIFF MULTIPLIER" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radialFft.amplitudeDiffMultiplier = slider->getScaledValue() ; 
    }
    
    if ( name == "CLEAR ALPHA" )
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        clearAlpha = slider->getScaledValue() ; 
    }
    
    if ( name == "GLOBAL AMPLITUDE SCALE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        amplitudeScale = slider->getScaledValue() ; 
    }

    fftGUI->saveSettings("GUI/FFT_0.xml" ) ;
}


void testApp::update()
{
    //Get amplitudes and pass them on to visuals
    float * amplitudes = fft->getAmplitude() ;

    ////if ( ofGetElapsedTimef() > 30 )
    radialFft.update( amplitudes ) ;
    //for ( int i = 0 ; i < visuals.size() ; i++ )
    //{
    //    visuals[i].update( amplitudes ) ;
    //}
}


void testApp::draw()
{
   //  if ( ofGetElapsedTimef() < 30 )
   //      return ;
    
   
       
     
	
    
	

    
    ofEnableAlphaBlending() ;
    screenFbo.begin() ;
    ofSetColor( 0 ,  0, 0 , clearAlpha ) ;
    ofRect( 0 , 0, ofGetWidth() , ofGetHeight() ) ;
     ofSetColor ( 255.0f , 255.0f , 255.0f ) ;

    ofEnableSmoothing() ;

   // for ( int i = 0 ; i < visuals.size() ; i++ )
   // {
   //     visuals[i].draw( ) ;
    radialFft.draw( ) ; 
   // }

    screenFbo.end() ;
    
    ofSetColor( 255 , 255  ,255 ) ; 
    screenFbo.draw( 0 , 0 ) ;
   
    
    if ( bGuiEnabled == true )
    {
        ofSetColor ( 255 , 255 , 255 )  ;
        //gui.draw() ;
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
        
        int graphYOffset = 10 ;
        ofSetHexColor(0xffffff);
        ofPushMatrix();
        ofTranslate( ( ofGetWidth() +- bufferSize )/2 , ofGetHeight() + plotHeight * -3 + -3 * graphYOffset );
        ofDrawBitmapString("Time Domain", 0, 0);
        //Draw Range
        const float binLength = (float)(fft->getBinSize()) ;
        ofEnableAlphaBlending() ;
        ofSetColor ( 255 , 0 , 0 , 125 ) ;
        ofFill() ;
        
        //for int i = 0 , for each range
        //float alpha = 255.0f / (float)visuals.size()  ;
        float ySpacing = plotHeight  ; /// (float)visuals.size() ;
        //for ( int i = 0 ; i < visuals.size() ; i++ )
        //{
        ofSetColor ( radialFft.color , 255 ) ;
        ofRect ( (float)radialFft.range.startIndex/binLength * bufferSize , ySpacing * 0 , (float)radialFft.range.numIndicies/binLength * bufferSize , ySpacing ) ;
        // }
        
        //void testApp::plot(float* array, int length, float scale, float offset) {
        plot(audioInput, bufferSize, plotHeight , 0);
        
       
        ofTranslate(0, plotHeight + graphYOffset );
        ofDrawBitmapString("Frequency Domain", 0, 0);
        ofSetColor ( 255 , 255 , 255 ) ;
        ofFill() ;
        fft->draw(0, 0, fft->getSignalSize(), plotHeight);
        ofTranslate( 0 ,  plotHeight + graphYOffset );
        spectrogram.update();
        spectrogram.draw(0, 0, bufferSize , plotHeight );
        
       
        ofPopMatrix();
        string msg = ofToString((int) ofGetFrameRate()) + " fps";
        
       
    }

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

    switch (key)
    {
        case 'p':
        {
          //  drawPadding = !drawPadding;
           // gui->setDrawWidgetPadding(drawPadding);
        }
            break;
            
        case 'g':
        {
            bGuiEnabled = false ; 
            
            fftGUI->toggleVisible();
            
            if ( fftGUI->isVisible() == true )
            {
                bGuiEnabled = true ;
            }
          //  bGuiEnabled = fftGUI->isVisible() ;
        }
            break;
            
            //FFT Mode
        case 'm': mode = MIC; break;
        case 'n': mode = NOISE; break;
        case 's': mode = SINE; break;
        case 'a': mode = ANIMATE ; break ;
            
        default:
            break;
    }
    /*
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
    */
}
