#pragma once
#include "common/String.h"
#include "common/Vec2.h"
#include "graphics/Pixel.h"
#include "gui/interface/Point.h"
#include "simulation/StructProperty.h"
#include "simulation/Particle.h"
#include "simulation/Sample.h"
#include "graphics/Renderer.h"
#include <memory>
#include <optional>

class Simulation;
class Brush;
class VideoBuffer;
struct Particle;

class Tool
{
private:
	std::unique_ptr<VideoBuffer> (*const textureGen)(int, Vec2<int>);

public:
	int const ToolID;
	String const Name;
	String const Description;
	ByteString const Identifier;
	RGB<uint8_t> const Colour;
	bool const Blocky;
	float Strength = 1.0f;
	bool shiftBehaviour = false;
	bool ctrlBehaviour = false;
	bool altBehaviour = false;

	Tool(int id, String name, String description,
		RGB<uint8_t> colour, ByteString identifier, std::unique_ptr<VideoBuffer> (*textureGen)(int, Vec2<int>) = NULL, bool blocky = false
	):
		textureGen(textureGen),
		ToolID(id),
		Name(name),
		Description(description),
		Identifier(identifier),
		Colour(colour),
		Blocky(blocky)
	{}

	virtual ~Tool()
	{}

	std::unique_ptr<VideoBuffer> GetTexture(Vec2<int>);
	virtual void Click(Simulation * sim, Brush const &brush, ui::Point position);
	virtual void Draw(Simulation * sim, Brush const &brush, ui::Point position);
	virtual void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false);
	virtual void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2);
	virtual void DrawFill(Simulation * sim, Brush const &brush, ui::Point position);
};

class GameModel;

class SignTool: public Tool
{
	GameModel &gameModel;

	friend class SignWindow;

public:
	SignTool(GameModel &model):
		Tool(0, "SIGN", "Sign. Displays text. Click on a sign to edit it or anywhere else to place a new one.",
			0x000000_rgb, "DEFAULT_UI_SIGN", SignTool::GetIcon
		),
		gameModel(model)
	{}

	virtual ~SignTool()
	{}

	static std::unique_ptr<VideoBuffer> GetIcon(int toolID, Vec2<int> size);
	void Click(Simulation * sim, Brush const &brush, ui::Point position) override;
	void Draw(Simulation * sim, Brush const &brush, ui::Point position) override { }
	void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false) override { }
	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override { }
};

class SampleTool: public Tool
{
	GameModel &gameModel;

public:
	SampleTool(GameModel &model):
		Tool(0, "SMPL", "Sample an element on the screen.",
			0x000000_rgb, "DEFAULT_UI_SAMPLE", SampleTool::GetIcon
		),
		gameModel(model)
	{}

	static std::unique_ptr<VideoBuffer> GetIcon(int toolID, Vec2<int> size);
	virtual ~SampleTool() {}
	void Click(Simulation * sim, Brush const &brush, ui::Point position) override { }
	void Draw(Simulation * sim, Brush const &brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false) override { }
	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override { }
};

class StackTool: public Tool
{
	GameModel &gameModel;
public:
	StackTool(GameModel &model):
	Tool(0, "STCK", "Stack or unstack particles.", 0xffff00_rgb, "DEFAULT_UI_STACK", NULL),
	gameModel(model)
	{
	}
	virtual ~StackTool() {}
	void ProcessParts(Simulation * sim, std::vector<int> &parts, ui::Point position, ui::Point position2);
	virtual void Click(Simulation * sim, Brush const &brush, ui::Point position) { }
	virtual void Draw(Simulation * sim, Brush const &brush, ui::Point position);
	virtual void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false);
	virtual void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2);
	virtual void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) { }
};

class ConfigTool: public Tool
{
	class ReleaseTool : public Tool
	{
		ConfigTool * configTool;
		Tool * clearTool;
	public:
		ReleaseTool(ConfigTool *configTool_):
		Tool(0, "", "", 0x000000_rgb, "DEFAULT_UI_CONFIG_RELEASE"),
		configTool(configTool_),
		clearTool(NULL)
		{
		}
		virtual ~ReleaseTool() {}
		void SetClearTool(Tool *clearTool_) { clearTool = clearTool_; }
		virtual void Click(Simulation * sim, const Brush &brush, ui::Point position);
		virtual void Draw(Simulation * sim, const Brush &brush, ui::Point position);
		virtual void DrawLine(Simulation * sim, const Brush &brush, ui::Point position1, ui::Point position2, bool dragging = false);
		virtual void DrawRect(Simulation * sim, const Brush &brush, ui::Point position1, ui::Point position2);
		virtual void DrawFill(Simulation * sim, const Brush &brush, ui::Point position);
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
		vsnsTmp2,
		convTmp,
		ldtcTmp,
		ldtcLife,
	};
	GameModel &gameModel;
	int configPartId;
	Particle configPart;
	int lastAdjacentPartsInfo[3][3];
	int dirx, diry;
	ui::Point cursorPos;
	ConfigState configState;
public:
	ReleaseTool releaseTool;
	ConfigTool(GameModel &model):
	Tool(0, "CNFG", "Quickly configure particle properties.", 0xffcc00_rgb, "DEFAULT_UI_CONFIG"),
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
	virtual void Click(Simulation * sim, const Brush &brush, ui::Point position);
	virtual void Draw(Simulation * sim, const Brush &brush, ui::Point position) { }
	virtual void DrawLine(Simulation * sim, const Brush &brush, ui::Point position1, ui::Point position2, bool dragging = false) { }
	virtual void DrawRect(Simulation * sim, const Brush &brush, ui::Point position1, ui::Point position2) { }
	virtual void DrawFill(Simulation * sim, const Brush &brush, ui::Point position) { }
private:
	int getIdAt(Simulation *sim, ui::Point position);
	Particle getPartAt(Simulation *sim, ui::Point position);
	bool isSamePart(Particle p1, Particle p2);
	ui::Point projectPoint(Particle part, int sampleX, int sampleY, bool allowDiag = true);
	int getDist(ui::Point relPos, int offset = 0);
	int getDist(Particle part, int sampleX, int sampleY, int offset = 0, bool allowDiag = true);
	int getTargetStackEditDepth(SimulationSample *sample);
	void drawRedLine(Renderer *ren, ui::Point start, ui::Point end);
	void drawWhiteLine(Renderer *ren, ui::Point start, ui::Point end);
	void drawTripleLine(Renderer *ren, int firstLineLen, int midLineLen, bool drawFirstLine = true, bool drawThirdLine = true);
	void drawSquareRdBox(Renderer *ren);
};

