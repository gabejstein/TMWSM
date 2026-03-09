#ifndef ENTITY_H
#define ENTITY_H

#include "main.h"

#define MAX_ENTITIES 200

void InitEntities(void);
void UpdateEntities(void);
void RenderEntities(void);
void AddEntity(Entity* e);
void HandleCollisions(void);
void ClearEntities(void);

Entity* NewEntity(void);

#endif