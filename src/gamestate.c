#include "gamestate.h"
#include "main.h"
#include "map.h"

static void update(void);
static void render(void);
static void cleanup(void);
static void StartLevelOver(void);

static int isPaused;

void StartGameState(void)
{
	game.update = update;
	game.render = render;
	game.cleanup = cleanup;
	

	InitEntities();
	InitBullets(); //Creates a bullet pool

	if (user.currentLevel >= game.levelCount)
	{
		printf("Trying to load level number that exceeds max level.\n");
		game.isRunning = 0;
		return;
	}

	char* mapFilePath = game.levelQueue[user.currentLevel];
	LoadMap(mapFilePath);

	CreateHud();
	
	isPaused = 0;
	
	PlayMusic();
}

static void update(void)
{
	//if (GetButtonDown(INP_SUBMIT))
	//	isPaused = isPaused ? 0 : 1;
	//if (isPaused)return;

	if (GetPlayer()->health <= 0)
	{
		StartLevelOver();
		return;
	}
		

	UpdateEntities();
	UpdateHud();
}

static void render(void)
{
	RenderMap();
	RenderEntities();
	RenderHud();

	if (isPaused)
		Graphics_DrawText(SCREEN_WIDTH / 2-100, SCREEN_HEIGHT / 2, "PAUSED", 255, 255, 255);
}

static void cleanup(void)
{
	ClearEntities();
	UnloadMap();
}

static void StartLevelOver(void)
{
	cleanup();
	StartGameState();
}