#include "entity.h"
#include <stdio.h>
#include "map.h"

Entity** entities;

int currentEntity; //The top of the entity stack.

static void debugRender(Entity* e);
static int RectCollision(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh);
static void HandleTileCollisions(Entity* e);

static float gravity = 8.0;

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

			if(e->weightless==0)
				e->vel.y += gravity;

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
				e->render();
				
			}
			else
			{
				BlitTexture(e->texture, e->pos.x-game.camera.x, e->pos.y-game.camera.y);
			}

			if (game.debug)
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
		free(entities[i]);
		
	free(entities);

	printf("Entities cleared.\n");
}

static int RectCollision(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
{
	if ((ax+aw) >= bx && ax <= (bx + bw) && (ay+ah) >= by && ay <= (by + bh))
		return 1;

	return 0;
}

static void HandleTileCollisions(Entity* e)
{
	Vec2 tempPos = { e->pos.x + e->vel.x*game.deltaTime, e->pos.y };
	int mx, my;

	my = tempPos.y / TILE_SIZE;

	if (e->vel.x > 0)
	{
		mx = (tempPos.x + e->w)/TILE_SIZE;
		

		if (GetTile(mx, my, 1) == 1)
		{
			tempPos.x = (mx * TILE_SIZE) - e->w;
			e->vel.x = 0.0;
		}
	}
	else if (e->vel.x < 0)
	{
		mx = tempPos.x / TILE_SIZE;


		if (GetTile(mx, my, 1) == 1)
		{
			tempPos.x = (mx * TILE_SIZE) + TILE_SIZE;
			e->vel.x = 0.0;
		}
	}

	tempPos.y = e->pos.y + e->vel.y * game.deltaTime;
	mx = tempPos.x / TILE_SIZE;

	if (e->vel.y > 0)
	{
		my = (tempPos.y + e->h) / TILE_SIZE;

		if (GetTile(mx, my,1) == 1)
		{
			tempPos.y = my * TILE_SIZE - e->h;
			e->vel.y = 0.0;
			e->isGrounded = 1;
		}
	}
	else if (e->vel.y < 0)
	{
		my = tempPos.y / TILE_SIZE;

		if (GetTile(mx, my,1) == 1)
		{
			tempPos.y = my * TILE_SIZE + TILE_SIZE;
			e->vel.y = 0.0;
		}
	}

	e->pos.x = tempPos.x;
	e->pos.y = tempPos.y;
}