#include "door.h"
#include "main.h"

static void OnHit(Entity* self, Entity* other);

void CreateDoor(float x, float y)
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
	door->onHit = OnHit;
	SDL_QueryTexture(door->texture, NULL, NULL, &door->w, &door->h);
	door->collider.w = door->w;
	door->collider.h = door->h;
	door->weightless = 0;
	printf("Door initialized at %f %f\n", x, y);
	AddEntity(door);
}

static void OnHit(Entity* self, Entity* other)
{
	if (other->tag == TAG_PLAYER)
	{
		if (HasKey(GREEN_KEY))
		{
			self->isActive = 0;
		}
		
	}
}