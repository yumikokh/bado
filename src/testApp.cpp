#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    oscMocapReciever.setup(19700);
    
	glInit();
	
	//ofBackground(0,0,0);
	windowResized(windowWidth,windowHeight);
	ofSetFrameRate(60);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	//ofSetBackgroundAuto(true);
    
	play = false;
    
	sounds[0].loadSound("waterdrop2.wav");
    sounds[1].loadSound("pon.mp3");
    sounds[2].loadSound("zi.mp3");
    
    soundPtr = &sounds[0];
    phase = 300;
	rec = false;
	nowFrame = 0;
    
	img.loadImage("emitter.png");
	img2.loadImage("particle2.png");
    myCubeMap.loadImages( "positive_x.png","positive_y.png","positive_z.png","negative_x.png", "negative_y.png", "negative_z.png" );
    
	shuttleMarkerPos.x = 10.0f;
	shuttleMarkerPos.y = 10.0f;
	shuttleMarkerPos.z = 0.0f;
    
	for(int i=0; i<TEMPLATE_RACKET_NUM; i++){
		for(int j=0; j<TEMPLATE_RACKET_MARKER_NUM; j++){
			racketMarkerPos[i][j][0] = 0.0f;
			racketMarkerPos[i][j][1] = 0.0f;
			racketMarkerPos[i][j][2] = 0.0f;
		}
	}
    
	shuttleVec[0] = 0.0f;
	shuttleVec[1] = 0.0f;
	shuttleVec[2] = 0.0f;
    
	shuttleVecPlusOrMinus = false;
    
	angle = 1.0f;
    
	shuttlePreFramePos = ofVec3f(1000.0f, 1000.0f, 0.0f);

	bufSize = 256;
	sampleRate = 44100;
	amp = 0.5;
	pan = 0.5;
	phase0 = 0;
	frequency = 440;
	waveShape = 1;
    
	ofSoundStreamSetup(2, 0, this);
    
}

