#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"

void CreatePlayer(float x, float y);
void GetKey(KEY keyType);
int HasKey(int keyColor);
Entity* GetPlayer(void);

#endif