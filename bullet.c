#include "bullet.h"
#include "main.h"

#define MAX_BULLETS 20

SDL_Texture* bulletTexture;

Entity* bulletPool[MAX_BULLETS];

int current;

static void BulletHit(Entity* self, Entity* other);

void InitBullets(void)
{
	bulletTexture = GetTexture("bullet");

	current = 0;

	int i;
	for (i = 0; i < MAX_BULLETS; i++)
	{
		Entity* e = NewEntity();

		e->texture = bulletTexture;
		SDL_QueryTexture(bulletTexture, NULL, NULL, &e->w, &e->h);
		e->collider.w = e->w;
		e->collider.h = e->h;

		e->onHit = BulletHit;
		e->isActive = 0;
		e->weightless = 1;
		bulletPool[i] = e;

		AddEntity(e);
	}
	
}

void SpawnBullet(float x, float y, float vx, float vy, TAG tag)
{
	Entity* e = bulletPool[current++];
	if (current >= MAX_BULLETS)current = 0;

	e->pos.x = x;
	e->pos.y = y;
	e->vel.x = vx;
	e->vel.y = vy;

	e->isActive = 1;
	e->tag = tag;
}

static void BulletHit(Entity* self, Entity* other)
{
	if(other->tag != TAG_PLAYER_BULLET && other->tag!=TAG_ENEMY_BULLET)
		self->isActive = 0;
	//TODO: create some impact effects
}