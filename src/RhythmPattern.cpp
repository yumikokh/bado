//
//  RhythmPattern.cpp
//  emptyExample
//
//  Created by Yumiko Kokubu on 2014/05/11.
//
//

#include "RhythmPattern.h"

RhythmPattern::RhythmPattern(ofSoundPlayer *_sound,int _phase){
    startFrame = ofGetFrameNum();
    timeCount = 0;
    phase = _phase;
    soundCount = 0;
    sound = _sound;
}

void RhythmPattern::play(){
    if(timeCount == phase ){
        timeCount = 0;
        soundCount = 0;
    }else{
		if(soundCount < tpms.size()){
			if(timeCount == tpms[soundCount].time){
				//音をならす
				sound -> play();
				soundCount++;
			}
		}
        timeCount++;
    }
}

void RhythmPattern::record(ofVec3f &_pos){
    int nowTime = ofGetFrameNum() - startFrame;
    TPMdata tmpData; //一時的に保存
    tmpData.pos = _pos;
    tmpData.time = nowTime;
    tpms.push_back(tmpData);
}

