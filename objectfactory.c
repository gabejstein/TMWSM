#include "objectfactory.h"

void CreateObject(char* name, float x, float y)
{
	if (strcmp(name, "player") == 0)
	{
		CreatePlayer(x, y);
	}
	else if (strcmp(name, "door") == 0)
	{
		//Make door
		Entity* door = (Entity*)malloc(sizeof(Entity));
		memset(door, '0', sizeof(Entity));
		door->pos.x = x;
		door->pos.y = y;
		door->isActive = 1;
		door->texture = IMG_LoadTexture(game.renderer, "assets/sprites/Door_01.png");
		door->update = NULL;
		door->render = NULL;
		door->onHit = NULL;
		SDL_QueryTexture(door->texture, NULL, NULL, &door->w, &door->h);
		door->collider.w = door->w;
		door->collider.h = door->h;
		printf("Door initialized at %f %f\n", x, y);
		AddEntity(door);
	}
	else if (strcmp(name, "key") == 0)
	{

	}
	else if (strcmp(name, "sentry") == 0)
	{
		printf("Sentry initialized at %f %f\n", x, y);
		CreateSentry(x, y);
	}
}

static void DoorOnHit(void)
{

}