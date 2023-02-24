#include <stdio.h>
#include "main.h"

Game game;

float lastFrame = 0.0;

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

	IMG_Init(IMG_INIT_PNG);

	if (TTF_Init() < 0)
	{
		printf("Could not initialize TTF.\n");
		exit(1);
	}

	game.isRunning = 1;
	
}

static void Start(void)
{
	Init_SDL();
	InitFont();
	LoadTextures("assets/sprite_resource.txt");

	StartGameState();
	//LoadCutscene("assets/cutscenes/cutscene01.txt");
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
			UpdateInputs();
			break;
		case SDL_KEYUP:
			ProcessKeyboardUp(&event);
			UpdateInputs();
			break;
		case SDL_JOYBUTTONDOWN:
			ProcessJoyButtonDown(&event);
			break;
		case SDL_JOYBUTTONUP:
			ProcessJoyButtonUp(&event);
			break;
		}
	}

	
}

static void Update(void)
{
	int waitTime = MILLISECS_PER_FRAME - (SDL_GetTicks() - lastFrame);
	if (waitTime > 0)
		SDL_Delay(waitTime);

	game.deltaTime = (SDL_GetTicks() - lastFrame) / 1000;
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

	FreeAllTextures();

	//SDL-related
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