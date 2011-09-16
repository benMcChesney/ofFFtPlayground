#include "RadialFft.h"

RadialFft::RadialFft()
{
    //ctor

}

RadialFft::~RadialFft()
{
    //dtor
}


void RadialFft::setup( int numBins )
{
    drawBothSides = false ;
    float radius = 65.0f ;
    float thetaStep = TWO_PI / (float) numBins ;

    globalCenterOffset = radius ;
    globalExtrusion = 50.0f ;
    globalBarWidth = 4.0f ;

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
        // points.push_back ( ofVec3f( barOffset + xIncrement * i , 0 , 0 ) ) ;
    }


    //range = FftRange( 0 , 200 , 200 ) ;
    range.startIndex = 0 ;
    range.endIndex = 200 ;
    range.numIndicies = 200 ;
    range.maxExtrusion = 240.0f ;
}


void RadialFft::update( float * amplitudes )
{
    range.update() ;
   // color.setHex ( hexColor ) ;
    const int len = range.numIndicies ;

    //If the range has changed we need to clear the vector and re populate it
    if ( bars.size() != range.numIndicies ) bars.clear() ;

    float thetaStep = TWO_PI / (float) range.numIndicies ;

    float sumAmplitude = 0.0f ;
    for ( int i = 0 ; i < len ; i++ )
    {

        float theta = thetaStep * (float) i ;
        ofVec3f newPoint = ofVec3f ( cos( theta ) * globalCenterOffset , sin ( theta ) * globalCenterOffset , 0 ) ;
        RadialBar rb = RadialBar() ;

        if ( bars.size() != range.numIndicies ) bars.push_back( rb ) ;

        bars[i].position = newPoint ;
        bars[i].theta = theta ;
        bars[i].startDistance = globalCenterOffset ;
        bars[i].extrusion = amplitudes[range.startIndex + i] * globalExtrusion ;
        sumAmplitude += amplitudes[ range.startIndex + i ] ;
    }

    meanAmplitude = sumAmplitude / ( float ) range.numIndicies ;
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
    ofSetColor ( red , green , blue ) ;

    for ( int i = 0 ; i < len ; i++ )
    {
        ofPushMatrix() ;
        ofTranslate( position.x + bars[i].position.x , position.y +  bars[i].position.y ) ;
        ofRotate( ofRadToDeg( bars[i].theta ) - 90  ) ;
        float extrusion =  bars[i].extrusion ;
        extrusion = ( extrusion > range.maxExtrusion ) ? range.maxExtrusion : extrusion ;
        ofRect (  -halfWidth , 0 , globalBarWidth ,  extrusion ) ;

        if ( drawBothSides == true )
        {
            ofRotate( 180  ) ;
            ofRect (  -halfWidth , 0 , globalBarWidth ,  extrusion ) ;
        }

        ofPopMatrix() ;
    }

    ofFill( ) ;
    ofEnableAlphaBlending() ;
    ofSetColor ( red , green, blue , 125 ) ;
   // float extrusion = ( extrusion > range.maxExtrusion ) ? range.maxExtrusion : extrusion ;

    ofCircle ( position.x , position.y , globalCenterOffset * 0.1f +  meanAmplitude * globalExtrusion ) ;
    ofDisableAlphaBlending() ;
}
