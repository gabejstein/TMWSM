#include "gamestate.h"
#include "main.h"
#include "map.h"

static void update(void);
static void render(void);
static void cleanup(void);


void StartGameState(void)
{
	game.update = update;
	game.render = render;
	game.cleanup = cleanup;
	LoadMap("assets/levels/level02.txt");

	InitEntities();
	InitBullets(); //Creates a bullet pool
	InitSentry();

	LoadEntities("assets/levels/level02objects.txt");
	
	
	game.debug = 1;
}

static void update(void)
{
	UpdateEntities();
	
}

static void render(void)
{
	RenderMap();
	RenderEntities();
}

static void cleanup(void)
{
	ClearEntities();
	UnloadMap();
}