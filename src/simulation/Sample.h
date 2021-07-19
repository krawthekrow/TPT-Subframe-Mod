#pragma once
#include "Particle.h"

class SimulationSample
{
public:
	Particle particle;
	int ParticleID;
	int SParticleCount;
	Particle SParticles[5];
	int SParticleIDs[5];
	int StackIndexBegin, StackIndexEnd;

	int PositionX, PositionY;
	float AirPressure;
	float AirTemperature;
	float AirVelocityX;
	float AirVelocityY;

	int WallType;
	float Gravity;
	float GravityVelocityX;
	float GravityVelocityY;

	int NumParts;
	bool isMouseInSim;

	SimulationSample() : particle(), ParticleID(0), SParticleCount(0), StackIndexBegin(0), StackIndexEnd(0), PositionX(0), PositionY(0), AirPressure(0), AirTemperature(0), AirVelocityX(0), AirVelocityY(0), WallType(0), Gravity(0), GravityVelocityX(0), GravityVelocityY(0), NumParts(0), isMouseInSim(true) {}
};
