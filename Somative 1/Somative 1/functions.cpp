#include "functions.h"
#include <fstream>
#include <string>
#include <vector>

bool box[TILEMAP_HEIGHT * TILEMAP_WIDTH] = { 0 };
int tiles[TILEMAP_HEIGHT * TILEMAP_WIDTH] = { 0 };

//function to save field
void save()
{
	std::ofstream save;
	std::ofstream tile_type_save;

	save.open("map.txt");
	while (save.is_open())
	{
		for (const auto element : box)
		{
			if (element)
			{
				save << "1\n";
			}
			else
			{
				save << "0\n";
			}
		}
		save.close();
	}

	tile_type_save.open("tile.txt");
	while (tile_type_save.is_open())
	{
		for (const auto element : tiles)
		{
			switch (element)
			{
			case 0:
				tile_type_save << "0\n";
				break;
			case 1:
				tile_type_save << "1\n";
				break;
			case 2:
				tile_type_save << "2\n";
				break;
			case 3:
				tile_type_save << "3\n";
				break;
				default:
					break;
			}
		}
		tile_type_save.close();
	}
}

//function to load the field
void load()
{
	std::string tile;

	std::ifstream file;
	file.open("map.txt");

	int i = 0;
	while (file.is_open())
	{
		while (file)
		{
			getline(file, tile);

			if (tile == "1")
			{
				box[i] = true;
				i++;
			}
			else
			{
				i++;
			}
		}
		file.close();
	}

	std::string tile_type;

	std::ifstream save;
	save.open("tile.txt");

	int location = 0;
	while (save.is_open())
	{
		while (save)
		{
			getline(save, tile_type);

			if (tile_type == "0")
			{
				tiles[location] = 0;
				location++;
			}
			else if(tile_type == "1")
			{
				tiles[location] = 1;
				location++;
			}
			else if (tile_type == "2")
			{
				tiles[location] = 2;
				location++;
			}
			else if (tile_type == "3")
			{
				tiles[location] = 3;
				location++;
			}
			else
			{
				location++;
			}
		}
		save.close();
	}

}
