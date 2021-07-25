#ifndef TOOL_H_
#define TOOL_H_
#include "Config.h"

#include "common/String.h"
#include "gui/interface/Point.h"
#include "simulation/StructProperty.h"
#include "simulation/Particle.h"
#include "simulation/Sample.h"
#include "graphics/Renderer.h"

class Simulation;
class Brush;
class VideoBuffer;

class Tool
{
protected:
	VideoBuffer * (*textureGen)(int, int, int);
	int toolID;
	String toolName;
	String toolDescription;
	float strength;
	bool blocky;
	ByteString identifier;
public:
	int colRed, colGreen, colBlue;

	Tool(int id, String name, String description, int r, int g, int b, ByteString identifier, VideoBuffer * (*textureGen)(int, int, int) = NULL);
	int GetToolID() { return toolID; }
	String GetName();
	String GetDescription();
	ByteString GetIdentifier();
	int GetBlocky() { return blocky; }
	void SetStrength(float value) { strength = value; }
	float  GetStrength() { return strength; }
	VideoBuffer * GetTexture(int width, int height);
	void SetTextureGen(VideoBuffer * (*textureGen)(int, int, int));
	virtual ~Tool();
	virtual void Click(Simulation * sim, Brush * brush, ui::Point position);
	virtual void Draw(Simulation * sim, Brush * brush, ui::Point position);
	virtual void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false);
	virtual void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2);
	virtual void DrawFill(Simulation * sim, Brush * brush, ui::Point position);
};

class GameModel;

class SignTool: public Tool
{
public:
	GameModel * gameModel;
	SignTool(GameModel *model):
	Tool(0, "SIGN", "Sign. Displays text. Click on a sign to edit it or anywhere else to place a new one.", 0, 0, 0, "DEFAULT_UI_SIGN", SignTool::GetIcon),
	  gameModel(model)
	{
	}
	static VideoBuffer * GetIcon(int toolID, int width, int height);
	virtual ~SignTool() {}
	void Click(Simulation * sim, Brush * brush, ui::Point position) override;
	void Draw(Simulation * sim, Brush * brush, ui::Point position) override { }
	void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) override { }
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override { }
};

class SampleTool: public Tool
{
	GameModel * gameModel;
public:
	SampleTool(GameModel *model):
	Tool(0, "SMPL", "Sample an element on the screen.", 0, 0, 0, "DEFAULT_UI_SAMPLE", SampleTool::GetIcon),
	gameModel(model)
	{
	}
	static VideoBuffer * GetIcon(int toolID, int width, int height);
	virtual ~SampleTool() {}
	void Click(Simulation * sim, Brush * brush, ui::Point position) override { }
	void Draw(Simulation * sim, Brush * brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) override { }
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override { }
};

class StackTool: public Tool
{
	GameModel * gameModel;
public:
	StackTool(GameModel *model):
	Tool(0, "STCK", "Stack or unstack particles.", 0xff, 0xff, 0, "DEFAULT_UI_STACK", NULL),
	gameModel(model)
	{
	}
	virtual ~StackTool() {}
	void ProcessParts(Simulation * sim, std::vector<int> &parts, ui::Point position, ui::Point position2);
	virtual void Click(Simulation * sim, Brush * brush, ui::Point position) { }
	virtual void Draw(Simulation * sim, Brush * brush, ui::Point position);
	virtual void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false);
	virtual void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2);
	virtual void DrawFill(Simulation * sim, Brush * brush, ui::Point position) { }
};

class ConfigTool: public Tool
{
	class ReleaseTool : public Tool
	{
		ConfigTool * configTool;
		Tool * clearTool;
	public:
		ReleaseTool(ConfigTool *configTool_):
		Tool(0, "", "", 0, 0, 0, "DEFAULT_UI_CONFIG_RELEASE", NULL),
		configTool(configTool_),
		clearTool(NULL)
		{
		}
		virtual ~ReleaseTool() {}
		void SetClearTool(Tool *clearTool_) { clearTool = clearTool_; }
		virtual void Click(Simulation * sim, Brush * brush, ui::Point position);
		virtual void Draw(Simulation * sim, Brush * brush, ui::Point position);
		virtual void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false);
		virtual void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2);
		virtual void DrawFill(Simulation * sim, Brush * brush, ui::Point position);
	};

	enum struct ConfigState
	{
		ready,
		drayTmp,
		drayTmp2,
		crayTmp,
		crayTmp2,
		dtecTmp2,
		tsnsTmp2,
		lsnsTmp2,
		convTmp,
		ldtcTmp,
		ldtcLife
	};
	GameModel * gameModel;
	int configPartId;
	Particle configPart;
	int lastAdjacentPartsInfo[3][3];
	int dirx, diry;
	ui::Point cursorPos;
	ConfigState configState;
public:
	ReleaseTool releaseTool;
	ConfigTool(GameModel *model):
	Tool(0, "CNFG", "Quickly configure particle properties.", 0xff, 0xcc, 0, "DEFAULT_UI_CONFIG", NULL),
	gameModel(model),
	cursorPos(0, 0),
	configState(ConfigState::ready),
	releaseTool(ReleaseTool(this))
	{
	}
	virtual ~ConfigTool() {}
	void SetClearTool(Tool *clearTool) { releaseTool.SetClearTool(clearTool); }
	bool IsCorrupted(Simulation * sim); // check if configPart moved or disappeared
	void Reset(Simulation * sim);
	Particle GetPart();
	int GetId();
	static bool IsConfigurableType(int type);
	bool IsConfiguring();
	bool IsConfiguringTemp();
	bool IsConfiguringLife();
	bool IsConfiguringTmp();
	bool IsConfiguringTmp2();
	void Update(Simulation *sim);
	void DrawHUD(Renderer *ren);
	void OnSelectFiltTmp(Simulation *sim, int tmp);
	virtual void Click(Simulation * sim, Brush * brush, ui::Point position);
	virtual void Draw(Simulation * sim, Brush * brush, ui::Point position) { }
	virtual void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) { }
	virtual void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) { }
	virtual void DrawFill(Simulation * sim, Brush * brush, ui::Point position) { }
