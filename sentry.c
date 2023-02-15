#include "sentry.h"

#include "main.h"

static void UpdateEnemy(Entity* self);
static void RenderEnemy(Entity* self);
static void EnemyHit(Entity* self, Entity* other);

SDL_Texture* sentryTexture;

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

	AddEntity(e);
	
}

static void UpdateEnemy(Entity* self)
{
	self->vel.x = -50.0;
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