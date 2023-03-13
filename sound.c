#include "sound.h"

static Mix_Chunk* sfxs[SFX_MAX];
static Mix_Music* music = NULL;

void InitSounds(void)
{
	memset(&sfxs, 0, sizeof(Mix_Chunk*) * SFX_MAX);

	sfxs[SFX_SHOOT] = Mix_LoadWAV("assets/sounds/laserRetro_000.ogg");
	sfxs[SFX_SHOOT2] = Mix_LoadWAV("assets/sounds/laserRetro_001.ogg");
	sfxs[SFX_DOOR_OPEN] = Mix_LoadWAV("assets/sounds/doorOpen_002.ogg");
	sfxs[SFX_ENEMY_DEATH] = Mix_LoadWAV("assets/sounds/war_medic.ogg");
	sfxs[SFX_PICKUP] = Mix_LoadWAV("assets/sounds/confirmation_003.ogg");

	LoadMusic("assets/sounds/bensound-dance.mp3");
}

void PlaySound(int sound, int channel)
{
	Mix_PlayChannel(-1,sfxs[sound], 0);
}

void LoadMusic(char* path)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	music = Mix_LoadMUS(path);
}

void PlayMusic(void)
{
	Mix_PlayMusic(music, -1);
}

void CleanupSound(void)
{
	printf("Cleaning up sound effects and music.\n");
	int i;
	for (i = 0; i < SFX_MAX; i++)
		Mix_FreeChunk(sfxs[i]);

	if (music != NULL)
		Mix_FreeMusic(music);

	Mix_CloseAudio();
}