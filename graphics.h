#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "main.h"

#define MAX_GLYPHS 256
#define FONT_TEXTURE_SIZE 512
#define FONT_SIZE 50

typedef struct Texture Texture;

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

struct Texture
{
	SDL_Texture* texture;
	char id[256];
	Texture* next;
};

void InitFont(void);
void BlitTexture(SDL_Texture* texture, int x, int y);
void DrawText(int x, int y, char* text, int r, int g, int b);

void LoadTextures(char* resourcePath);
SDL_Texture* GetTexture(char* id);
void FreeAllTextures(void);

void PlayAnimatedSprite(AnimatedSprite* sprite, int x, int y, SDL_RendererFlip flip);

int FadeOut(void);

#endif