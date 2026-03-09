#include "sound.h"
#include <stdio.h>

static Mix_Chunk* sfxs[SFX_MAX];
static Mix_Music* music = NULL;

static Music* musicList = NULL;

static void LoadMusicResource(char* filePath);
static char* GetSongPath(char* id);
static void CleanupMusicResource(void);

static bool isSoundMuted = false;
static bool isMusicMuted = false;

void SetSoundMute(bool value)
{
	isSoundMuted = value;
}

void SetMusicMute(bool value)
{
	isMusicMuted = value;
}

//All of this is really just a convenient way to load music without having to enter long filepaths
static void LoadMusicResource(char* filePath)
{
	FILE* f = fopen(filePath, "r");
	if (f)
	{
		do
		{
			Music* m = (Music*)malloc(sizeof(Music));
			memset(m, 0, sizeof(Music));
			
			fscanf(f, "%s %s\n", m->id, m->filePath);
			printf("Got music, id: %s filepath: %s\n", m->id, m->filePath);
			m->next = musicList;
			musicList = m;

		} while (!feof(f));

		fclose(f);
	}
	else
	{
		printf("Could not load music resource file.\n");
		exit(1);
	}

}

static char* GetSongPath(char* id)
{
	Music* m = musicList;
	while (m != NULL)
	{
		if (strcmp(id, m->id) == 0)
		{
			return m->filePath;
		}
		m = m->next;
	}

	return NULL;
}

static void CleanupMusicResource(void)
{
	printf("Clearing Music Resource\n");

	Music* m = musicList;
	Music* t;
	while (m != NULL)
	{
		t = m;
		m = m->next;
		free(t);
	}
}

void InitSounds(void)
{
	memset(&sfxs, 0, sizeof(Mix_Chunk*) * SFX_MAX);

	sfxs[SFX_SHOOT] = Mix_LoadWAV("assets/sounds/laserRetro_000.ogg");
	sfxs[SFX_SHOOT2] = Mix_LoadWAV("assets/sounds/laserRetro_001.ogg");
	sfxs[SFX_DOOR_OPEN] = Mix_LoadWAV("assets/sounds/doorOpen_002.ogg");
	sfxs[SFX_ENEMY_DEATH] = Mix_LoadWAV("assets/sounds/war_medic.ogg");
	sfxs[SFX_PICKUP] = Mix_LoadWAV("assets/sounds/confirmation_003.ogg");

	LoadMusicResource("assets/music_resource.txt");
}

void PlaySound(int sound, int channel)
{
	if (isSoundMuted) return;
	Mix_PlayChannel(-1,sfxs[sound], 0);
}

void LoadMusic(char* id)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	char* path = GetSongPath(id);
	music = Mix_LoadMUS(path);
}

void PlayMusic(void)
{
	if (isMusicMuted) return;
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

	CleanupMusicResource();

	Mix_CloseAudio();
}