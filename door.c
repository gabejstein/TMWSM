#include "door.h"
#include "main.h"

static void OnHit(Entity* self, Entity* other);

static Entity* CreateDoor(float x, float y)
{
	//Make door
	Entity* door = NewEntity();
	door->pos.x = x;
	door->pos.y = y;
	door->isActive = 1;
	door->isSolid = 1;
	door->onHit = OnHit;
	door->collider.w = door->w;
	door->collider.h = door->h;
	door->weightless = 1;
	
	printf("Door initialized at %f %f\n", x, y);
	
	return door;
}

void CreateGreenDoor(float x, float y)
{
	Entity* door = CreateDoor(x, y);
	door->texture = GetTexture("green_door");
	SDL_QueryTexture(door->texture, NULL, NULL, &door->w, &door->h);
	door->scoreValue = GREEN_KEY;
	AddEntity(door);
}

void CreateRedDoor(float x, float y)
{
	Entity* door = CreateDoor(x, y);
	door->texture = GetTexture("red_door");
	SDL_QueryTexture(door->texture, NULL, NULL, &door->w, &door->h);
	door->scoreValue = RED_KEY;
	AddEntity(door);
}

void CreateBlueDoor(float x, float y)
{
	Entity* door = CreateDoor(x, y);
	door->texture = GetTexture("blue_door");
	SDL_QueryTexture(door->texture, NULL, NULL, &door->w, &door->h);
	door->scoreValue = BLUE_KEY;
	AddEntity(door);
}

static void OnHit(Entity* self, Entity* other)
{
	if (other->tag == TAG_PLAYER)
	{
		if (HasKey(self->scoreValue))
		{
			PlaySound(SFX_DOOR_OPEN, -1);
			self->isActive = 0;
		}
		
	}
}