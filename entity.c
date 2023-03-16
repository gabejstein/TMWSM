#include "entity.h"
#include <stdio.h>
#include "map.h"

Entity** entities;

int currentEntity; //The top of the entity stack.

static void debugRender(Entity* e);
static int RectCollision(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh);
static void ResolveSolidCollisions(Entity* self, Entity* other);
static void HandleTileCollisions(Entity* e);

static float gravity = 15.0;

void InitEntities(void)
{
	entities = (Entity**)malloc(sizeof(Entity*) * MAX_ENTITIES);
	memset(entities, '0', sizeof(entities));

	currentEntity = 0;
}

void UpdateEntities(void)
{
	
	int i;
	for (i = 0; i < currentEntity; i++)
	{
		Entity* e = entities[i];
		if (!e)continue;
		if (e->isActive)
		{
			if (e->update)
			{
				e->update(e);
			}

			if (e->weightless == 0)
			{
				e->vel.y += gravity;
				
			}

			HandleTileCollisions(e);

			//update collider
			e->collider.x = e->pos.x;
			e->collider.y = e->pos.y;
			e->isHit = 0;
		}
	}

	HandleCollisions();
}

void RenderEntities(void)
{
	int i;
	for (i = 0; i < currentEntity; i++)
	{
		Entity* e = entities[i];
		if (!e)continue;
		if (e->isActive)
		{
			if (e->render)
			{
				e->render(e);
				
			}
			else
			{
				BlitTexture(e->texture, e->pos.x-game.camera.x, e->pos.y-game.camera.y);
			}

			if (game.settings.debug)
				debugRender(e);
		}
	}
}

static void debugRender(Entity* e)
{
	SDL_Rect screenRect = e->collider;
	screenRect.x -= game.camera.x;
	screenRect.y -= game.camera.y;
	if(e->isHit)
		SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
	else
		SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(game.renderer, &screenRect);
	SDL_SetRenderDrawColor(game.renderer, 0, 0, 255, 255);
	SDL_RenderDrawPoint(game.renderer, screenRect.x, screenRect.y);
	SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 0);
}

//A constructor for creating a base entity
Entity* NewEntity(void)
{
	Entity* e = (Entity*)malloc(sizeof(Entity));
	memset(e, 0, sizeof(Entity));
	e->isActive = 1;
	e->weightless = 0;
	e->isSolid = 0;
	e->update = NULL;
	e->render = NULL;
	e->cleanup = NULL;
	e->onHit = NULL;
	e->data = NULL;
	e->onTileHit = NULL;

	return e;
}

//Adds to the entity list
void AddEntity(Entity* e)
{
	if (currentEntity >= MAX_ENTITIES)
		return;
	entities[currentEntity++] = e;
	
}

void HandleCollisions(void)
{
	int i, j;
	for (i = 0; i < currentEntity-1; i++)
	{
		Entity* e = entities[i];
		if (!e->isActive)continue;

		for (j = i + 1; j < currentEntity; j++)
		{
			Entity* e2 = entities[j];
			if (!e2->isActive)continue;

			if (RectCollision(e->pos.x, e->pos.y, e->w, e->h, e2->pos.x, e2->pos.y, e2->w, e2->h))
			{
				

				if (e->onHit)
					e->onHit(e, e2);

				if (e2->onHit)
					e2->onHit(e2, e);

				e->isHit = e2->isHit = 1;

				ResolveSolidCollisions(e, e2);
			}
		}
	}
}

void ClearEntities(void)
{
	printf("Clearing Entities.\n");
	printf("Entities: %d\n", currentEntity);

	int i;
	for (i = 0; i < currentEntity; i++)
	{
		if (entities[i]->cleanup)
			entities[i]->cleanup(entities[i]);
		free(entities[i]);
	}
		
		
	free(entities);

	printf("Entities cleared.\n");
}

