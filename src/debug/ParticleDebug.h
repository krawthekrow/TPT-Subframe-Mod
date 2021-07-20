#pragma once
#include "DebugInfo.h"

class Simulation;
class GameModel;
class GameController;
class ParticleDebug : public DebugInfo
{
	Simulation * sim;
	GameModel * model;
	GameController * c;
public:
	ParticleDebug(unsigned int id, Simulation * sim, GameModel * model, GameController * c);
	int UpdateSimUpToInterestingChange();
	void Debug(int mode, int x, int y);
	bool KeyPress(int key, int scan, bool shift, bool ctrl, bool alt, ui::Point currentMouse) override;
	void Update() override;
};
