#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdbool.h>
#include "../constants.h"

#define MAX_SFX_CHANNELS 6

enum
{
	SFX_SHOOT,
	SFX_SHOOT2,
	SFX_DOOR_OPEN,
	SFX_JUMP,
	SFX_PICKUP,
	SFX_ENEMY_DEATH,
	SFX_MAX
};

typedef struct Music Music;

struct Music
{
	char id[MAX_FILENAME];
	char filePath[MAX_FILENAME];
	Music* next;
};

void InitSounds(void);
void PlaySound(int sound, int channel);
void LoadMusic(char* id);
void PlayMusic(void);
void CleanupSound(void);
void SetSoundMute(bool value);
void SetMusicMute(bool value);

#endif