#include "graphics.h"

#define MAX_GLYPHS 256
#define FONT_TEXTURE_SIZE 512
#define FONT_SIZE 50

TTF_Font* font;
SDL_Texture* fontTexture;
SDL_Rect glyphs[MAX_GLYPHS];

void BlitTexture(SDL_Texture* texture, int x, int y)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(game.renderer, texture, NULL, &dest);
}

//I'll admit that I took inspiration from Parallel Realities for this font atlas method.
void InitFont(void)
{
	SDL_Surface* fontSurface, *stringSurface;
	char c[2];
	int i;
	SDL_Rect dest;
	SDL_Color white = { 255,255,255,255 };

	memset(&glyphs, 0, sizeof(SDL_Rect) * MAX_GLYPHS);
	
	font = TTF_OpenFont("assets/fonts/DigitalJots.ttf", FONT_SIZE);

	fontSurface = SDL_CreateRGBSurface(0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff);
	SDL_SetColorKey(fontSurface, SDL_TRUE, SDL_MapRGBA(fontSurface->format, 0, 0, 0, 0));

	dest.x = dest.y = 0;

	for (i = ' '; i <= 'z'; i++)
	{
		c[0] = i;
		c[1] = 0;
		stringSurface = TTF_RenderUTF8_Blended(font, c, white);
		TTF_SizeText(font, c, &dest.w, &dest.h);

		if (dest.x + dest.w >= FONT_TEXTURE_SIZE)
		{
			dest.x = 0;
			dest.y += dest.h + 1;

			if (dest.y + dest.h >= FONT_TEXTURE_SIZE)
			{
				printf("The font texture has run out of space.\n");
				exit(1);
			}
		}
		SDL_BlitSurface(stringSurface, NULL, fontSurface, &dest);

		glyphs[i].x = dest.x;
		glyphs[i].y = dest.y;
		glyphs[i].w = dest.w;
		glyphs[i].h = dest.h;

		SDL_FreeSurface(stringSurface);
		dest.x += dest.w;
	}

	fontTexture = SDL_CreateTextureFromSurface(game.renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
}

void DrawText(int x, int y, char* text, int r, int g, int b)
{
	
	char c;
	int i;
	SDL_Rect dest;
	int wordLength = strlen(text);

	SDL_SetTextureColorMod(fontTexture, r, g, b);

	//Loop through each character in string
	for (i=0; i < wordLength; i++)
	{
		c = text[i];
		
		dest.x = x;
		dest.y = y;
		dest.w = glyphs[c].w;
		dest.h = glyphs[c].h;
		SDL_RenderCopy(game.renderer, fontTexture, &glyphs[c], &dest);
		x += dest.w;
	}
	
}

void PlayAnimatedSprite(AnimatedSprite* sprite, int x, int y, SDL_RendererFlip flip)
{
	SDL_Rect src;
	SDL_Rect dest;

	src.x = sprite->sx + sprite->currentFrame * sprite->width;
	src.y = sprite->sy;
	src.w = sprite->width;
	src.h = sprite->height;

	dest.x = x;
	dest.y = y;
	dest.w = sprite->width;
	dest.h = sprite->height;

	SDL_RenderCopyEx(game.renderer, sprite->texture, &src, &dest, 0, NULL, flip);

	if(sprite->speed > 0)
		sprite->currentFrame = (int)(SDL_GetTicks()/100) % sprite->maxFrames;
}
