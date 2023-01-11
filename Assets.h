#pragma once

#include "Settings.h"

typedef struct Assets_s
{
    SDL_Texture *textureBoard;
    SDL_Texture *textureYellowPiece;
    SDL_Texture *textureRedPiece;
    SDL_Texture *textureP1;
    SDL_Texture *textureP2;
    SDL_Texture *textureVictP1;
    SDL_Texture *textureVictP2;
    SDL_Texture *textureAIvsAI;
    SDL_Texture *texturePvsAI;
    SDL_Texture *textureP1vsP2;
    SDL_Texture *textureExit;
    SDL_Texture *textureOptions;
    SDL_Texture *textureConnect4;
    SDL_Texture *textureContinue;
    SDL_Texture *textureP1wins;
    SDL_Texture *textureP2wins;
} Assets;

Assets *Assets_New(SDL_Renderer *renderer);
void Assets_Delete(Assets *self);
