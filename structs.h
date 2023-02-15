#ifndef STRUCTS_H
#define STRUCTS_H

#include "main.h"
#include "vector.h"

#define MAX_KEYBOARD 256

typedef struct Entity Entity;

struct Entity
{
	Vec2 pos;
	Vec2 vel;
	int w;
	int h;
	int isActive;
	SDL_Texture* texture;
	void (*update)(Entity* self);
	void (*render)(void);
	void (*onHit)(Entity* self, Entity* other);
	void(*data); //For extending members
	SDL_Rect collider;
	int isHit;
	int direction;
	int health;
	TAG tag;
	int isGrounded;
	int weightless;
};

//Enemy Sentry
typedef struct
{
	Vec2 dest; //point to move to
	int canSeePlayer;
}Sentry;

typedef struct
{
	int tiles[MAX_MAP_LAYERS][(MAP_WIDTH * MAP_HEIGHT)];
	
}Stage;

typedef struct
{
	int keys[MAX_KEYBOARD];
}Input;

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	float deltaTime;
	int isRunning;
	void (*update)();
	void (*render)();
	void (*cleanup)();
	Input input;
	Vec2 camera;
	int debug;
}Game;

#endif 
