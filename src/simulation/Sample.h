#ifndef The_Powder_Toy_Sample_h
#define The_Powder_Toy_Sample_h

#include "Particle.h"

class SimulationSample
{
public:
	static const int SPRK_FLAG = 0x1;
	static const int FILT_FLAG = 0x2;

	Particle particle;
	int ParticleID;
	int SParticleCount;
	Particle SParticles[5];
	int SParticleIDs[5];
	int StackIndexBegin, StackIndexEnd;
	int EffectiveStackEditDepth;

	// config tool info
	int AdjacentPartsInfo[3][3];

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

	// returns -1 if no particles exist in sample
	int GetStackEditPartId()
	{
		if (!isMouseInSim || SParticleCount == 0)
			return -1;
		return SParticleIDs[EffectiveStackEditDepth - StackIndexBegin];
	}
};

#endif
