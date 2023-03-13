#include "pickup.h"
#include "main.h"

static void onPickupHit(Entity* self, Entity* other);
static void onAmmoPickupHit(Entity* self, Entity* other);
static void onKeyHit(Entity* self, Entity* other);

void CreateMoney(int x, int y)
{
	Entity* money = NewEntity();
	AddEntity(money);
	money->texture = GetTexture("money");
	money->weightless = 1;
	money->scoreValue = 100;
	money->onHit = onPickupHit;
	money->tag = TAG_PICKUP;
	money->pos.x = x;
	money->pos.y = y;
	SDL_QueryTexture(money->texture, NULL, NULL, &money->w, &money->h);
}

void CreateAmmoPickup(int x, int y)
{
	Entity* ammoE = NewEntity();
	AddEntity(ammoE);
	ammoE->texture = GetTexture("ammoPickup");
	ammoE->weightless = 1;
	ammoE->scoreValue = 10;
	ammoE->tag = TAG_PICKUP;
	ammoE->pos.x = x;
	ammoE->pos.y = y;
	ammoE->onHit = onAmmoPickupHit;
	SDL_QueryTexture(ammoE->texture, NULL, NULL, &ammoE->w, &ammoE->h);
}

void CreateRedKey(int x, int y)
{
	Entity* key = NewEntity();
	AddEntity(key);
	key->texture = GetTexture("key_red");;
	key->weightless = 1;
	key->scoreValue = RED_KEY;
	key->tag = TAG_PICKUP;
	key->pos.x = x;
	key->pos.y = y;
	key->onHit = onKeyHit;
	SDL_QueryTexture(key->texture, NULL, NULL, &key->w, &key->h);
}

void CreateBlueKey(int x, int y)
{
	Entity* key = NewEntity();
	AddEntity(key);
	key->texture = GetTexture("key_blue");
	key->weightless = 1;
	key->scoreValue = BLUE_KEY;
	key->tag = TAG_PICKUP;
	key->pos.x = x;
	key->pos.y = y;
	key->onHit = onKeyHit;
	SDL_QueryTexture(key->texture, NULL, NULL, &key->w, &key->h);
}

void CreateGreenKey(int x, int y)
{
	Entity* key = NewEntity();
	AddEntity(key);
	key->texture = GetTexture("key_green");
	key->weightless = 1;
	key->scoreValue = GREEN_KEY;
	key->tag = TAG_PICKUP;
	key->pos.x = x;
	key->pos.y = y;
	key->onHit = onKeyHit;
	SDL_QueryTexture(key->texture, NULL, NULL, &key->w, &key->h);
}

static void onPickupHit(Entity* self, Entity* other)
{
	if (other->tag == TAG_PLAYER)
	{
		PlayerObject* po = (PlayerObject*)other->data;
		po->score += self->scoreValue;
		self->isActive = 0;
		PlaySound(SFX_PICKUP, -1);
	}
}

static void onKeyHit(Entity* self, Entity* other)
{
	if (other->tag == TAG_PLAYER)
	{
		GetKey(self->scoreValue);
		self->isActive = 0;
		PlaySound(SFX_PICKUP, -1);
	}
}

static void onAmmoPickupHit(Entity* self, Entity* other)
{
	if (other->tag == TAG_PLAYER)
	{
		PlayerObject* po = (PlayerObject*)other->data;
		po->ammo += self->scoreValue;
		self->isActive = 0;
		PlaySound(SFX_PICKUP, -1);
	}
}