class PropertyTool: public Tool
{
public:
	struct Configuration
	{
		StructProperty prop;
		PropertyValue propValue;
		bool changeType;
		int propertyIndex;
		String propertyValueStr;
	};

private:
	void SetProperty(Simulation *sim, ui::Point position);
	void SetConfiguration(std::optional<Configuration> newConfiguration);

	GameModel &gameModel;
	std::optional<Configuration> configuration;

	friend class PropertyWindow;

public:
	PropertyTool(GameModel &model):
		Tool(0, "PROP", "Property Drawing Tool. Use to alter the properties of elements in the field.",
			0xFEA900_rgb, "DEFAULT_UI_PROPERTY", NULL
		),
		gameModel(model)
	{}

	virtual ~PropertyTool()
	{}

	void OpenWindow(Simulation *sim, const Particle *takePropertyFrom);
	void Click(Simulation * sim, Brush const &brush, ui::Point position) override { }
	void Draw(Simulation *sim, Brush const &brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override;
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override;

	std::optional<Configuration> GetConfiguration() const
	{
		return configuration;
	}
};

class GOLTool: public Tool
{
	GameModel &gameModel;
public:
	GOLTool(GameModel &gameModel):
		Tool(0, "CUST", "Add a new custom GOL type. (Use ctrl+shift+rightclick to remove them)",
			0xFEA900_rgb, "DEFAULT_UI_ADDLIFE", NULL
		),
		gameModel(gameModel)
	{}

	virtual ~GOLTool()
	{}

	void OpenWindow(Simulation *sim, int toolSelection, int rule = 0, RGB<uint8_t> colour1 = 0x000000_rgb, RGB<uint8_t> colour2 = 0x000000_rgb);
	void Click(Simulation * sim, Brush const &brush, ui::Point position) override { }
	void Draw(Simulation *sim, Brush const &brush, ui::Point position) override { };
	void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false) override { };
	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override { };
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override { };
};


class ElementTool: public Tool
{
public:
	ElementTool(int id, String name, String description,
		RGB<uint8_t> colour, ByteString identifier, std::unique_ptr<VideoBuffer> (*textureGen)(int, Vec2<int>) = NULL):
		Tool(id, name, description, colour, identifier, textureGen)
	{}

	virtual ~ElementTool()
	{}

	void Draw(Simulation * sim, Brush const &brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override;
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override;
};

class Element_LIGH_Tool: public ElementTool
{
public:
	Element_LIGH_Tool(int id, String name, String description,
		RGB<uint8_t> colour, ByteString identifier, std::unique_ptr<VideoBuffer> (*textureGen)(int, Vec2<int>) = NULL):
		ElementTool(id, name, description, colour, identifier, textureGen)
	{}

	virtual ~Element_LIGH_Tool()
	{}

	void Click(Simulation * sim, Brush const &brush, ui::Point position) override { }
	void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override { }
};

class Element_TESC_Tool: public ElementTool
{
public:
	Element_TESC_Tool(int id, String name, String description,
		RGB<uint8_t> colour, ByteString identifier, std::unique_ptr<VideoBuffer> (*textureGen)(int, Vec2<int>) = NULL):
		ElementTool(id, name, description, colour, identifier, textureGen)
	{}

	virtual ~Element_TESC_Tool()
	{}

	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override;
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override;
};

class PlopTool: public ElementTool
{
public:
	PlopTool(int id, String name, String description,
		RGB<uint8_t> colour, ByteString identifier, std::unique_ptr<VideoBuffer> (*textureGen)(int, Vec2<int>) = NULL):
		ElementTool(id, name, description, colour, identifier, textureGen)
	{}

	virtual ~PlopTool()
	{}

	void Draw(Simulation * sim, Brush const &brush, ui::Point position) override { }
	void Click(Simulation * sim, Brush const &brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false) override { }
	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override { }
};

class WallTool: public Tool
{
public:
	WallTool(int id, String description,
		RGB<uint8_t> colour, ByteString identifier, std::unique_ptr<VideoBuffer> (*textureGen)(int, Vec2<int>) = NULL):
		Tool(id, "", description, colour, identifier, textureGen, true)
	{
	}

	virtual ~WallTool()
	{}

	void Draw(Simulation * sim, Brush const &brush, ui::Point position) override;
	void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override;
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override;
};

class WindTool: public Tool
{
public:
	WindTool():
		Tool(0, "WIND", "Creates air movement.",
			0x404040_rgb, "DEFAULT_UI_WIND")
	{}

	virtual ~WindTool()
	{}

	void Draw(Simulation * sim, Brush const &brush, ui::Point position) override { }
	void DrawLine(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2, bool dragging = false) override;
	void DrawRect(Simulation * sim, Brush const &brush, ui::Point position1, ui::Point position2) override { }
	void DrawFill(Simulation * sim, Brush const &brush, ui::Point position) override { }
};
