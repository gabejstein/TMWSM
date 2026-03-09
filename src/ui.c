#include "ui.h"
#include "system/graphics.h"
#include <stdio.h>
#include <stdlib.h>

static void RenderNineSlice(DialogueBox* box);

static DialogueBox dBox;
static SDL_Rect src[9];

void InitDialogueBox(void)
{
	memset(&dBox, 0, sizeof(DialogueBox));
	dBox.texture = GetTexture("dialogue_box");
	dBox.sliceSize = 32; //TODO: Query the texture size and automatically do this.

	int i;
	for (i = 0; i < 9; i++)
	{
		src[i].x = (i % 3) * dBox.sliceSize;
		src[i].y = i / 3 * dBox.sliceSize;
		src[i].w = dBox.sliceSize;
		src[i].h = dBox.sliceSize;
	}
}

void RenderDialogueBox(int x, int y, int w, int h)
{
	dBox.x = x;
	dBox.y = y;
	dBox.w = w;
	dBox.h = h;
	
	//This is kept as a separate function to separate the dialogue system from the UI system.
	RenderNineSlice(&dBox);
}

//What this function essentially does is splits the texture into 9 areas that are scaled according to the size specified.
static void RenderNineSlice(DialogueBox* box)
{
	
	SDL_Rect topLeft = {
		.x = box->x,
		.y = box->y,
		.w=box->sliceSize,
		.h=box->sliceSize
	};
	
	SDL_Rect topMiddle = {
		.x = box->x+box->sliceSize,
		.y = box->y,
		.w = box->w-box->sliceSize*2,
		.h = box->sliceSize
	};

	SDL_Rect topRight = {
		.x = box->x+box->w-box->sliceSize,
		.y = box->y,
		.w = box->sliceSize,
		.h = box->sliceSize
	};

	SDL_Rect middleLeft = {
		.x = box->x,
		.y = box->y+box->sliceSize,
		.w = box->sliceSize,
		.h = box->h - box->sliceSize*2
	};

	SDL_Rect middle = {
	.x = box->x + box->sliceSize,
	.y = box->y + box->sliceSize,
	.w = box->w-box->sliceSize*2,
	.h = box->h-box->sliceSize*2
	};

	SDL_Rect middleRight = {
		.x = box->x+box->w-box->sliceSize,
		.y = box->y + box->sliceSize,
		.w = box->sliceSize,
		.h = box->h - box->sliceSize * 2
	};

	SDL_Rect bottomLeft = {
		.x = box->x,
		.y = box->y + box->h - box->sliceSize,
		.w = box->sliceSize,
		.h = box->sliceSize
	};

	SDL_Rect bottomMiddle = {
		.x = box->x + box->sliceSize,
		.y = box->y + box->h-box->sliceSize,
		.w = box->w - box->sliceSize * 2,
		.h = box->sliceSize
	};

	SDL_Rect bottomRight = {
		.x = box->x + box->w - box->sliceSize,
		.y = box->y + box->h - box->sliceSize,
		.w = box->sliceSize,
		.h = box->sliceSize
	};

	SDL_RenderCopy(game.renderer, box->texture, &src[0], &topLeft);
	SDL_RenderCopy(game.renderer, box->texture, &src[1], &topMiddle);
	SDL_RenderCopy(game.renderer, box->texture, &src[2], &topRight);
	SDL_RenderCopy(game.renderer, box->texture, &src[3], &middleLeft);
	SDL_RenderCopy(game.renderer, box->texture, &src[4], &middle);
	SDL_RenderCopy(game.renderer, box->texture, &src[5], &middleRight);
	SDL_RenderCopy(game.renderer, box->texture, &src[6], &bottomLeft);
	SDL_RenderCopy(game.renderer, box->texture, &src[7], &bottomMiddle);
	SDL_RenderCopy(game.renderer, box->texture, &src[8], &bottomRight);

	
}