#include "player.h"
#include <stdio.h>
#include "map.h"

static Entity* player;
static float playerSpeed = 400;

static void UpdatePlayer(Entity* self);
static void RenderPlayer(void);
static void PlayerOnHit(Entity* self, Entity* other);
static void HandleCamera(void);
static void FireGun(void);
static void RenderHud(void);

static int ammo = 100;
static float fireRate = 500;
static float lastFireTime = 0.0;

static AnimatedSprite walkAnimation;

static KEY keys[MAX_KEYS];
SDL_Texture* keyTextures[MAX_KEYS];

Vec2 startPos;

static float lastDamagedTime;
static float damagePeriod = 1000;
static int isDamageBlink;

void CreatePlayer(float x, float y)
{
	player = (Entity*)malloc(sizeof(Entity));
	memset(player, '0', sizeof(Entity));
	player->pos.x = x;
	player->pos.y = y;
	startPos = player->pos;
	player->isActive = 1;
	//player->texture = IMG_LoadTexture(game.renderer, "assets/sprites/mega_boy.png");
	//SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
	player->w = 64;
	player->h = 64;
	player->collider.w = player->w;
	player->collider.h = player->h;

	player->update = UpdatePlayer;
	player->render = RenderPlayer;
	player->onHit = PlayerOnHit;
	player->direction = LEFT;

	player->health = 4;
	player->tag = TAG_PLAYER;
	player->weightless = 0;

	AddEntity(player);

	walkAnimation.texture = GetTexture("player_walk");
	walkAnimation.width = 64;
	walkAnimation.height = 64;
	walkAnimation.maxFrames = 8;
	walkAnimation.currentFrame = 0;
	walkAnimation.sx = 0;
	walkAnimation.sy = 0;
	walkAnimation.speed = 1000.0;

	keys[GREEN_KEY] = 1;
	keys[RED_KEY] = 1;
	keys[BLUE_KEY] = 1;

	keyTextures[BLUE_KEY] = GetTexture("key_blue");
	keyTextures[RED_KEY] = GetTexture("key_red");
	keyTextures[GREEN_KEY] = GetTexture("key_green");
	
}

static void UpdatePlayer(Entity* self)
{
	player->vel.x = 0.0;

	if (GetButton(INP_LEFT))
	{
		player->vel.x -= playerSpeed;
		walkAnimation.speed = 1;
	}
	else if (GetButton(INP_RIGHT))
	{
		player->vel.x += playerSpeed;
		walkAnimation.speed = 1;
	}


	if (GetButton(INP_JUMP) && player->isGrounded)
	{
		player->vel.y -= 450.0;
		player->isGrounded = 0;
	}
	/*
	if (game.input.keys[SDL_SCANCODE_R])
		player->pos = startPos;
		*/
	if (player->vel.x < 0)
		player->direction = LEFT;
	else if (player->vel.x > 0)
		player->direction = RIGHT;

	if (GetButton(INP_FIRE1) && SDL_GetTicks() - lastFireTime > fireRate)
	{
		if(ammo>0 )
			FireGun();
		lastFireTime = SDL_GetTicks();
	}
		

	HandleCamera();

	if (isDamageBlink)
	{
		if (SDL_GetTicks() - lastDamagedTime > damagePeriod)
		{
			isDamageBlink = 0;
		}
	}

}

static void RenderPlayer(void)
{
	//TODO: Replace this with a proper blink
	if (isDamageBlink && ((SDL_GetTicks()/100) % 2) == 0)return;

	Vec2 screenPos = { player->pos.x - game.camera.x,player->pos.y - game.camera.y };
	//BlitTexture(player->texture, screenPos.x, screenPos.y);
	SDL_RendererFlip flip = player->direction == RIGHT ? 0 : 1;
	if (!player->isGrounded)
		walkAnimation.speed = 0;
	
	
	PlayAnimatedSprite(&walkAnimation, screenPos.x, screenPos.y, flip);
	

	RenderHud();
}

static void RenderHud(void)
{
	Vec2 keyPos = { 900,5 };
	int i;
	for (i = 0; i < MAX_KEYS; i++)
	{
		if(keys[i])
			BlitTexture(keyTextures[i], keyPos.x + (i* 64), keyPos.y);
	}
	
	char ammoDisplay[50];
	sprintf(ammoDisplay, "Ammo: %d", ammo);
	DrawText(50, 5, ammoDisplay,255,0,0);
	char healthDisplay[50];
	sprintf(healthDisplay, "Health: %d", player->health);
	DrawText(480, 5, healthDisplay,0,0,255);
	
}

static void PlayerOnHit(Entity* self, Entity* other)
{
	//TODO: separate into its own function and create a damage delay.

	if ((other->tag == TAG_ENEMY || other->tag==TAG_ENEMY_BULLET) && !isDamageBlink)
	{
		self->health--;
		lastDamagedTime = SDL_GetTicks();
		isDamageBlink = 1;
	}
		

	
	if (self->health <= 0)
	{
		//self->isActive = 0;
		self->pos = startPos;
		self->health = 4;
	}
		
}

static void FireGun(void)
{
	float bulletSpeed = 500.0;

	Vec2 bulletPos;
	Vec2 bulletVel;

	if (player->direction == LEFT)
	{
		bulletPos.x =player->pos.x - 50;
		bulletVel.x = -bulletSpeed;
	}
	else
	{
		bulletPos.x = player->pos.x + 100;
		bulletVel.x = bulletSpeed;
	}

		bulletPos.y = player->pos.y;
		bulletVel.y = 0;

	SpawnBullet(bulletPos.x,bulletPos.y, bulletVel.x,bulletVel.y,TAG_PLAYER_BULLET);

	ammo--;
}

static void HandleCamera(void)
{
	game.camera = player->pos;

	game.camera.x -= SCREEN_WIDTH / 2;
	game.camera.y -= SCREEN_HEIGHT / 2;

	if (game.camera.x < 0)
	{
		game.camera.x = 0;
	}
	else if (game.camera.x > MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH)
	{
		game.camera.x = MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH;
	}

	if (game.camera.y < 0)
	{
		game.camera.y = 0;
	}
	else if (game.camera.y > MAP_HEIGHT * TILE_SIZE - SCREEN_HEIGHT)
	{
		game.camera.y = MAP_HEIGHT * TILE_SIZE - SCREEN_HEIGHT;
	}
}

Entity* GetPlayer(void) { return player; }

//TODO: Separate removing the key into its own function
int HasKey(int keyColor)
{
	if (keys[keyColor])
	{
		keys[keyColor] = 0;
		return 1;
	}

	return 0;
}