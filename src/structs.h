#ifndef STRUCTS_H
#define STRUCTS_H

#include "main.h"
#include "system/vector.h"
#include "system/graphics.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct Entity Entity;

typedef struct
{
	bool hasCollision;
	float x;
	float y;
}TileCollision;

struct Entity
{
	Vec2 pos;
	Vec2 vel;
	int w;
	int h;
	int isActive;
	SDL_Texture* texture;
	void (*update)(Entity* self);
	void (*render)(Entity* self);
	void (*onHit)(Entity* self, Entity* other);
	void (*cleanup)(Entity* self);
	void (*onTileHit)(Entity* self);
	void(*data); //For extending members
	SDL_Rect collider;
	int isHit;
	DIRECTION direction;
	int health;
	TAG tag;
	int isGrounded;
	int isSolid;
	int weightless;
	float lastTime;
	int scoreValue; //how much a powerup/enemy is worth. might separate out.
};

typedef struct
{
	unsigned int score;
	unsigned int ammo;
	KEY keys[MAX_KEYS];
}PlayerObject;

//Enemy Sentry
typedef struct
{
	Vec2 dest; //point to move to
	int canSeePlayer;
	int isDamaged;
	float lastDamaged;
	AnimatedSprite walkingAnimation;
}Sentry;

typedef struct
{
	int tiles[MAX_MAP_LAYERS][(MAP_WIDTH * MAP_HEIGHT)];
	char music[MAX_FILENAME];
}Stage;

typedef struct
{
	unsigned int currentLevel;
}UserData;

typedef struct
{
	uint8_t debug : 1;
	uint8_t soundOn : 1;
	uint8_t musicOn : 1;
}Settings;

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	float deltaTime;
	bool isRunning;
	void (*update)();
	void (*render)();
	void (*cleanup)();
	Vec2 camera;
	Settings settings;
	unsigned int levelCount;
	char** levelQueue; //stores filepaths for each level
}Game;

#endif 
