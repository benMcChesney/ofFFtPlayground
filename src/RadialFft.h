#ifndef RADIALFFT_H
#define RADIALFFT_H

#include "FftRange.h"
#include "RadialBar.h"
#include "Tweenzor.h"

class RadialFft
{
    public:
        RadialFft() { }
        ~RadialFft( ) { }
    
        RadialFft( float gco , float ge , float gbw , ofVec2f pos , FftRange r , ofColor _color )
        {
            doFill = false ;
            drawBothSides = false ;

            globalCenterOffset = gco ;
            globalExtrusion = ge ;
            globalBarWidth = gbw ;

            position = pos ;
            range = r ;

            color = _color ; 
         }
    
    //fromHsb (float hue, float saturation, float brightness, float alpha = limit());
        float hue ;
        float saturation ;
        float brightness ;
    
        float hueFftMultiplier ;
    
        float amplitudeMultiplier ;
    
        float noiseTimeMultiplier ;
        float noiseStrength ;
        float noiseIndexMultiplier ; 

        void setup( int numBins ) ;
        void update( float * amplitudes ) ;
        void draw( ) ;

        vector<RadialBar> bars ;
        FftRange range ;

        bool drawBothSides ;
        bool doFill ;
    ofColor color ; 
        float meanAmplitude ;

        float globalCenterOffset ;
        float globalExtrusion ;
        float globalBarWidth ;

        ofVec2f position ;
    
   
    
        float lastAmplitude ;
        float amplitudeDiffMultiplier ;
        float ampltiudeDiff ;
    
        float interpolatedMeanAmplitude ;
        float interpolateTime ; 


    protected:
    private:
};

#endif // RADIALFFT_H
