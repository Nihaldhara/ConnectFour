#include "Assets.h"

typedef struct TextureSpec_s
{
    SDL_Texture **ptr;
    char *path;
} TextureSpec;

Assets *Assets_New(SDL_Renderer *renderer)
{
    Assets *self = (Assets *)calloc(1, sizeof(Assets));
    AssertNew(self);

    // -------------------------------------------------------------------------
    // Chargement des textures

    TextureSpec texSpecs[] = {
        { &self->textureBoard, "../Assets/board.png" },
        { &self->textureYellowPiece, "../Assets/yellow_piece.png" },
        { &self->textureRedPiece, "../Assets/red_piece.png" },
        { &self->textureP1, "../Assets/player1.png"},
        { &self->textureP2, "../Assets/player2.png" },
        { &self->textureVictP1, "../Assets/player1wins.png" },
        { &self->textureVictP2, "../Assets/player2wins.png" },
        { &self->textureAIvsAI, "../Assets/AIvsAI.png" },
        { &self->texturePvsAI, "../Assets/PvsAI.png" },
        { &self->textureP1vsP2, "../Assets/P1vsP2.png" },
        { &self->textureExit, "../Assets/exit.png" },
        { &self->textureOptions, "../Assets/options.png" },
        { &self->textureConnect4, "../Assets/connect4.png" },
        { &self->textureContinue, "../Assets/continue.png" },
        { &self->textureP1wins, "../Assets/P1wins.png" },
        { &self->textureP2wins, "../Assets/P2wins.png" },
    };
    int texSpecCount = sizeof(texSpecs) / sizeof(TextureSpec);

    for (int i = 0; i < texSpecCount; i++)
    {
        SDL_Texture **texPtr = texSpecs[i].ptr;
        char *path = texSpecs[i].path;

        *texPtr = IMG_LoadTexture(renderer, path);
        if (*texPtr == NULL)
        {
            printf("ERROR - Loading texture %s\n", path);
            printf("      - %s\n", SDL_GetError());
            assert(false);
            abort();
        }
    }

    return self;
}

void Assets_Delete(Assets *self)
{
    if (!self) return;

    // -------------------------------------------------------------------------
    // Libère les textures

    SDL_Texture **texPointers[] = {
        &self->textureBoard,
        &self->textureYellowPiece,
        &self->textureRedPiece,
        &self->textureP1,
        &self->textureP2,
        &self->textureVictP1,
        &self->textureVictP2,
        &self->textureAIvsAI,
        &self->texturePvsAI,
        &self->textureP1vsP2,
        &self->textureExit,
        &self->textureOptions,
        &self->textureConnect4,
        &self->textureContinue,
        &self->textureP1wins,
        &self->textureP2wins,
    };
    int count = sizeof(texPointers) / sizeof(SDL_Texture **);

    for (int i = 0; i < count; i++)
    {
        if (*texPointers[i])
            SDL_DestroyTexture(*(texPointers[i]));
    }

    free(self);
}