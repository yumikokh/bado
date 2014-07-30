//
//  RhythmPattern.h
//  emptyExample
//
//  Created by Yumiko Kokubu on 2014/05/11.
//
//

#ifndef __emptyExample__RhythmPattern__
#define __emptyExample__RhythmPattern__

#include <iostream>

#include "ofMain.h"
#include "TPMdata.h"

class RhythmPattern{
public:
    RhythmPattern(ofSoundPlayer *_sound, int _phase);
    void play();
    void record(ofVec3f &_pos);
    
    
    int timeCount;
    int soundCount;
    int phase;
    int startFrame;

    
    vector <TPMdata> tpms;

    ofSoundPlayer *sound;
};



#endif /* defined(__emptyExample__RhythmPattern__) */


