#pragma once

#include "Settings.h"
#include "Timer.h"

SDL_bool SDL_PointInFRect(SDL_Point* p, SDL_FRect* r);
void App_Init(int sdlFlags, int imgFlags);
void App_Quit();

extern Timer *g_time;
