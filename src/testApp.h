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


//０番目のテンプレートのマーカー数定義
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
    void setup();//最初に一回実行
    void glInit();//openGLの初期化
    void update();//毎フレーム一回実行、計算用
    void draw();//毎フレーム一回実行、描画用
    
    void keyPressed(int key);//キーボードのキーを押した時に実行
    void keyReleased(int key);//キーボードのキー離した時に実行
    void mouseMoved(int x, int y);//マウスをウィンドウ内で動かすと実行
    void mouseDragged(int x, int y, int button);//マウスでウィンドウ内をドラッグすると実行
    void mousePressed(int x, int y, int button);//マウスクリック時に実行
    void mouseReleased(int x, int y, int button);//マウスクリックを離すと実行
    void windowResized(int w, int h);//ウィンドウの大きさを変えると実行
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
    
    //ウィンドウサイズ保存
    int windowWidth;
    int windowHeight;
    
    ofVec3f shuttlePreFramePos;	//シャトルの前のフレームの位置
    
    deque<ofVec3f> shuttleMarkerPoses;
    ofVec3f shuttleMarkerPos;
    ofVec3f shuttleMarkerGetPos;
    
    float racketMarkerPos[TEMPLATE_RACKET_NUM][TEMPLATE_RACKET_MARKER_NUM][3];	//ラケット2個のマーカー位置4個を保存(x, y, z)
    //float playerMarkerPos[TEMPLATE_PLAYER_NUM][TEMPLATE_PLAYER_MARKER_NUM][3];  //プレイヤー2人のマーカー位置4個を保存(x, y, z)
    
    //float shuttleVec[3];		//シャトルのベクトル
    bool shuttleVecPlusOrMinus;	//シャトルのベクトルがプラスかマイナスかで方向を判定
    
    ofVec3f shuttleVec;
    ofVec3f shuttleVecCam;
    
    //パーティクル
    deque<ofVec3f> pos;
    deque<ofVec3f> vel;
    deque<float> pSize;
    
 
    ofxCubeMap myCubeMap;
    
    ofImage img;
    ofImage img2;
    
    
    //カメラ
    ofEasyCam cam;
    
    float angle;
    float random[10];
    
    //音
    int sampleRate;
    int bufSize;
    float amp;
    float pan;
    float phase0;
    float frequency;
    
    int waveShape;
    
    float lAudio[256];
    float rAudio[256];
    
    
    //サンプリング
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
