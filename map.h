#ifndef MAP_H
#define MAP_H

void LoadMap(char* path);
void LoadEntities(char* filePath);
void RenderMap(void);
void UnloadMap(void);

int GetTile(int x, int y, int layer);

#endif