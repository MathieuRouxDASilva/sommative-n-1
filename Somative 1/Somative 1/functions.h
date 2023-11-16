#pragma once

#define TILEMAP_WIDTH 200
#define TILEMAP_HEIGHT 50
#define TILE_SIZE_PX 50
#define PLATEFORM_HEIGHT_SETUPP 18
#define EDGE_MAP_RESPAWN 1500
#define EDGE_MAP_LENGTH 5000
#define EDGE_MAP_HEIGHT 2000

extern bool box[TILEMAP_HEIGHT * TILEMAP_WIDTH];
extern int tiles[TILEMAP_HEIGHT * TILEMAP_WIDTH];



void save();
void load();

