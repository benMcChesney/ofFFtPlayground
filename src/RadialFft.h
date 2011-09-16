#ifndef RADIALFFT_H
#define RADIALFFT_H

#include "FftRange.h"
#include "RadialBar.h"

class RadialFft
{
    public:
        RadialFft();
        RadialFft( float gco , float ge , float gbw , ofVec2f pos , FftRange r )
        {   
            globalCenterOffset = gco ;
            globalExtrusion = ge ;
            globalBarWidth = gbw ;
            
            position = pos ; 
            range = r ; 
        }
    
        void setup( int numBins ) ;
        void update( float * amplitudes ) ;
        void draw( bool drawBothSides ) ; 
    
        virtual ~RadialFft();
    
        vector<RadialBar> bars ;
        FftRange range ;
    
        float globalCenterOffset ;
        float globalExtrusion ;
        float globalBarWidth ;
    
        ofVec2f position ; 
    protected:
    private:
};

#endif // RADIALFFT_H
