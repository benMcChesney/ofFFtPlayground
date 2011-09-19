#ifndef FFTRANGE_H
#define FFTRANGE_H

//Really simple class to hold the FFT range values and update properly

class FftRange
{
    public:
        FftRange()
        {
            startIndex = 0 ;
            endIndex = 1 ;
            numIndicies = 1 ;
        }

        FftRange ( int _startIndex , int _endIndex , int _maxExtrusion )
        {
            startIndex = _startIndex ; 
            endIndex = endIndex ; 
            numIndicies = endIndex +- startIndex ; 
            maxExtrusion = _maxExtrusion ; 
        }

        void update()
        {
            if ( endIndex < startIndex )
            {
                int tempIndex = endIndex ;
                endIndex = startIndex ;
                startIndex = tempIndex ;
            }
            numIndicies = endIndex +- startIndex ;
        }
        virtual ~FftRange() {}

        int startIndex ;
        int endIndex ;
        int numIndicies ;

        float maxExtrusion ;

    protected:
    private:
};

#endif // FFTRANGE_H
