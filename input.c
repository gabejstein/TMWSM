#include "input.h"

static int keys[MAX_KEYBOARD];
static int gamepadButtons[MAX_KEYBOARD];
static int inputs[MAX_INPUTS];

void ProcessKeyboardUp(SDL_Event* event)
{
	if(event->key.repeat==0)
		keys[event->key.keysym.scancode] = 0;
}

void ProcessKeyboardDown(SDL_Event* event)
{
	if (event->key.repeat == 0)
		keys[event->key.keysym.scancode] = 1;

}

void ProcessJoyButtonDown(SDL_Event* event)
{
	if (event->jbutton.state == SDL_PRESSED)
		gamepadButtons[event->jbutton.button] = 1;
}

void ProcessJoyButtonUp(SDL_Event* event)
{
	if (event->jbutton.state == SDL_PRESSED)
		gamepadButtons[event->jbutton.button] = 0;
}

void UpdateInputs(void)
{
	if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A] || gamepadButtons[SDL_CONTROLLER_BUTTON_X])
	{
		inputs[INP_LEFT] = 1;
	}
	else
	{
		inputs[INP_LEFT] = 0;
	}

	if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
	{
		inputs[INP_RIGHT] = 1;
	}
	else
	{
		inputs[INP_RIGHT] = 0;
	}

	if (keys[SDL_SCANCODE_J])
	{
		inputs[INP_JUMP] = 1;
	}
	else
	{
		inputs[INP_JUMP] = 0;
	}

	if (keys[SDL_SCANCODE_SPACE])
	{
		inputs[INP_FIRE1] = 1;
	}
	else
	{
		inputs[INP_FIRE1] = 0;
	}

	if (keys[SDL_SCANCODE_RETURN])
		inputs[INP_SUBMIT] = 1;
	else
		inputs[INP_SUBMIT] = 0;

}

int GetButton(int inputType)
{
	if (inputType > MAX_INPUTS || inputType < 0)return 0;

	return inputs[inputType];
}

//The same as above, but it resets the button press. It's good for menus, etc.
int GetButtonDown(int inputType)
{
	if (inputType > MAX_INPUTS || inputType < 0)return 0;

	if (inputs[inputType])
	{
		inputs[inputType] = 0; //reset button/key
		return 1;
	}

	return 0;
}