static int RectCollision(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
{
	if ((ax+aw) >= bx && ax <= (bx + bw) && (ay+ah) >= by && ay <= (by + bh))
		return 1;

	return 0;
}

static void ResolveSolidCollisions(Entity* self, Entity* other)
{
	float adjustmentx = 0, adjustmenty = 0;
	float margin = 2;

	if (other->isSolid)
	{
	
		adjustmentx = self->vel.x > 0 ? -((self->pos.x + self->w + margin) - (other->pos.x)) : (other->pos.x + other->w+margin) - (self->pos.x);
		adjustmenty = self->vel.y > 0 ? -((self->pos.y + self->h + margin) - (other->pos.y)) : (other->pos.y + other->h+margin) - (self->pos.y);
		//It seems like a bad-aid solution, but I found that pushing the object slightly away from the other object by about 0.1 prevents
		//unwanted collisions from happening. Things like the character being pushed through the floor or floating up to the ceiling.
		//It's not perfect and has the caveat of the character not being able to squeeze through a 1-tile-sized space, but this can be fixed by making the player smaller
		//and it's not a big enough problem to bother me.

		float abAdjustmentX = fabs(adjustmentx);
		float abAdjustmentY = fabs(adjustmenty);
		if (abAdjustmentX < abAdjustmentY)
		{
			if (self->vel.x != 0)
			{
				self->pos.x += adjustmentx;
				self->vel.x = 0;
			}
			
		}
		else
		{
		
			if (self->vel.y != 0)
			{
				self->pos.y += adjustmenty;

				if (self->vel.y > 0)
					self->isGrounded = 1;
				self->vel.y = 0;
			}
			
		}
	
		
	}
}

static void HandleTileCollisions(Entity* e)
{
	Vec2 tempPos = { e->pos.x + e->vel.x*game.deltaTime, e->pos.y };
	int mx, my, hit;
	e->isGrounded = 0;

	hit = 0;

	if (e->vel.x > 0)
	{
		mx = (tempPos.x + e->w) * INV_TILE_SIZE;
		my = tempPos.y * INV_TILE_SIZE;

		if (IsCollisionTile(mx,my,1))
		{
			hit = 1;
		}

		//need to minus the height by 1 because of how int truncation works.
		my = (tempPos.y + e->h-1) * INV_TILE_SIZE;

		if (IsCollisionTile(mx, my, 1))
		{
			hit = 1;
		}

		if (hit)
		{
			tempPos.x = (mx * TILE_SIZE) - e->w;
			e->vel.x = 0.0;

			if (e->onTileHit)
				e->onTileHit(e);
		}
	}
	else if (e->vel.x < 0)
	{
		mx = tempPos.x * INV_TILE_SIZE;
		my = tempPos.y * INV_TILE_SIZE;

		if (IsCollisionTile(mx, my, 1))
		{
			hit = 1;
		}

		my = (tempPos.y + e->h-1) * INV_TILE_SIZE;

		if (IsCollisionTile(mx, my, 1))
		{
			hit = 1;
		}

		if (hit)
		{
			tempPos.x = (mx * TILE_SIZE) + TILE_SIZE;
			e->vel.x = 0.0;

			if (e->onTileHit)
				e->onTileHit(e);
		}
	}

	hit = 0;
	tempPos.y = e->pos.y + e->vel.y * game.deltaTime;
	

	if (e->vel.y > 0)
	{
		mx = tempPos.x * INV_TILE_SIZE;
		my = (tempPos.y + e->h) * INV_TILE_SIZE;

		if (IsCollisionTile(mx, my, 1) || IsOneWayCollider(mx,my,1))
		{
			hit = 1;

		}

		mx = (tempPos.x+e->w-1) * INV_TILE_SIZE;

		if (IsCollisionTile(mx, my, 1) || IsOneWayCollider(mx, my, 1))
		{
			hit = 1;
		}

		if (hit)
		{
			tempPos.y = my * TILE_SIZE - e->h;
			e->vel.y = 0.0;
			e->isGrounded = 1;

			if (e->onTileHit)
				e->onTileHit(e);
		}
	}
	else if (e->vel.y < 0)
	{
		mx = tempPos.x * INV_TILE_SIZE;
		my = tempPos.y * INV_TILE_SIZE;

		if (IsCollisionTile(mx, my, 1))
		{
			hit = 1;
		}

		mx = (tempPos.x + e->w-1) * INV_TILE_SIZE;

		if (IsCollisionTile(mx, my, 1))
		{
			hit = 1;
		}

		if (hit)
		{
			tempPos.y = my * TILE_SIZE + TILE_SIZE;
			e->vel.y = 0.0;

			if (e->onTileHit)
				e->onTileHit(e);
		}
	}

	e->pos.x = tempPos.x;
	e->pos.y = tempPos.y;
}