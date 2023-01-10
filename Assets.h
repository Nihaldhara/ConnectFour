#pragma once

#include "Settings.h"

typedef struct Assets_s
{
    SDL_Texture *textureBoard;
    SDL_Texture *textureYellowPiece;
    SDL_Texture *textureRedPiece;
    SDL_Texture* textureP1;
    SDL_Texture *textureP2;
} Assets;

Assets *Assets_New(SDL_Renderer *renderer);
void Assets_Delete(Assets *self);
