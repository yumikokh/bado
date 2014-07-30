#pragma once
//#include <windows.h>
#include "ofMain.h"
#include <GLUT/glut.h>
#include "ofxCubeMap.h"
#include "RhythmPattern.h"
#include <deque>

#include "ofxOsc.h"
#include "natOscMocapDataReceiver.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 768

//#include "Cortex.h"
//#pragma comment(lib,"src/Cortex_SDK.lib")

#define PORT 12345
#define NUM_MSG_STRINGS 20


//�O�Ԗڂ̃e���v���[�g�̃}�[�J�[����`
#define TEMPLATE0_MARKERNUM 4

#define TEMPLATE_RACKET_NUM 2
#define TEMPLATE_RACKET_MARKER_NUM 4
#define TEMPLATE_PLAYER_MARKER_NUM 4
#define TEMPLATE_PLAYER_NUM 2
#define SHUTTLE_FRAME_BUFFER_NUM 10
#define PARTICLE_NUM 20
#define IMPACT_NUM 100

class testApp : public ofBaseApp{
public:
    void setup();//�ŏ��Ɉ����s
    void glInit();//openGL�̏�����
    void update();//���t���[�������s�A�v�Z�p
    void draw();//���t���[�������s�A�`��p
    
    void keyPressed(int key);//�L�[�{�[�h�̃L�[�����������Ɏ��s
    void keyReleased(int key);//�L�[�{�[�h�̃L�[���������Ɏ��s
    void mouseMoved(int x, int y);//�}�E�X���E�B���h�E���œ������Ǝ��s
    void mouseDragged(int x, int y, int button);//�}�E�X�ŃE�B���h�E�����h���b�O����Ǝ��s
    void mousePressed(int x, int y, int button);//�}�E�X�N���b�N���Ɏ��s
    void mouseReleased(int x, int y, int button);//�}�E�X�N���b�N�𗣂��Ǝ��s
    void windowResized(int w, int h);//�E�B���h�E�̑傫����ς���Ǝ��s
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioRequested(float * input, int bufferSize, int Channels);
    
    ofTrueTypeFont font;
//    ofxOscReceiver receiver;
    
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    int mouseX, mouseY;
    string mouseButtonState;
    nat::OscMocapDataReceiver oscMocapReciever;
    
    //�E�B���h�E�T�C�Y�ۑ�
    int windowWidth;
    int windowHeight;
    
    ofVec3f shuttlePreFramePos;	//�V���g���̑O�̃t���[���̈ʒu
    
    deque<ofVec3f> shuttleMarkerPoses;
    ofVec3f shuttleMarkerPos;
    ofVec3f shuttleMarkerGetPos;
    
    float racketMarkerPos[TEMPLATE_RACKET_NUM][TEMPLATE_RACKET_MARKER_NUM][3];	//���P�b�g2�̃}�[�J�[�ʒu4��ۑ�(x, y, z)
    //float playerMarkerPos[TEMPLATE_PLAYER_NUM][TEMPLATE_PLAYER_MARKER_NUM][3];  //�v���C���[2�l�̃}�[�J�[�ʒu4��ۑ�(x, y, z)
    
    //float shuttleVec[3];		//�V���g���̃x�N�g��
    bool shuttleVecPlusOrMinus;	//�V���g���̃x�N�g�����v���X���}�C�i�X���ŕ����𔻒�
    
    ofVec3f shuttleVec;
    ofVec3f shuttleVecCam;
    
    //�p�[�e�B�N��
    deque<ofVec3f> pos;
    deque<ofVec3f> vel;
    deque<float> pSize;
    
 
    ofxCubeMap myCubeMap;
    
    ofImage img;
    ofImage img2;
    
    
    //�J����
    ofEasyCam cam;
    
    float angle;
    float random[10];
    
    //��
    int sampleRate;
    int bufSize;
    float amp;
    float pan;
    float phase0;
    float frequency;
    
    int waveShape;
    
    float lAudio[256];
    float rAudio[256];
    
    
    //�T���v�����O
    bool rec;
    bool play;
	ofSoundPlayer sounds[3];
    
    vector <RhythmPattern*> rhythms;
    RhythmPattern *rhythmPtr;
    ofSoundPlayer *soundPtr;
    
    int cutTime;
    int nowFrame;
    
    int phase;
    
    
    
};
