#ifndef RADIALFFT_H
#define RADIALFFT_H

#include "FftRange.h"
#include "RadialBar.h"

class RadialFft
{
    public:
        RadialFft();
        RadialFft( float gco , float ge , float gbw , ofVec2f pos , FftRange r , ofColor _color )
        {
            doFill = false ;
            drawBothSides = false ;

            globalCenterOffset = gco ;
            globalExtrusion = ge ;
            globalBarWidth = gbw ;

            position = pos ;
            range = r ;

            red = _color.r ;
            green = _color.g ;
            blue = _color.b ;
         }

        void setup( int numBins ) ;
        void update( float * amplitudes ) ;
        void draw( ) ;

        virtual ~RadialFft();

        vector<RadialBar> bars ;
        FftRange range ;

        bool drawBothSides ;
        bool doFill ;
        int red , green, blue ;
        float meanAmplitude ;

        float globalCenterOffset ;
        float globalExtrusion ;
        float globalBarWidth ;

        ofVec2f position ;


    protected:
    private:
};

#endif // RADIALFFT_H
