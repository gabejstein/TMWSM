#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../constants.h"

typedef struct TextureResource
{
	SDL_Texture* texture;
	char id[256];
	struct TextureResource* next;
}TextureResource;

//Font-related stuff

static TTF_Font* font;
static SDL_Texture* fontTexture;
static SDL_Rect glyphs[MAX_GLYPHS];

//Texture-related stuff
//Linked list to hold all textures
static TextureResource* textureListHead = NULL;
static SDL_Renderer* sRenderer = NULL;

static void AddTexture(char* id, char* path);

void Graphics_SetRenderer(const SDL_Renderer* renderer)
{
	if (!renderer)
	{
		printf("Problem setting renderer.\n");
		exit(1);
	}

	sRenderer = renderer;
}

void Graphics_BlitTexture(SDL_Texture* texture, int x, int y)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(sRenderer, texture, NULL, &dest);
}

//I'll admit that I took inspiration from Parallel Realities for this font atlas method.
void Graphics_InitFont(void)
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

	fontTexture = SDL_CreateTextureFromSurface(sRenderer, fontSurface);
	SDL_FreeSurface(fontSurface);
}

void Graphics_DrawText(int x, int y, char* text, int r, int g, int b)
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
		SDL_RenderCopy(sRenderer, fontTexture, &glyphs[c], &dest);
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

	SDL_RenderCopyEx(sRenderer, sprite->texture, &src, &dest, 0, NULL, flip);

	if(sprite->speed > 0)
		sprite->currentFrame = (int)(SDL_GetTicks()/100) % sprite->maxFrames;

	sprite->speed = 0;
}

static void AddTexture(char* id, char* path)
{
	SDL_Surface* surface;
	SDL_Texture* texture;
	surface = IMG_Load(path);
	texture = SDL_CreateTextureFromSurface(sRenderer, surface);
	SDL_FreeSurface(surface);

	TextureResource* textureObject = (TextureResource*)malloc(sizeof(TextureResource));
	memset(textureObject, 0, sizeof(TextureResource));
	strncpy(textureObject->id,id,256);
	textureObject->texture = texture;
	textureObject->next = NULL;

	if (textureListHead == NULL)
	{
		textureListHead = textureObject;
		return;
	}

	textureObject->next = textureListHead;
	textureListHead = textureObject;
}

//Loads all the textures at once based on the resource list.
void Graphics_LoadTextures(const char* resourcePath)
{
	char textID[256], textPath[256];

	FILE* f = fopen(resourcePath, "r");
	if (!f) { printf("Could not open sprite resource file.\n"); exit(1); }
	do {
		fscanf(f, "%s %s\n", textID, textPath);
		printf("Adding texture at ID: %s and path: %s\n", textID, textPath);
		AddTexture(textID, textPath);
	} while (!feof(f));
	
	fclose(f);
}

//Pulls textures from repository.
SDL_Texture* Graphics_GetTexture(const char* id)
{
	TextureResource* current;
	for (current = textureListHead; current != NULL; current = current->next)
	{
		if (strcmp(id, current->id) == 0)
		{
			return current->texture;
		}
	}

	printf("Could not find texture at id: %s\n", id);
	return NULL;
}

void Graphics_FreeAllTextures(void)
{
	printf("Freeing texture resources.\n");
	TextureResource* current = textureListHead;
	TextureResource* temp;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		SDL_DestroyTexture(temp->texture);
		free(temp);
	}
	printf("Texture resources freed.\n");
}

static double fadeAmount = 0;
int FadeOut(float dt)
{
	fadeAmount += dt * 300;
	if (fadeAmount >= 255)
	{
		fadeAmount = 255;
		return 1;
	}
	SDL_SetRenderDrawColor(sRenderer,0, 0, 0, fadeAmount);
	SDL_Rect blackScreen = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	SDL_RenderFillRect(sRenderer, &blackScreen);
	SDL_SetRenderDrawColor(sRenderer,0, 0, 0, 0);

	return 0;
}
