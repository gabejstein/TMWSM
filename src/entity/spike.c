#include "spike.h"
#include "../main.h"

static void UpdateSpike(Entity* self);
static void RenderSpike(Entity* self);
static void SpikeHit(Entity* self, Entity* other);

void CreateSpike(float x, float y)
{
	Entity* e = NewEntity();

	e->pos.x = x;
	e->pos.y = y;
	e->update = UpdateSpike;
	e->render = RenderSpike;
	e->onHit = SpikeHit;
	e->texture = GetTexture("door_red");
	SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);

	AddEntity(e);
}

static void UpdateSpike(Entity* self)
{

}

static void RenderSpike(Entity* self)
{

}

static void SpikeHit(Entity* self, Entity* other)
{
	if (other->tag == TAG_PLAYER)
	{

	}
}