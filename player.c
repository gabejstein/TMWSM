#include "player.h"
#include "main.h"
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

	walkAnimation.texture = IMG_LoadTexture(game.renderer, "assets/sprites/girl_walk.png");
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

	keyTextures[BLUE_KEY] = IMG_LoadTexture(game.renderer, "assets/sprites/key_blue.png");
	keyTextures[RED_KEY] = IMG_LoadTexture(game.renderer, "assets/sprites/key_red.png");
	keyTextures[GREEN_KEY] = IMG_LoadTexture(game.renderer, "assets/sprites/key_green.png");
	
}

static void UpdatePlayer(Entity* self)
{
	player->vel.x = 0.0;

	if (game.input.keys[SDL_SCANCODE_A])
	{
		player->vel.x -= playerSpeed;
	}
	else if (game.input.keys[SDL_SCANCODE_D])
	{
		player->vel.x += playerSpeed;
	}


	if (game.input.keys[SDL_SCANCODE_J] && player->isGrounded)
	{
		player->vel.y -= 450.0;
		player->isGrounded = 0;
	}

	if (game.input.keys[SDL_SCANCODE_R])
		player->pos = startPos;

	if (player->vel.x < 0)
		player->direction = LEFT;
	else if (player->vel.x > 0)
		player->direction = RIGHT;

	if (game.input.keys[SDL_SCANCODE_SPACE] && SDL_GetTicks() - lastFireTime > fireRate)
	{
		if(ammo>0 )
			FireGun();
		lastFireTime = SDL_GetTicks();
		game.input.keys[SDL_SCANCODE_SPACE] = 0;
	}
		

	HandleCamera();

}

static void RenderPlayer(void)
{
	Vec2 screenPos = { player->pos.x - game.camera.x,player->pos.y - game.camera.y };
	//BlitTexture(player->texture, screenPos.x, screenPos.y);
	SDL_RendererFlip flip = player->direction == RIGHT ? 0 : 1;
	if(player->vel.x !=0 && player->isGrounded)
		PlayAnimatedSprite(&walkAnimation, screenPos.x, screenPos.y, flip,1);
	else
		PlayAnimatedSprite(&walkAnimation, screenPos.x, screenPos.y, flip, 0);

	RenderHud();
}

static void RenderHud(void)
{
	Vec2 keyPos = { 200,64 };
	int i;
	for (i = 0; i < MAX_KEYS; i++)
	{
		if(keys[i])
			BlitTexture(keyTextures[i], keyPos.x + (i* 64), keyPos.y);
	}
	/*
	char ammoDisplay[50];
	sprintf(ammoDisplay, "Ammo: %d", ammo);
	DrawText(50, 40, ammoDisplay);
	char healthDisplay[50];
	sprintf(healthDisplay, "Health: %d", player->health);
	DrawText(480, 40, healthDisplay);*/
}

static void PlayerOnHit(Entity* self, Entity* other)
{
	//TODO: separate into its own function and create a damage delay.

	if (other->tag == TAG_ENEMY)
		self->health--;

	
	if (self->health <= 0)
		self->isActive = 0;
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