#include "Tool.h"

#include "GameModel.h"

#include <vector>
#include <algorithm>

#include "gui/game/Brush.h"

#include "simulation/SimulationData.h"
#include "simulation/Simulation.h"

static bool comparePoints(ui::Point a, ui::Point b)
{
	if (a.Y == b.Y) return a.X < b.X;
	else return a.Y < b.Y;
}

static bool compareParts(Particle a, Particle b)
{
	if (a.y == b.y) return a.x < b.x;
	else return a.y < b.y;
}

// parts must be sorted!
void StackTool::ProcessParts(Simulation *sim, std::vector<int> &parts, ui::Point position, ui::Point position2)
{
	if (parts.empty()) return;
	int partx = sim->parts[parts[0]].x;
	int party = sim->parts[parts[0]].y;
	bool samePos = true;
	ui::Point stackPos(partx, party);
	for (size_t i = 1; i < parts.size(); i++)
	{
		Particle part = sim->parts[parts[i]];
		if (part.x != partx || part.y != party)
			samePos = false;
		bool xgood = (position2.X > position.X) == (part.x > stackPos.X);
		bool ygood = (position2.Y > position.Y) == (part.y > stackPos.Y);
		if (xgood)
			stackPos.X = part.x;
		if (ygood)
			stackPos.Y = part.y;
	}
	if (samePos)
	{
		auto &sd = SimulationData::CRef();
		auto &elements = sd.elements;
		ui::Point unstackDir(0, 1);
		if (position2.X == position.X) {
			if (position2.Y < position.Y)
				unstackDir = ui::Point(0, -1);
			if (position2.Y > position.Y)
				unstackDir = ui::Point(0, 1);
		}
		if (position2.Y == position.Y) {
			if (position2.X < position.X)
				unstackDir = ui::Point(-1, 0);
			if (position2.X > position.X)
				unstackDir = ui::Point(1, 0);
		}
		unsigned int unstackLimit = parts.size();
		for (size_t i = 1; i < parts.size(); i++){
			int nx = partx + i * unstackDir.X, ny = party + i * unstackDir.Y;
			if (nx < 0 || nx >= XRES || ny < 0 || ny >= YRES ||
				sim->pmap[ny][nx] || sim->photons[ny][nx])
			{
				unstackLimit = i;
				if (!sim->stackToolNotifShown || !(
					sim->stackToolNotifShownX == position.X &&
					sim->stackToolNotifShownY == position.Y
				))
				{
					gameModel.Log("Warning: Not enough space to unstack fully.", false);
					sim->stackToolNotifShown = true;
					sim->stackToolNotifShownX = position.X;
					sim->stackToolNotifShownY = position.Y;
				}
				break;
			}
		}
		bool reverseOrder = unstackDir.X < 0 || unstackDir.Y < 0;
		for (size_t i = 0; i < unstackLimit; i++){
			int partidx = reverseOrder ?
				(unstackLimit - i - 1) : (parts.size() - unstackLimit + i);
			int partID = parts[partidx];
			Particle *part = &sim->parts[partID];
			part->x += (int)i * unstackDir.X;
			part->y += (int)i * unstackDir.Y;
			int nx = (int)(part->x + 0.5f), ny = (int)(part->y + 0.5f);
			int t = part->type;
			if (elements[t].Properties & TYPE_ENERGY)
				sim->photons[ny][nx] = PMAP(partID, t);
			else
				sim->pmap[ny][nx] = PMAP(partID, t);
		}
	}
	else
	{
		if (parts.size() > 5)
		{
			gameModel.Log("Warning: More than 5 stacked particles.", false);
		}
		Particle *partobjs = new Particle[parts.size()];
		for (size_t i = 0; i < parts.size(); i++)
			partobjs[i] = sim->parts[parts[i]];
		std::sort(partobjs, partobjs + parts.size(), compareParts);
		for (size_t i = 0; i < parts.size(); i++)
			sim->parts[parts[i]] = partobjs[i];
		for (size_t i = 0; i < parts.size(); i++)
		{
			sim->parts[parts[i]].x = stackPos.X;
			sim->parts[parts[i]].y = stackPos.Y;
		}
		delete[] partobjs;
	}
}

