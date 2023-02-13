#include "graphics.h"

TTF_Font* font;

void BlitTexture(SDL_Texture* texture, int x, int y)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(game.renderer, texture, NULL, &dest);
}

void InitFont(void)
{
	font = TTF_OpenFont("assets/fonts/DigitalJots.ttf", 100);
}

void DrawText(int x, int y, char* text)
{
	SDL_Color color = { 255,255,255,255 };
	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(game.renderer,surface);
	SDL_FreeSurface(surface);

	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(game.renderer, texture, NULL, &dest);
}

void PlayAnimatedSprite(AnimatedSprite* sprite, int x, int y, SDL_RendererFlip flip, float speed)
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

	if(speed > 0)
		sprite->currentFrame = (int)(SDL_GetTicks()/100) % sprite->maxFrames;
}
