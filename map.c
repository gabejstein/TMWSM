#include "map.h"
#include <stdio.h>
#include "main.h"
#include "objectfactory.h"

static void RenderTile(int type, int x, int y);
static void LoadEntities(FILE* f);

Stage* stage;

SDL_Texture* tileSpriteSet;

void LoadMap(char* path)
{
	stage = (Stage*)malloc(sizeof(Stage));
	memset(stage, '0', sizeof(Stage));

	FILE* f = fopen(path, "r");
	if (!f) { printf("Could not load map %s\n", path); exit(1); }

	//TODO: Find way to deal with variable number of layers
	int currLayer = 0;
	int row, col;

	for (currLayer = 0; currLayer < 2;currLayer++)
	{
		for (row = 0; row < MAP_HEIGHT; row++)
		{
			for (col = 0; col < MAP_WIDTH; col++)
			{
				int tile = 0;
				fscanf(f, "%d,", &tile);
				stage->tiles[currLayer][row * MAP_WIDTH + col] = tile;
			}
		}
		fgetc(f); //skip newline
		//fgetc(f); //skip newline
	}
	fgetc(f);
	
	LoadEntities(f);

	fclose(f);

	tileSpriteSet = GetTexture("tileset_01");
}

static void LoadEntities(FILE* f)
{
	
	char buffer[256];
	Vec2 point;
	do {
		fscanf(f, "%s %f,%f\n", buffer, &point.x, &point.y);
		
		CreateObject(buffer, point.x, point.y);
		
	} while (!feof(f));

}

void RenderMap(void)
{
	int row, col, curLayer;

	for (curLayer = 0; curLayer < 2; curLayer++)
	{
		for (row = -1; row < TILES_ON_SCREEN_H+1; row++)
		{
			for (col = -1; col < TILES_ON_SCREEN_W+1; col++)
			{
				int tx = (int)game.camera.x / TILE_SIZE;
				int ty = (int)game.camera.y / TILE_SIZE;
				int tile = GetTile(col + tx, row + ty, curLayer);
				if (tile > -1)
					RenderTile(tile, col, row);
			}
		}
	}
	
}

void UnloadMap(void)
{
	printf("Unloading Map\n");
	free(stage);
}

int GetTile(int x, int y, int layer)
{
	int tile;
	if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT)
	{
		tile = stage->tiles[layer][y * MAP_WIDTH + x];
		return tile;
	}
	
	tile = -1;
	return tile;
}

int IsCollisionTile(int x, int y, int layer)
{
	int tile = GetTile(x, y, layer);

	if (tile == 1)
	{
		return 1;
	}

	return 0;
}

int IsOneWayCollider(int x, int y, int layer)
{
	int tile = GetTile(x, y, layer);

	if (tile == 2)
	{
		return 1;
	}

	return 0;
}

static void RenderTile(int type, int x, int y)
{
	int tileSetRows = 4; //Number of horizontal tiles in sprite. TODO: Figure out when loading tileset.
	int tileSetCols = 4; //Number of vertical tiles in sprite.

	int spriteRow =  type / tileSetRows;
	int spriteCol =  type % tileSetCols;

	SDL_Rect tileSRC = { spriteCol * TILE_SIZE, spriteRow * TILE_SIZE, TILE_SIZE, TILE_SIZE };
	SDL_Rect tileDest = { x * TILE_SIZE, y * TILE_SIZE,TILE_SIZE,TILE_SIZE };
	tileDest.x -= (int) game.camera.x % TILE_SIZE;
	tileDest.y -= (int) game.camera.y % TILE_SIZE;

	SDL_RenderCopy(game.renderer, tileSpriteSet, &tileSRC, &tileDest);
}