#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 768

#define FPS 60
#define MILLISECS_PER_FRAME (1000/FPS)

#define MAX_MAP_LAYERS 3
#define MAP_WIDTH 60
#define MAP_HEIGHT 30

#define TILE_SIZE 64
#define INV_TILE_SIZE (1.0 / TILE_SIZE)

#define TILES_ON_SCREEN_W 20
#define TILES_ON_SCREEN_H 12

enum
{
	RIGHT,
	LEFT
};

typedef enum
{
	TAG_PLAYER,
	TAG_ENEMY,
	TAG_DOOR,
	TAG_KEY,
	TAG_PLAYER_BULLET,
	TAG_ENEMY_BULLET,
	TAG_PICKUP
}TAG;

#define MAX_KEYS 3

typedef enum
{
	BLUE_KEY,
	RED_KEY,
	GREEN_KEY
}KEY;

#endif