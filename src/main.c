#include <stdio.h>
#include <string.h>
#include "main.h"

Game game;
UserData user;

static float lastFrame = 0.0;

static void LoadSettings(char* path);
static void InitLevelQueue(char* path);
static void FreeLevelQueue(void);

static void Init_SDL(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Could not start SDL.\n");
		return;
	}

	game.window = SDL_CreateWindow("The Man Who Spied Too Much", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (game.window == 0)
	{
		printf("Could not create window.\n");
		return;
	}

	game.renderer = SDL_CreateRenderer(game.window, -1, 0);

	if (game.renderer == 0)
	{
		printf("Could not create renderer.\n");
		return;
	}
	Graphics_SetRenderer(game.renderer);

	SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);

	IMG_Init(IMG_INIT_PNG);

	if (TTF_Init() < 0)
	{
		printf("Could not initialize TTF.\n");
		exit(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("Could not initialize SDL_Mixer.\n");
		exit(1);
	}

	Mix_AllocateChannels(MAX_SFX_CHANNELS);

	InitGamepad();

	SDL_ShowCursor(0);

}

static void LoadSettings(char* path)
{
	char buffer[256];
	int arg;
	FILE* f = fopen(path, "r");
	if (f)
	{
		do
		{
			fscanf(f, "%s %d", buffer, &arg);
			if (strcmp(buffer, "debug") == 0)
			{
				game.settings.debug = arg;
			}
			else if (strcmp(buffer, "soundOn") == 0)
			{
				game.settings.soundOn = arg;
				SetSoundMute(!game.settings.soundOn);
			}
			else if (strcmp(buffer, "musicOn") == 0)
			{
				game.settings.musicOn = arg;
				SetMusicMute(!game.settings.musicOn);
			}
		} while (!feof(f));
		fclose(f);
	}
	else
	{
		printf("Could not read settings file.\n");
	}
}

static void Start(void)
{
	memset(&game, 0, sizeof(game));

	Init_SDL();
	Graphics_InitFont();
	InitLevelQueue("assets/level_resource.txt");
	Graphics_LoadTextures("assets/sprite_resource.txt");
	InitSounds();
	InitDialogueBox();

	game.isRunning = 1;
	LoadSettings("settings.ini");

	//StartGameState();
	LoadCutscene("assets/cutscenes/opening.txt");
}

static void ProcessInput(void)
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			game.isRunning = 0;
			break;
		case SDL_KEYDOWN:
			ProcessKeyboardDown(&event);
			break;
		case SDL_KEYUP:
			ProcessKeyboardUp(&event);
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			ProcessJoyButtonDown(&event);
			break;
		case SDL_CONTROLLERBUTTONUP:
			ProcessJoyButtonUp(&event);
			break;
		}
	}

	UpdateInputs();
}

static void Update(void)
{
	int waitTime = MILLISECS_PER_FRAME - (SDL_GetTicks() - lastFrame);
	if (waitTime > 0)
		SDL_Delay(waitTime);

	game.deltaTime = (SDL_GetTicks() - lastFrame) / 1000.0f;
	lastFrame = SDL_GetTicks();

	if(game.update)
		game.update();

}

static void Render(void)
{
	SDL_RenderClear(game.renderer);
	
	if (game.render)
		game.render();

	SDL_RenderPresent(game.renderer);
}

static void Cleanup(void)
{
	printf("Cleaning up game.\n");
	if (game.cleanup)
		game.cleanup();

	FreeLevelQueue();
	Graphics_FreeAllTextures();
	CleanupSound();

	//SDL-related
	CloseGamepad();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	printf("Program ended.\n");
}

int main(int argc, char* argv[])
{
	Start();

	while (game.isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}

	Cleanup();
	return 0;
}

static void InitLevelQueue(char* path)
{
	FILE* f = fopen(path,"rb");
	if (!f) { printf("Could not load level queue.\n"); exit(1); }

	game.levelCount = 0;
	//TODO: have some kind of validation to see if the line is valid first.
	while (!feof(f))
		if (fgetc(f) == '\n')
			game.levelCount++;
	game.levelCount++;

	fclose(f);
	f = fopen(path, "r");
	if (!f) { printf("Could not load level queue.\n"); exit(1); }

	game.levelQueue = (char**)malloc(sizeof(char*) * game.levelCount);

	for (unsigned int i = 0; i < game.levelCount; i++)
	{
		game.levelQueue[i] = (char*)malloc(MAX_FILENAME);
		fscanf(f, "%s\n", game.levelQueue[i]);
	}
		

	fclose(f);

	printf("Num levels in queue: %d\n", game.levelCount);
	user.currentLevel = 0;
}

static void FreeLevelQueue(void)
{
	for (unsigned int i = 0; i < game.levelCount; i++)
		free(game.levelQueue[i]);
	free(game.levelQueue);
}