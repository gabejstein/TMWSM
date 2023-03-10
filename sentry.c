#include "sentry.h"

#include "main.h"
#include "map.h"

static void UpdateEnemy(Entity* self);
static void UpdateShooter(Entity* self);
static void RenderEnemy(Entity* self);
static void EnemyHit(Entity* self, Entity* other);
static void CleanupEnemy(Entity* e);
static Entity* NewEnemy(void);

static float sentrySpeed = 300.0;

static int enemySize = 64;

//Base enemy constructor
static Entity* NewEnemy(void)
{
	Entity* e = NewEntity();

	e->onHit = EnemyHit;
	e->cleanup = CleanupEnemy;
	e->render = NULL;
	e->isActive = 1;
	//SDL_QueryTexture(sentryTexture, NULL, NULL, &e->w, &e->h);
	e->w = enemySize;
	e->h = enemySize;
	e->collider.x = e->pos.x;
	e->collider.y = e->pos.y;
	e->collider.w = e->w;
	e->collider.h = e->h;
	e->health = 3;
	e->tag = TAG_ENEMY;
	e->direction = LEFT;
	e->lastTime = 0;
	
	return e;
}

void CreateSentry(float x, float y)
{

	Entity* e = NewEnemy();

	e->pos.x = x;
	e->pos.y = y;
	e->update = UpdateEnemy;
	e->render = RenderEnemy;

	e->texture = GetTexture("sentry");
	e->health = 2;

	Sentry* sentryData = (Sentry*)malloc(sizeof(Sentry));
	memset(sentryData, 0, sizeof(Sentry));
	sentryData->walkingAnimation.currentFrame = 0;
	sentryData->walkingAnimation.height= 64;
	sentryData->walkingAnimation.width = 64;
	sentryData->walkingAnimation.texture = e->texture;
	sentryData->walkingAnimation.maxFrames = 3;
	sentryData->walkingAnimation.sx = 0;
	sentryData->walkingAnimation.sy = 0;
	sentryData->walkingAnimation.speed = 1;

	e->data = sentryData;

	AddEntity(e);

}

void CreateShooter(float x, float y)
{
	Entity* e = NewEnemy();

	e->pos.x = x;
	e->pos.y = y;
	e->update = UpdateShooter;
	e->render = RenderEnemy;

	e->texture = GetTexture("shooter");
	e->health = 3;

	Sentry* sentryData = (Sentry*)malloc(sizeof(Sentry));
	memset(sentryData, 0, sizeof(Sentry));
	sentryData->walkingAnimation.currentFrame = 0;
	sentryData->walkingAnimation.height = 64;
	sentryData->walkingAnimation.width = 64;
	sentryData->walkingAnimation.texture = e->texture;
	sentryData->walkingAnimation.maxFrames = 3;
	sentryData->walkingAnimation.sx = 0;
	sentryData->walkingAnimation.sy = 0;
	sentryData->walkingAnimation.speed = 1;

	e->data = sentryData;

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
	if (IsCollisionTile(mx,my,1))
	{
		self->direction = self->direction == LEFT ? RIGHT : LEFT;
	}

	my = self->pos.y + self->h;
	my /= TILE_SIZE;
	if (GetTile(mx, my, 1) == 0)
	{
		self->direction = self->direction == LEFT ? RIGHT : LEFT;
	}

	Sentry* sentry = (Sentry*) self->data;
	sentry->walkingAnimation.speed = 1;

}

//update function for specialized shooter enemies
static void UpdateShooter(Entity* self)
{
	//I guess this is how inheritance is done in C
	UpdateEnemy(self);

	Vec2 bulVel = { 0,0 };
	Vec2 bulPos = { 0,0 };

	Entity* player = GetPlayer();
	
	if (self->direction == LEFT)
	{
		//If player is to the left of shooter and is within bounds and within range then fire projectile
		if (player->pos.x < self->pos.x)
		{
			if (player->pos.y + player->h > self->pos.y && player->pos.y < self->pos.y + self->h)
			{
				bulVel.x = -500.0;
				bulPos.x = self->pos.x - 100;
			}
		}
		
	}
	else if (self->direction == RIGHT)
	{
		//If player is to the left of shooter and is within bounds and within range then fire projectile
		if (player->pos.x > self->pos.x)
		{
			if (player->pos.y + player->h > self->pos.y && player->pos.y < self->pos.y + self->h)
			{
				bulVel.x = 500.0;
				bulPos.x = self->pos.x + 100;
			}
		}
	}

	if (bulVel.x != 0 && SDL_GetTicks() - self->lastTime > 1000)
	{
		PlaySound(SFX_SHOOT2,-1);
		SpawnBullet(bulPos.x, self->pos.y, bulVel.x, bulVel.y, TAG_ENEMY_BULLET);
		self->lastTime = SDL_GetTicks();
	}
}

static void RenderEnemy(Entity* self)
{
	Sentry* sentry = (Sentry*)self->data;
	SDL_RendererFlip flip = self->direction == RIGHT ? 0 : 1;
	if (!self->isGrounded)
		sentry->walkingAnimation.speed = 0;

	Vec2 screenPos = { self->pos.x - game.camera.x,self->pos.y - game.camera.y };

	PlayAnimatedSprite(&sentry->walkingAnimation, screenPos.x, screenPos.y, flip);
	
}

static void EnemyHit(Entity* self, Entity* other)
{
	if(other->tag==TAG_PLAYER_BULLET)
		if (--self->health <= 0)
		{
			self->isActive = 0;
			PlaySound(SFX_ENEMY_DEATH, -1);
		}
			

	if(other->isSolid)
		self->direction = self->direction == LEFT ? RIGHT : LEFT;
}

static void CleanupEnemy(Entity* e)
{
	free(e->data);
}