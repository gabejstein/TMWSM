#include "gamestate.h"
#include "main.h"
#include "map.h"

static void update(void);
static void render(void);
static void cleanup(void);

static int isPaused;

void StartGameState(void)
{
	game.update = update;
	game.render = render;
	game.cleanup = cleanup;
	

	InitEntities();
	InitBullets(); //Creates a bullet pool

	LoadMap("assets/levels/mapOutput.txt");

	CreateHud();
	
	isPaused = 0;
	
	PlayMusic();
}

static void update(void)
{
	if (GetButtonDown(INP_SUBMIT))
		isPaused = isPaused ? 0 : 1;
	if (isPaused)return;

	UpdateEntities();
	UpdateHud();
}

static void render(void)
{
	RenderMap();
	RenderEntities();
	RenderHud();

	if (isPaused)
		DrawText(SCREEN_WIDTH / 2-100, SCREEN_HEIGHT / 2, "PAUSED", 255, 255, 255);
}

static void cleanup(void)
{
	ClearEntities();
	UnloadMap();
}