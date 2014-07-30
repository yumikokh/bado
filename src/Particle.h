#pragma once

#include "ofMain.h"

class Particle
{
public:

	ofVec2f pos; //�ʒu
	ofVec2f vel; //���x
	ofVec2f frc; //�́i�����x�j

	float damping; //���C��

	Particle();
	virtual ~Particle();
	void resetForce();
	void addForce(float x, float y);
	void AddDampingForce();
	void setInitialCondition(float px, float py, float vx, float vy);
	void update();
	void draw();

};

