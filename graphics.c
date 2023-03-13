#include "graphics.h"
#include <stdio.h>
#include <string.h>

//Font-related stuff

static TTF_Font* font;
static SDL_Texture* fontTexture;
static SDL_Rect glyphs[MAX_GLYPHS];

//Texture-related stuff
//Linked list to hold all textures
static Texture* textureListHead = NULL;

static void AddTexture(char* id, char* path);

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

	sprite->speed = 0;
}

static void AddTexture(char* id, char* path)
{
	SDL_Surface* surface;
	SDL_Texture* texture;
	surface = IMG_Load(path);
	texture = SDL_CreateTextureFromSurface(game.renderer, surface);
	SDL_FreeSurface(surface);

	Texture* textureObject = (Texture*)malloc(sizeof(Texture));
	memset(textureObject, 0, sizeof(Texture));
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
void LoadTextures(char* resourcePath)
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
SDL_Texture* GetTexture(char* id)
{
	Texture* current;
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

void FreeAllTextures(void)
{
	printf("Freeing texture resources.\n");
	Texture* current = textureListHead;
	Texture* temp;
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
int FadeOut(void)
{
	fadeAmount += game.deltaTime * 300;
	if (fadeAmount >= 255)
	{
		fadeAmount = 255;
		return 1;
	}
	SDL_SetRenderDrawColor(game.renderer,0, 0, 0, fadeAmount);
	SDL_Rect blackScreen = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	SDL_RenderFillRect(game.renderer, &blackScreen);
	SDL_SetRenderDrawColor(game.renderer,0, 0, 0, 0);

	return 0;
}
