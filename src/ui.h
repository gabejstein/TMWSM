#ifndef UI_H
#define UI_H

#include "main.h"

typedef struct
{
	int x;
	int y;
	int w;
	int h;
	SDL_Texture* texture;
	int sliceSize; //what size the pieces of the box are
}DialogueBox;

void InitDialogueBox(void);
void RenderDialogueBox(int x, int y, int w, int h);

#endif