private:
	int getIdAt(Simulation *sim, ui::Point position);
	Particle getPartAt(Simulation *sim, ui::Point position);
	bool isSamePart(Particle p1, Particle p2);
	ui::Point projectPoint(Particle part, int sampleX, int sampleY, bool allowDiag = true);
	int getDist(ui::Point relPos, int offset = 0);
	int getDist(Particle part, int sampleX, int sampleY, int offset = 0, bool allowDiag = true);
	int getTargetStackEditDepth(SimulationSample *sample);
	void drawRedLine(Renderer *ren, int startx, int starty, int endx, int endy);
	void drawWhiteLine(Renderer *ren, int startx, int starty, int endx, int endy);
	void drawTripleLine(Renderer *ren, int firstLineLen, int midLineLen, bool drawFirstLine = true, bool drawThirdLine = true);
	void drawSquareRdBox(Renderer *ren);
};

class PropertyTool: public Tool
{
public:
	GameModel * gameModel;
	PropertyTool(GameModel *model):
	Tool(0, "PROP", "Property Drawing Tool. Use to alter the properties of elements in the field.", 0xfe, 0xa9, 0x00, "DEFAULT_UI_PROPERTY", NULL),
	gameModel(model)
	{
	}
	StructProperty::PropertyType propType;
	PropertyValue propValue;
	bool changeType;
	size_t propOffset;
	bool validProperty;

	void OpenWindow(Simulation *sim);
	virtual ~PropertyTool() {}
	virtual void SetProperty(Simulation *sim, ui::Point position);
	void Click(Simulation * sim, Brush * brush, ui::Point position) override { }
	void Draw(Simulation *sim, Brush *brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override;
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override;
};

class GOLTool: public Tool
{
public:
	GameModel * gameModel;
	GOLTool(GameModel * gameModel):
	Tool(0, "CUST", "Add a new custom GOL type. (Use ctrl+shift+rightclick to remove them)", 0xfe, 0xa9, 0x00, "DEFAULT_UI_ADDLIFE", NULL),
	gameModel(gameModel)
	{
	}
	void OpenWindow(Simulation *sim, int toolSelection, int rule = 0, int colour1 = 0, int colour2 = 0);
	virtual ~GOLTool() {}
	void Click(Simulation * sim, Brush * brush, ui::Point position) override { }
	void Draw(Simulation *sim, Brush *brush, ui::Point position) override { };
	void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) override { };
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override { };
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override { };
};


class ElementTool: public Tool
{
public:
	ElementTool(int id, String name, String description, int r, int g, int b, ByteString identifier, VideoBuffer * (*textureGen)(int, int, int) = NULL);
	virtual ~ElementTool();
	void Draw(Simulation * sim, Brush * brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override;
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override;
};

class Element_LIGH_Tool: public ElementTool
{
public:
	Element_LIGH_Tool(int id, String name, String description, int r, int g, int b, ByteString identifier, VideoBuffer * (*textureGen)(int, int, int) = NULL):
		ElementTool(id, name, description, r, g, b, identifier, textureGen)
	{ }
	virtual ~Element_LIGH_Tool() { }
	void Click(Simulation * sim, Brush * brush, ui::Point position) override { }
	void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override { }
};

class Element_TESC_Tool: public ElementTool
{
public:
	Element_TESC_Tool(int id, String name, String description, int r, int g, int b, ByteString identifier, VideoBuffer * (*textureGen)(int, int, int) = NULL):
		ElementTool(id, name, description, r, g, b, identifier, textureGen)
	{ }
	virtual ~Element_TESC_Tool() {}
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override;
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override;
};

class PlopTool: public ElementTool
{
public:
	PlopTool(int id, String name, String description, int r, int g, int b, ByteString identifier, VideoBuffer * (*textureGen)(int, int, int) = NULL):
		ElementTool(id, name, description, r, g, b, identifier, textureGen)
	{ }
	virtual ~PlopTool() { }
	void Draw(Simulation * sim, Brush * brush, ui::Point position) override { }
	void Click(Simulation * sim, Brush * brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) override { }
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override { }
};

class WallTool: public Tool
{
public:
	WallTool(int id, String name, String description, int r, int g, int b, ByteString identifier, VideoBuffer * (*textureGen)(int, int, int) = NULL);
	virtual ~WallTool();
	void Draw(Simulation * sim, Brush * brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override;
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override;
};

class WindTool: public Tool
{
public:
	WindTool(int id, String name, String description, int r, int g, int b, ByteString identifier, VideoBuffer * (*textureGen)(int, int, int) = NULL);
	virtual ~WindTool() { }
	void Draw(Simulation * sim, Brush * brush, ui::Point position) override { }
	void DrawLine(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush * brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush * brush, ui::Point position) override { }
};

#endif /* TOOL_H_ */
