#include "hud.h"
#include "main.h"

#include <stdio.h>

static Entity* player;

static int ammo;
static int health;
static int score;
static KEY keys[MAX_KEYS];

static SDL_Texture* keyTextures[MAX_KEYS];

void CreateHud(void)
{
	player = GetPlayer();

	keyTextures[BLUE_KEY] = GetTexture("key_blue");
	keyTextures[RED_KEY] = GetTexture("key_red");
	keyTextures[GREEN_KEY] = GetTexture("key_green");
}

void UpdateHud(void)
{
	health = player->health;

	PlayerObject* po = (PlayerObject*) player->data;
	ammo = po->ammo;
	keys[0] = po->keys[0];
	keys[1] = po->keys[1];
	keys[2] = po->keys[2];
	score = po->score;
}

void RenderHud(void)
{
	SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 50);
	SDL_Rect blackScreen = { 0,0,SCREEN_WIDTH,120 };
	SDL_RenderFillRect(game.renderer, &blackScreen);
	SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 0);

	Vec2 keyPos = { 900,5 };
	int i;
	for (i = 0; i < MAX_KEYS; i++)
	{
		if (keys[i])
			BlitTexture(keyTextures[i], keyPos.x + (i * 64), keyPos.y);
	}

	char buffer[50];
	sprintf(buffer, "Ammo: %d", ammo);
	DrawText(50, 5, buffer, 255, 0, 0);

	sprintf(buffer, "Health: %d", health);
	DrawText(480, 5, buffer, 0, 0, 255);

	sprintf(buffer, "Score: %d", score);
	DrawText(480, 69, buffer, 0, 0, 255);

}

void CleanupHud(void)
{

}