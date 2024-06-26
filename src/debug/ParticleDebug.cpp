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

void ParticleDebug::Update()
{
	if (!sim->subframe_mode)
		return;

	int i;
	do
	{
		i = sim->debug_nextToUpdate;
		while (i < NPART - 1 && !sim->parts[i].type)
			i++;
		sim->framerender = 1;
		model->UpdateUpTo(i + 1);
	}
	while(i < NPART - 1 && !sim->debug_interestingChangeOccurred);
}

void ParticleDebug::Debug(int mode, int x, int y)
{
	int i = 0;
	String logmessage;

	if (sim->debug_nextToUpdate == 0 && sim->needReloadParticleOrder && c->GetAutoreloadEnabled())
	{
		model->ReloadParticleOrder();

		model->Log("Particle order reloaded.", false);
	}

	if (mode == 0)
	{
		if (!sim->NUM_PARTS)
			return;
		i = sim->debug_nextToUpdate;
		while (i < NPART - 1 && !sim->parts[i].type)
			i++;
		if (i == NPART - 1)
			logmessage = "End of particles reached, updated sim";
		else
			logmessage = String::Build("Updated particle #", i);
	}
	else if (mode == 1)
	{
		i = NPART - 1;
		if (x >= 0 && x < XRES && y >= 0 && y < YRES)
		{
			int partid = sim->GetStackEditParticleId(x, y);
			if (partid != NPART && partid >= sim->debug_nextToUpdate)
			{
				i = partid;
			}
		}
	}
	else
	{
		printf("BUG: SetDebug called with unknown mode");
	}
	sim->framerender = 1;
	auto prevToUpdate = sim->debug_nextToUpdate;
	model->UpdateUpTo(i + 1);
	if (sim->debug_nextToUpdate)
	{
		logmessage = String::Build("Updated particles from #", prevToUpdate, " through #", i);
	}
	else
	{
		logmessage = String::Build("Updated particles from #", prevToUpdate, " to end");
	}
	model->Log(logmessage, false);
}

bool ParticleDebug::KeyPress(int key, int scan, bool shift, bool ctrl, bool alt, ui::Point currentMouse)
{
	if (key == 'f' && !ctrl)
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
			if (sim->debug_nextToUpdate > 0)
			{
				String logmessage = String::Build("Updated particles from #", sim->debug_nextToUpdate, " to end due to frame step");
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
