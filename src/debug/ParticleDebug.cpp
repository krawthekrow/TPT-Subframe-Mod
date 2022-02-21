#include "ParticleDebug.h"

#include "gui/game/GameModel.h"
#include "gui/game/GameController.h"

#include "simulation/Simulation.h"

ParticleDebug::ParticleDebug(unsigned int id, Simulation * sim, GameModel * model, GameController * c):
	DebugInfo(id),
	sim(sim),
	model(model),
	c(c)
{

}

void ParticleDebug::updateSimUpTo(int i)
{
	if (sim->debug_currentParticle == 0)
	{
		sim->framerender = 1;
		sim->BeforeSim();
		sim->framerender = 0;
	}
	sim->UpdateParticles(sim->debug_currentParticle, i);
	if (i < NPART-1)
		sim->debug_currentParticle = i+1;
	else
	{
		sim->AfterSim();
		sim->debug_currentParticle = 0;
	}
}

int ParticleDebug::updateSimOneParticle()
{
	int i = sim->debug_currentParticle;
	while (i < NPART - 1 && !sim->parts[i].type)
		i++;

	updateSimUpTo(i);
	return i;
}

int ParticleDebug::UpdateSimUpToInterestingChange()
{
	int i;
	do
	{
		i = updateSimOneParticle();
	}
	while(i < NPART && !sim->debug_interestingChangeOccurred);
	return i;
}

void ParticleDebug::Debug(int mode, int x, int y)
{
	int debug_currentParticle = sim->debug_currentParticle;
	int i = 0;
	String logmessage;

	if (sim->debug_currentParticle == 0 && sim->needReloadParticleOrder && c->GetAutoreloadEnabled())
	{
		sim->ReloadParticleOrder();

		logmessage = "Particle order reloaded.";
		model->Log(logmessage, false);
	}

	if (mode == 0)
	{
		if (!sim->NUM_PARTS)
			return;

		i = updateSimOneParticle();

		if (i == NPART - 1)
			logmessage = "End of particles reached, updated sim";
		else
			logmessage = String::Build("Updated particles #", debug_currentParticle, " through #", i);
		model->Log(logmessage, false);
	}
	else if (mode == 1)
	{
		if (x < 0 || x >= XRES || y < 0 || y >= YRES || !sim->pmap[y][x] || (i = sim->GetStackEditParticleId(x, y)) < debug_currentParticle)
		{
			i = NPART - 1;
			logmessage = String::Build("Updated particles from #", debug_currentParticle, " to end, updated sim");
		}
		else
			logmessage = String::Build("Updated particles #", debug_currentParticle, " through #", i);

		updateSimUpTo(i);

		model->Log(logmessage, false);
	}
	else
	{
		printf("BUG: SetDebug called with unknown mode");
	}
}

bool ParticleDebug::KeyPress(int key, int scan, bool shift, bool ctrl, bool alt, ui::Point currentMouse)
{
	if (key == 'f')
	{
		model->SetPaused(1);
		if (alt)
		{
			Debug(0, 0, 0);
		}
		else if (shift)
		{
			ui::Point mouse = currentMouse;
			if (mouse.X >= XRES)
				mouse.X = XRES-1;
			else if (mouse.X < 0)
				mouse.X = 0;
			if (mouse.Y >= YRES)
				mouse.Y = YRES-1;
			else if (mouse.Y < 0)
				mouse.Y = 0;

			mouse = model->AdjustZoomCoords(mouse);
			Debug(1, mouse.X, mouse.Y);
		}
		else
		{
			if (ctrl)
				return true;
			if (sim->debug_currentParticle > 0)
			{
				String logmessage = String::Build("Updated particles from #", sim->debug_currentParticle, " to end, updated sim");
				sim->CompleteDebugUpdateParticles();
				model->Log(logmessage, false);
			}
			else
			{
				model->FrameStep(1);
			}
		}
		return false;
	}
	return true;
}

ParticleDebug::~ParticleDebug()
{

}