void testApp::glInit(){
    
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	

	float position[] = {1, 1, 1, 0};
	float direction[] = {-1, -1, -1};
	float diffuse[] = {0.7, 0.7, 0.7, 1};
	float specular[] = {0.7, 0.7, 0.7, 1};
	float shi[] = {30};
	float ambient[] = {0.3, 0.3, 0.3, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_SHININESS, shi);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
}
//--------------------------------------------------------------
void testApp::update(){
    
    while(oscMocapReciever.hasWaitingMessages()){
        float **unBuf = oscMocapReciever.getUnidentifiedMarkerData();
        std::cout << "x:" << unBuf[0][0] << std::endl;
        std::cout << "y:" << unBuf[0][1] << std::endl;
        std::cout << "z:" << unBuf[0][2] << std::endl;
        //ofDrawBitmapString(std::to_string((int)buf[1][0]), 100, 100);
        shuttleMarkerGetPos[0] = unBuf[0][0];
        shuttleMarkerGetPos[1] = unBuf[0][1];
        shuttleMarkerGetPos[2] = unBuf[0][2];
    }
    
    
    
    double a = pow(shuttleMarkerGetPos[0] - shuttlePreFramePos.x,2);
    double b = pow(shuttleMarkerGetPos[1] - shuttlePreFramePos.y,2);
    double c = pow(shuttleMarkerGetPos[2] - shuttlePreFramePos.z,2);
    double dis = sqrt(a+b+c);
    
    if(dis > 100000){
        shuttleMarkerPos.x = shuttlePreFramePos.x;	//x
        shuttleMarkerPos.y = shuttlePreFramePos.y;	//y
        shuttleMarkerPos.z = shuttlePreFramePos.z;	//z
    }else{
        shuttleMarkerPos.x = shuttleMarkerGetPos.x;	//x
        shuttleMarkerPos.y = shuttleMarkerGetPos.y;	//y
        shuttleMarkerPos.z = shuttleMarkerGetPos.z;	//z
    }
    
    shuttlePreFramePos.x = shuttleMarkerPos[0];
    shuttlePreFramePos.y = shuttleMarkerPos[1];
    shuttlePreFramePos.z = shuttleMarkerPos[2];
    
    shuttleMarkerPoses.push_back(ofVec3f(shuttleMarkerPos[0],shuttleMarkerPos[1],shuttleMarkerPos[2]));
    
    
    if(shuttleMarkerPoses.size() > 5){
        int lastIndex = shuttleMarkerPoses.size() - 5;
        shuttleVec.x = shuttleMarkerPoses.back().x - shuttleMarkerPoses[lastIndex].x;
        shuttleVec.y = shuttleMarkerPoses.back().y - shuttleMarkerPoses[lastIndex].y;
        shuttleVec.z = shuttleMarkerPoses.back().z - shuttleMarkerPoses[lastIndex].z;
    }
    
    ofVec3f p;
    p.set(shuttleMarkerPoses.back());
    pos.push_back(p);
    
    ofVec3f v;
    v.set(ofRandom(-1,1),ofRandom(-1,1),ofRandom(-1,1));
    vel.push_back(v);
    
    pSize.push_back(3.0f);
    
    if(pos.size() == PARTICLE_NUM){
        pos.pop_front();
        vel.pop_front();
        pSize.pop_front();
    }
    
    
    
    for (int i =0; i < pos.size(); i ++){
        pos[i] += vel[i]*3;
    }
    
    if(shuttleMarkerPoses.size() == SHUTTLE_FRAME_BUFFER_NUM){
        shuttleMarkerPoses.pop_front();
    }
    
	nowFrame = ofGetFrameNum();
	
    if(abs(shuttleVec[0])>10.0){
        
        if(shuttleVec[0]>=0){
            if(shuttleVecPlusOrMinus==false){
                if (rec) {
                    soundPtr->play();
                    rhythmPtr->record(shuttleMarkerPos);
					std::cout << "RECORD!!";
                }else{
                    sounds[0].play();
                }
            }
            shuttleVecPlusOrMinus = true;
        }else{
            if(shuttleVecPlusOrMinus==true){
                if (rec) {
					std::cout << "RECORD!!";
                    soundPtr->play();
                    rhythmPtr->record(shuttleMarkerPos);
                }else{
                    sounds[0].play();
                }
            }
            shuttleVecPlusOrMinus = false;
        }
    }
    
    if (play) {
		std::cout << "PLAY!!";
        for (int i = 0; i < rhythms.size(); i++) {
            rhythms[i]->play();
		}
    }
    
    if (nowFrame == cutTime && rec == true) {
        
        std::cout << "YES!!";
        
        rhythms.push_back(rhythmPtr);
        
        rec = false;
        play = true;
    }
    
	angle += 0.1f;
	if(angle>=360.0f){
		angle = 0.0f;
	}
    
    
}

