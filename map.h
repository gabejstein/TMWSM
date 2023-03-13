#ifndef MAP_H
#define MAP_H

void LoadMap(char* path);
void RenderMap(void);
void UnloadMap(void);

int GetTile(int x, int y, int layer);
int IsCollisionTile(int x, int y, int layer);
int IsOneWayCollider(int x, int y, int layer);

#endif