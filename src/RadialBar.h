//
//  RadialBar.h
//  emptyExample
//
//  Created by Ben McChesney on 9/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "ofMain.h"

class RadialBar
{

    public :
    RadialBar() {} ;

    ofVec3f position ;          //position
    float theta ;               //angle from center
    float startDistance ;       //distance to start drawing from center
    float extrusion ;           //distance drawn starting from startDistance

};
