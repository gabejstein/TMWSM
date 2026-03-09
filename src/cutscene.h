#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "main.h"

#define MAX_SCRIPT_LINES 512
#define SCRIPT_LINE_SIZE 512 //This is the total line length for the script text
#define COMMAND_SIZE 256
#define PARAM_SIZE 256

#define TEXTBOX_SIZE 256

//How the command types will be stored internally
enum
{
	INVALID_COMMAND,
	SHOW_IMAGE,
	WRITE_TEXT,
	CLEAR,
	WAIT_INPUT,
	WAIT_SECS,
	FADE_OUT,
	LOAD_LEVEL
};

typedef struct
{
	int command;
	char stringParam[PARAM_SIZE];
	int numParam;
}ScriptCommand;

typedef struct
{
	char* filePath;
	ScriptCommand scriptLine[MAX_SCRIPT_LINES];
	int currentLine;
	int maxLines;
	float waitTime;
	float lastTime;
	int waitForInput;
}CutsceneScript;

typedef struct
{
	SDL_Texture* image;
	int x;
	int y;
	int isVisible;
}ImageBox;

typedef struct
{
	char text[TEXTBOX_SIZE];
	int x;
	int y;
	int isVisible;
}TextBox;

void LoadCutscene(char* filePath);
void PlayCutscene(void);


#endif