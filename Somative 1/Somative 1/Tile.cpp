#include "Tile.h"
#include <SFML/Graphics.hpp>

void Tile::ChoseTile()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
	{
		selected_tile_number_ = kAir;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
	{
		selected_tile_number_ = kGround;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
	{
		selected_tile_number_ = kPlateform;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
	{
		selected_tile_number_ = kEndTile;
	}
}

Tile::TypeOfTile Tile::GetSelectedNumber()
{
	return selected_tile_number_;
}

Tile::TypeOfTile Tile::GetKairTile()
{
	return kAir;
}

Tile::TypeOfTile Tile::GetKgroundTile()
{
	return kGround;
}

Tile::TypeOfTile Tile::GetKplateformTile()
{
	return kPlateform;
}

Tile::TypeOfTile Tile::GetKendTile()
{
	return kEndTile;
}
