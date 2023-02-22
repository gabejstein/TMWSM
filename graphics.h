#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "main.h"

void InitFont(void);
void BlitTexture(SDL_Texture* texture, int x, int y);
void DrawText(int x, int y, char* text, int r, int g, int b);

void LoadTextures(void);
Texture* GetTexture(char id[256]);

typedef struct
{
	int currentFrame;
	int maxFrames;
	float speed;
	int sx;
	int sy;
	int width;
	int height;
	SDL_Texture* texture;
}AnimatedSprite;

void PlayAnimatedSprite(AnimatedSprite* sprite, int x, int y, SDL_RendererFlip flip);

#endif