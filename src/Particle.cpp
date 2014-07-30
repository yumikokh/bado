#include "Particle.h"


Particle::Particle(void)
{
	setInitialCondition(0,0,0,0);
	damping = 0.01f;
}

void Particle::resetForce(){
	frc.set(0,0);
}

void Particle::addForce(float x, float y){
	frc.x += x;
	frc.y += y;
}

void Particle::AddDampingForce(){
	frc.x = frc.x - vel.x * damping;
	frc.y = frc.y - vel.y * damping;
}

void Particle::setInitialCondition( float px, float py, float vx, float vy){
	pos.set(px,py);
	vel.set(vx,vy);
}

void Particle::update(){
	vel += frc;
	pos += vel;
}

void Particle::draw(){
	ofCircle(pos.x, pos.y, 3);
}

Particle::~Particle(void)
{
}
