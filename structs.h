#ifndef STRUCTS_H
#define STRUCTS_H

#include "main.h"
#include "vector.h"

typedef struct Entity Entity;

typedef struct AnimatedSprite AnimatedSprite;

struct AnimatedSprite
{
	int currentFrame;
	int maxFrames;
	float speed;
	int sx;
	int sy;
	int width;
	int height;
	SDL_Texture* texture;
};

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
	void(*data); //For extending members
	SDL_Rect collider;
	int isHit;
	int direction;
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
	int score;
	int ammo;
	KEY keys[MAX_KEYS];
}PlayerObject;

//Enemy Sentry
typedef struct
{
	Vec2 dest; //point to move to
	int canSeePlayer;
	AnimatedSprite walkingAnimation;
}Sentry;

typedef struct
{
	int tiles[MAX_MAP_LAYERS][(MAP_WIDTH * MAP_HEIGHT)];
}Stage;

typedef struct
{
	int debug;
	int soundOn;
}Settings;

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	float deltaTime;
	int isRunning;
	void (*update)();
	void (*render)();
	void (*cleanup)();
	Vec2 camera;
	Settings settings;
}Game;

#endif 
