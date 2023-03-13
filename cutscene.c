#include "cutscene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static CutsceneScript currentCutScene;
static ImageBox imageBox;
static TextBox textBox;

static void InterpretScript(char line[SCRIPT_LINE_SIZE], int i);
static void GetCommand(char buffer[COMMAND_SIZE], char line[SCRIPT_LINE_SIZE]);
static void GetStringParam(char buffer[PARAM_SIZE], char line[SCRIPT_LINE_SIZE]);
static int GetIntParam(char buffer[PARAM_SIZE], char line[SCRIPT_LINE_SIZE]);
static void RenderCutscene(void);
static void CleanupCutscene(void);

static int curChar;

static int isFadingOut = 0;

void LoadCutscene(char* filePath)
{
	memset(&currentCutScene, 0, sizeof(CutsceneScript));

	int i = 0;
	char line[SCRIPT_LINE_SIZE];
	FILE* f = fopen(filePath, "r");
	if (!f) { printf("Cutscene file could be loaded.\n"); exit(1); }

	do{
		fgets(line, SCRIPT_LINE_SIZE, f);
		InterpretScript(line, i);
		i++;
	} while (!feof(f));

	currentCutScene.maxLines = i;
	currentCutScene.currentLine = 0;
	currentCutScene.waitForInput = 0;
	currentCutScene.waitTime = 0;

	fclose(f);

	//TODO: might want to separate everything below into its own function
	game.update = PlayCutscene;
	game.render = RenderCutscene;
	game.cleanup = CleanupCutscene;

	imageBox.isVisible = 0;
	imageBox.x = 0;
	imageBox.y = 0;

	textBox.isVisible = 0;
	textBox.x = SCREEN_WIDTH/2 - 100;
	textBox.y = 500;
	
}

//Converts GabeScript to binary code format
static void InterpretScript(char line[SCRIPT_LINE_SIZE], int i)
{
	char command[COMMAND_SIZE];
	char param[PARAM_SIZE];
	curChar = 0;
	
	if (line[curChar] == "\n" || line[curChar]=='#')return;

	GetCommand(command, line);
	printf("Command Created: %s\n", command);

	ScriptCommand scriptCommand = {0,'\0'};

	if (strncmp(command, "ShowImage",COMMAND_SIZE) == 0)
	{
		scriptCommand.command = SHOW_IMAGE;

		GetStringParam(param, line);
		strcpy(scriptCommand.stringParam, param);
	}
	else if (strncmp(command, "WriteText",COMMAND_SIZE) == 0)
	{
		scriptCommand.command = WRITE_TEXT;

		GetStringParam(param, line);
		strcpy(scriptCommand.stringParam, param);
	}
	else if (strncmp(command, "WaitInput",COMMAND_SIZE) == 0)
	{
		scriptCommand.command = WAIT_INPUT;
	}
	else if (strncmp(command, "WaitSecs", COMMAND_SIZE) == 0)
	{
		scriptCommand.command = WAIT_SECS;
		scriptCommand.numParam = GetIntParam(param, line) * 1000; //convert from secs to millisecs
		printf("Wait num of secs: %d\n", scriptCommand.numParam);
	}
	else if (strncmp(command, "Clear", COMMAND_SIZE) == 0)
	{
		scriptCommand.command = CLEAR;
	}
	else if (strncmp(command, "FadeOut", COMMAND_SIZE) == 0)
	{
		scriptCommand.command = FADE_OUT;
	}
	else if (strncmp(command, "LoadLevel", COMMAND_SIZE) == 0)
	{
		scriptCommand.command = LOAD_LEVEL;
	}
	else
	{
		printf("Invalid script command: %s.\n",command);
		scriptCommand.command = INVALID_COMMAND;
	}

	currentCutScene.scriptLine[i].command = scriptCommand.command;
	currentCutScene.scriptLine[i].numParam = scriptCommand.numParam;
	strcpy(currentCutScene.scriptLine[i].stringParam, scriptCommand.stringParam);
}

static void GetCommand(char buffer[COMMAND_SIZE], char line[SCRIPT_LINE_SIZE])
{
	int i = 0;

	while (line[curChar] != '\n' && line[curChar] != ' ' && line[curChar] !='\0')
	{
		buffer[i] = line[curChar];
		i++;
		curChar++;
	}
	buffer[i] = '\0';

}

static void GetStringParam(char buffer[PARAM_SIZE], char line[SCRIPT_LINE_SIZE])
{
	int i = 0;
	curChar++; //skip white space
	curChar++; //skip "

	while (line[curChar] != '\n' && line[curChar] != '"')
	{
		buffer[i] = line[curChar];
		i++;
		curChar++;
	}
	curChar++; //skip closing "

	buffer[i] = '\0';
}

static int GetIntParam(char buffer[PARAM_SIZE], char line[SCRIPT_LINE_SIZE])
{
	int i = 0;
	curChar++; //skip white space

	while (line[curChar] != '\n' && line[curChar] != ' ')
	{
		buffer[i] = line[curChar];
		i++;
		curChar++;
	}

	buffer[i] = '\0';

	int num = atoi(buffer);
	return num;
}

void PlayCutscene(void)
{
	//if (isFadingOut)
		//return;

	if (currentCutScene.waitTime > 0)
	{
		if (SDL_GetTicks() - currentCutScene.lastTime > currentCutScene.waitTime)
			currentCutScene.waitTime = 0;
		else
		{
			if (GetButtonDown(INP_SUBMIT))
				currentCutScene.waitTime = 0;
			return;
		}
			
	}

	if (currentCutScene.waitForInput)
	{
		if (GetButtonDown(INP_SUBMIT))
			currentCutScene.waitForInput = 0;
		return;
	}

	ScriptCommand command = currentCutScene.scriptLine[currentCutScene.currentLine];
	printf("Cutscene command: %d\n", command.command);

	if (command.command == SHOW_IMAGE)
	{
		printf("Image ID: %s\n", command.stringParam);
		imageBox.image = GetTexture(command.stringParam);
		imageBox.isVisible = 1;
	}
	else if (command.command == WRITE_TEXT)
	{
		strcpy(textBox.text, command.stringParam);
		textBox.isVisible = 1;
	}
	else if (command.command == CLEAR)
	{
		imageBox.isVisible = 0;
		textBox.isVisible = 0;
	}
	else if (command.command == FADE_OUT)
	{
		isFadingOut = 1;
	}
	else if (command.command == WAIT_INPUT)
	{
		currentCutScene.waitForInput = 1;
	}
	else if (command.command == WAIT_SECS)
	{
		currentCutScene.waitTime = command.numParam;
		currentCutScene.lastTime = SDL_GetTicks();
	}
	else if (command.command == LOAD_LEVEL)
	{
		StartGameState();
	}

	currentCutScene.currentLine++;

	if (currentCutScene.currentLine >= currentCutScene.maxLines)
		currentCutScene.currentLine = 0; //TODO: end cutscene
}

//Will be set to the gamestate's current render loop.
static void RenderCutscene(void)
{
	if (imageBox.isVisible)
		BlitTexture(imageBox.image, imageBox.x, imageBox.y);

	if (textBox.isVisible)
		DrawText(textBox.x, textBox.y, textBox.text, 255, 255, 255);

	if (isFadingOut)
	{
		FadeOut();
	}
		
}

static void CleanupCutscene(void)
{

}