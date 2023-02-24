#ifndef INPUT_H
#define INPUT_H

#include "main.h"

#define MAX_KEYBOARD 256

#define MAX_INPUTS 5

//This is what the user would use to get input
enum
{
	INP_LEFT,
	INP_RIGHT,
	INP_JUMP,
	INP_SUBMIT,
	INP_FIRE1
};

//The following are accessed from the main loop only
void ProcessKeyboardUp(SDL_Event* event);
void ProcessKeyboardDown(SDL_Event* event);
void ProcessJoyButtonDown(SDL_Event* event);
void ProcessJoyButtonUp(SDL_Event* event);

void UpdateInputs(void);

//Accessed from anywhere in the program
int GetButton(int inputType);
int GetButtonDown(int inputType);

#endif INPUT_H