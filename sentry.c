#include "sentry.h"

#include "main.h"
#include "map.h"

static void UpdateEnemy(Entity* self);
static void RenderEnemy(Entity* self);
static void EnemyHit(Entity* self, Entity* other);

static SDL_Texture* sentryTexture;

static float sentrySpeed = 300.0;

void InitSentry(void)
{
	sentryTexture = IMG_LoadTexture(game.renderer, "assets/sprites/ninja.png");
}

void CreateSentry(float x, float y)
{

	Entity* e = (Entity*)malloc(sizeof(Entity));
	memset(e, '0', sizeof(Entity));

	e->pos.x = x;
	e->pos.y = y;
	e->update = UpdateEnemy;
	e->render = NULL;
	e->onHit = EnemyHit;
	e->isActive = 1;
	e->texture = sentryTexture;
	SDL_QueryTexture(sentryTexture, NULL, NULL, &e->w, &e->h);
	e->collider.x = e->pos.x;
	e->collider.y = e->pos.y;
	e->collider.w = e->w;
	e->collider.h = e->h;
	e->health = 3;
	e->tag = TAG_ENEMY;
	e->weightless = 0;
	e->direction = LEFT;

	AddEntity(e);

}

static void UpdateEnemy(Entity* self)
{
	if (self->direction == LEFT)
		self->vel.x = -sentrySpeed;
	else
		self->vel.x = sentrySpeed;

	//Turn enemies around if they hit a wall or ledge.
	int mx, my;
	mx = self->direction == LEFT ? self->pos.x-1 : self->pos.x + self->w;
	mx /= TILE_SIZE;
	my = self->pos.y;
	my /= TILE_SIZE;
	if (GetTile(mx, my, 1) == 1)
	{
		self->direction = self->direction == LEFT ? RIGHT : LEFT;
	}

	my = self->pos.y + self->h;
	my /= TILE_SIZE;
	if (GetTile(mx, my, 1) == -1)
	{
		self->direction = self->direction == LEFT ? RIGHT : LEFT;
	}

}

static void RenderEnemy(Entity* self)
{

}

static void EnemyHit(Entity* self, Entity* other)
{
	if(other->tag==TAG_PLAYER_BULLET)
		if(--self->health<=0)
			self->isActive = 0;
}