#ifndef MAIN_H
#define MAIN_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string.h>

#include "constants.h"
#include "system/input.h"
#include "structs.h"
#include "system/graphics.h"
#include "system/sound.h"

#include "gamestate.h"
#include "entity.h"
#include "entity/player.h"
#include "entity/bullet.h"
#include "hud.h"
#include "ui.h"

#include "cutscene.h"


extern Game game;

extern UserData user;

#endif