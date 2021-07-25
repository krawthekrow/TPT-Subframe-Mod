#pragma once
#include "Particle.h"

struct SimulationSample
{
	static const int SPRK_FLAG = 0x1;
	static const int FILT_FLAG = 0x2;

	Particle particle;
	int ParticleID = 0;
	int SParticleCount = 0;
	Particle SParticles[5] = {0};
	int SParticleIDs[5] = {0};
	int StackIndexBegin = 0;
	int StackIndexEnd = 0;
	int EffectiveStackEditDepth = 0;

	// config tool info
	int AdjacentPartsInfo[3][3] = {0};

	int PositionX = 0;
	int PositionY = 0;
	float AirPressure = 0;
	float AirTemperature = 0;
	float AirVelocityX = 0;
	float AirVelocityY = 0;

	int WallType = 0;
	float GravityVelocityX = 0;
	float GravityVelocityY = 0;

	int NumParts = 0;
	bool isMouseInSim = true;

	// returns -1 if no particles exist in sample
	int GetStackEditPartId()
	{
		if (!isMouseInSim || SParticleCount == 0)
			return -1;
		return SParticleIDs[EffectiveStackEditDepth - StackIndexBegin];
	}
};
