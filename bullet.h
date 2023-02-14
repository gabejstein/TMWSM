#ifndef BULLET_H
#define BULLET_H

#include "constants.h"

void InitBullets(void);
void SpawnBullet(float x, float y, float vx, float vy, TAG tag);

#endif