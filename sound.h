#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>

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

void InitSounds(void);
void PlaySound(int sound, int channel);
void LoadMusic(char* path);
void PlayMusic(void);
void CleanupSound(void);

#endif