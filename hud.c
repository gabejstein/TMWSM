#include "hud.h"
#include "main.h"

#include <stdio.h>

static Entity* player;

static int ammo;
static int health;
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
}

void RenderHud(void)
{
	Vec2 keyPos = { 900,5 };
	int i;
	for (i = 0; i < MAX_KEYS; i++)
	{
		if (keys[i])
			BlitTexture(keyTextures[i], keyPos.x + (i * 64), keyPos.y);
	}

	char ammoDisplay[50];
	sprintf(ammoDisplay, "Ammo: %d", ammo);
	DrawText(50, 5, ammoDisplay, 255, 0, 0);
	char healthDisplay[50];
	sprintf(healthDisplay, "Health: %d", health);
	DrawText(480, 5, healthDisplay, 0, 0, 255);

}

void CleanupHud(void)
{

}