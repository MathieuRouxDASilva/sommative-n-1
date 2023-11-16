#pragma once
#include <vector>
#include "functions.h"

class Tile
{
protected:
	enum TypeOfTile
	{
		kAir,
		kGround,
		kPlateform,
		kEndTile,
	};
	TypeOfTile selected_tile_number_ = kAir;

public:
	void ChoseTile();

	TypeOfTile tile_map_[TILEMAP_HEIGHT * TILEMAP_WIDTH];

	TypeOfTile GetSelectedNumber();
	TypeOfTile GetKairTile();
	TypeOfTile GetKgroundTile();
	TypeOfTile GetKplateformTile();
	TypeOfTile GetKendTile();
};