void StackTool::Draw(Simulation *sim, Brush *cBrush, ui::Point position)
{
	if (cBrush)
	{
		std::vector<int> parts;
		for (int i=0; i<=sim->parts_lastActiveIndex; i++)
		{
			if (sim->parts[i].type)
			{
				int partx = (int)(sim->parts[i].x+0.5f);
				int party = (int)(sim->parts[i].y+0.5f);
				if (cBrush->HasPoint(ui::Point(partx, party) - position))
					parts.push_back(i);
			}
		}
		ProcessParts(sim, parts, position, position);
	}
}

void StackTool::DrawLine(Simulation *sim, Brush *cBrush, ui::Point position, ui::Point position2, bool dragging)
{
	std::vector<ui::Point> points;
	int x1 = position.X, y1 = position.Y, x2 = position2.X, y2 = position2.Y;
	bool reverseXY = abs(y2-y1) > abs(x2-x1);
	int x, y, dx, dy, sy, rx = cBrush->GetRadius().X, ry = cBrush->GetRadius().Y;
	float e = 0.0f, de;
	if (reverseXY)
	{
		y = x1;
		x1 = y1;
		y1 = y;
		y = x2;
		x2 = y2;
		y2 = y;
	}
	if (x1 > x2)
	{
		y = x1;
		x1 = x2;
		x2 = y;
		y = y1;
		y1 = y2;
		y2 = y;
	}
	dx = x2 - x1;
	dy = abs(y2 - y1);
	if (dx)
		de = dy/(float)dx;
	else
		de = 0.0f;
	y = y1;
	sy = (y1<y2) ? 1 : -1;
	for (x=x1; x<=x2; x++)
	{
		if (reverseXY)
			points.push_back(ui::Point(y, x));
		else
			points.push_back(ui::Point(x, y));
		e += de;
		if (e >= 0.5f)
		{
			y += sy;
			if (!(rx+ry) && ((y1<y2) ? (y<=y2) : (y>=y2)))
			{
				if (reverseXY)
					points.push_back(ui::Point(y, x));
				else
					points.push_back(ui::Point(x, y));
			}
			e -= 1.0f;
		}
	}
	std::sort(points.begin(), points.end(), comparePoints);
	std::vector<int> parts;
	for (int i=0; i<=sim->parts_lastActiveIndex; i++)
	{
		if (sim->parts[i].type)
		{
			int partx = (int)(sim->parts[i].x+0.5f);
			int party = (int)(sim->parts[i].y+0.5f);
			ui::Point partpos(partx, party);
			std::vector<ui::Point>::iterator it = std::lower_bound(points.begin(), points.end(), partpos, comparePoints);
			if (it != points.end() && *it == partpos)
				parts.push_back(i);
		}
	}
	ProcessParts(sim, parts, position, position2);
}

void StackTool::DrawRect(Simulation *sim, Brush *cBrush, ui::Point position, ui::Point position2)
{
	int x1 = position.X, y1 = position.Y, x2 = position2.X, y2 = position2.Y;
	int i, j;
	if (x1>x2)
	{
		i = x2;
		x2 = x1;
		x1 = i;
	}
	if (y1>y2)
	{
		j = y2;
		y2 = y1;
		y1 = j;
	}
	std::vector<int> parts;
	for (int i=0; i<=sim->parts_lastActiveIndex; i++)
	{
		if (sim->parts[i].type)
		{
			int partx = (int)(sim->parts[i].x+0.5f);
			int party = (int)(sim->parts[i].y+0.5f);
			if (partx >= x1 && partx <= x2 && party >= y1 && party <= y2)
				parts.push_back(i);
		}
	}
	ProcessParts(sim, parts, position, position2);
}
