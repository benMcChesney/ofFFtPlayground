#include "RadialFft.h"

void RadialFft::setup( int numBins )
{
    
    drawBothSides = false ;
    float thetaStep = TWO_PI / (float) numBins ;

    //Default values, XML will overwrite these
    globalCenterOffset = 65.0f ; 
    globalExtrusion = 50.0f ;
    globalBarWidth = 4.0f ;

    //set the range properties
    range.startIndex = 0 ;
    range.endIndex = 200 ;
    range.numIndicies = 200 ;
    range.maxExtrusion = 240.0f ;
    
    lastAmplitude = 0 ;
    
    //Create starting positions for bars, these will be updated every frame
    for ( int i = 0 ; i < numBins ; i++ )
    {
        float theta = thetaStep * (float) i ;
        ofVec3f newPoint = ofVec3f ( cos( theta ) * globalCenterOffset , sin ( theta ) * globalCenterOffset , 0 ) ;
        RadialBar rb = RadialBar() ;
        rb.position = newPoint ;
        rb.theta = theta ;
        rb.startDistance = globalCenterOffset ;
        rb.extrusion = globalExtrusion;
        bars.push_back( rb ) ;
    }
    
    color = ofColor( ofRandom ( 255 ) , ofRandom( 255 ) , ofRandom( 255 ) ) ; 

}


void RadialFft::update( float * amplitudes )
{
    
    
    range.update() ;
    const int len = range.numIndicies ;

    //If the range has changed we need to clear the vector and re populate it
    if ( bars.size() != range.numIndicies ) bars.clear() ;

    float thetaStep = TWO_PI / (float) range.numIndicies ;

    float sumAmplitude = 0.0f ;
    
    
    for ( int i = 0 ; i < len ; i++ )
    {

        float theta = thetaStep * (float) i ;
        ofVec3f newPoint = ofVec3f ( cos( theta ) * globalCenterOffset , sin ( theta ) * globalCenterOffset , 0 ) ;

        //if there is a size change create and add a new RadialBar
        if ( bars.size() != range.numIndicies )
        {
            RadialBar rb = RadialBar() ;
            bars.push_back( rb ) ;
        }
        
        bars[i].position = newPoint ;
        bars[i].theta = theta ;
        bars[i].startDistance = globalCenterOffset ;
        
       // if ( i < numAmplitudes )
       // {
        
        /*
         float noiseTimeMultiplier ;
         float noiseStrength ;
         float noiseIndexMultiplier ;
         */
        float noiseOffset = ofNoise( ofGetElapsedTimef() * noiseTimeMultiplier + ( i * noiseIndexMultiplier) )  * noiseStrength  ;
            bars[i].extrusion = amplitudes[range.startIndex + i] * amplitudeMultiplier * globalExtrusion + noiseOffset ;
            sumAmplitude += amplitudes[ range.startIndex + i ] ;
       // }
    }

    //Get the Mean, useful if you want a single value to change an animation
    meanAmplitude = sumAmplitude / ( float ) range.numIndicies ;
    
    
    

    Tweenzor::add( &interpolatedMeanAmplitude , interpolatedMeanAmplitude , meanAmplitude , 0.0f , interpolateTime , EASE_OUT_QUAD ) ;
    ampltiudeDiff = meanAmplitude - lastAmplitude ;
    //cout << "amplitude diff : " << ampltiudeDiff << endl ;
    lastAmplitude = meanAmplitude ;
    //hueFftMultiplier
    if ( hue > 360 )
        hue -= 360.0f ; 
    color = ofColor::fromHsb( hue + ( meanAmplitude * amplitudeMultiplier ) * hueFftMultiplier , saturation, brightness ) ;
}

void RadialFft::draw( )
{
    
    
    float halfWidth = globalBarWidth / 2.0f ;
    const int len = bars.size() ;
    float ratio ;
    if ( doFill == true )
        ofFill( ) ;
    else
        ofNoFill() ;
    ofSetColor ( color ) ;
    
    float diff = amplitudeDiffMultiplier * ampltiudeDiff  ; //ampltiduelastAmplitude - meanAmplitude ;
   // diff *= amplitudeDiffMultiplier ;
    
    ofPushMatrix() ;
    ofTranslate( position.x , position.y ) ;
    ofScale( 1 + diff  , 1+  diff  ) ;
    ofRotateZ( diff * 360.0f ) ;
    for ( int i = 0 ; i < len ; i++ )
    {
        ofPushMatrix() ;
            ofTranslate( bars[i].position.x , bars[i].position.y ) ;
            ofRotate( ofRadToDeg( bars[i].theta ) - 90  ) ;
        

            float extrusion =  bars[i].extrusion ;
        
            //Limit the jumpiness of ranges
            extrusion = ( extrusion > range.maxExtrusion ) ? range.maxExtrusion : extrusion ;
        
            //Draw bar outward and centered
            ofRect (  -halfWidth , diff * 10.000f , globalBarWidth ,  extrusion ) ;

            if ( drawBothSides == true )
            {
                //Draw the other bar
                ofRotate( 180  ) ;
                ofRect (  -halfWidth , diff , globalBarWidth ,  extrusion ) ;
            }

        ofPopMatrix() ;
    }
    
    ofPopMatrix() ; 

    
    ofEnableAlphaBlending() ;
    ofSetColor ( color.r , color.g , color.b , 125 ) ;

    float adjustedOffset = globalCenterOffset * meanAmplitude * amplitudeMultiplier ; 
    //Draw two circles based on the mean amplitude
    ofCircle ( position.x , position.y , globalCenterOffset * 0.1f +  meanAmplitude * globalExtrusion ) ;
    ofCircle ( position.x , position.y , globalCenterOffset * 0.1f +  meanAmplitude * globalExtrusion * .1f ) ;
    
    ofDisableAlphaBlending() ;
}