//--------------------------------------------------------------
void testApp::draw(){

	//cam.begin();
	//cam.lookAt(ofVec3f(mouseY, 0, mouseX),ofVec3f(0, 1, 0));
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScaled(0.15,0.15,0.15);
	gluLookAt(1000,3000,5000,
			  0,1000,0,
			  0, -1, 0);
    
	glPushMatrix();
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
    

	myCubeMap.bind();
	myCubeMap.drawSkybox( 10000 );
	myCubeMap.unbind();
    
	glDisable(GL_DEPTH_TEST);
//	for(int i=0; i<TEMPLATE_RACKET_NUM; i++){
//		for(int j=0; j<TEMPLATE_RACKET_MARKER_NUM; j++){
//			/*ofSphere(racketMarkerPos[i][j][0], racketMarkerPos[i][j][1], racketMarkerPos[i][j][2], 50);*/
//			img2.draw(racketMarkerPos[i][j][0], racketMarkerPos[i][j][1], racketMarkerPos[i][j][2]);
//		}
//	}
    
	img.draw(shuttleMarkerPos[0]-80, shuttleMarkerPos[1]-80, shuttleMarkerPos[2]);
	

	for (int i = 0; i < pos.size(); i++){
		ofPushMatrix();
		ofTranslate(pos[i]);
		//ofCircle(0,0, 8.0f);
		pSize[i]-=0.2f;
		glScalef(pSize[i],pSize[i],pSize[i]);
		img.draw(-80, -80, 0);
		ofPopMatrix();
	}
    
    
    
	glEnable(GL_DEPTH_TEST);
    
	glLineWidth( 3.5f );
	glBegin(GL_LINE_STRIP);
	for(deque<ofVec3f>::iterator it = shuttleMarkerPoses.begin(); it != shuttleMarkerPoses.end(); it++){
		ofSetColor(105, 255, 255, 100);
		glVertex3f(it->x, it->y, it->z);
	}
	glEnd();
    
	glLineWidth( 0.5f );
	for(deque<ofVec3f>::iterator it = shuttleMarkerPoses.begin(); it != shuttleMarkerPoses.end(); it++){
		ofSetColor(105, 35, 255, 100);
		/*ofLine(playerMarkerPos[1][1][0],playerMarkerPos[1][1][1],playerMarkerPos[1][1][2],it->x, it->y, it->z);
         ofLine(playerMarkerPos[2][1][0],playerMarkerPos[2][1][1],playerMarkerPos[2][1][2],it->x, it->y, it->z);*/
		ofLine(-3000,0,0,it->x, it->y, it->z);
		ofLine(3000,0,0,it->x, it->y, it->z);
		//ofLine(0,0,-3000,it->x, it->y, it->z);
		//ofLine(0,0,3000,it->x, it->y, it->z);
	}
	
    
	int x,z;
    
	glColor3f(1.0f, 1.0f, 1.0f);
	for(x=-3000;x<=3000;x+=1000){
		ofLine(x,0,-3000,x,0,3000);
	}
	for(z=-3000;z<=3000;z+=1000){
		ofLine(-3000,0,z,3000,0,z);
	}
    
    
	glPopMatrix();
	//cam.end();
    
}

void testApp::audioRequested(float * output, int bufferSize, int nChannels){
    //    float sample;
    //    float phaseDiff; //位相の変化
    //
    //	pan = shuttleMarkerPos.x / (float)ofGetWidth();
    //	float heightPct = (float(ofGetWidth()-shuttleMarkerPos.y) / float(ofGetHeight()));
    //	frequency = 4000.0f * heightPct;
    //
    //
    //    //1サンプルあたりの位相の変化を計算
    //    phaseDiff = TWO_PI * frequency / sampleRate;
    //
    //    //バッファ・サイズ分の波形を生成
    //    for (int i = 0; i < bufferSize; i++) {
    //        phase0 += phaseDiff;
    //        while (phase0 > TWO_PI) {
    //            phase -= TWO_PI;
    //        }
    //
    //       sample = sin(phase0);
    //
    //        lAudio[i] = output[i * nChannels] = sample * pan * amp;
    //        rAudio[i] = output[i * nChannels + 1] = sample * (1.0 - pan) * amp;
    //    }
    //
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key){
		case 'f':
			ofToggleFullscreen();
            break;
            
	    case 's':
            if (rec == false) {
                rhythmPtr = new RhythmPattern(soundPtr,phase);
                cutTime = rhythmPtr->startFrame + rhythmPtr->phase;
                std::cout << cutTime;
            }
            rec = true;
            play = false;
            break;
            
        case '1':
            soundPtr = &sounds[0];
            break;
            
        case '2':
            soundPtr = &sounds[1];
            break;
            
        case '3':
            soundPtr = &sounds[2];
            break;
            
        case 'n':
            phase = 300;
            break;
            
        case'm':
            phase = 100;
            break;
	}
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
	windowWidth=w;
	windowHeight=h;
    
	glViewport(0,0,w,h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)windowWidth / (double)windowHeight, 0.1, 1000.0);
	glPushMatrix();
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}