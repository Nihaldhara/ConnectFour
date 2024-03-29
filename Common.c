#include "Common.h"

Timer *g_time = NULL;

SDL_bool SDL_PointInFRect(SDL_Point* p, SDL_FRect* r)
{
    return ((p->x >= r->x) && (p->x < (r->x + r->w)) &&
        (p->y >= r->y) && (p->y < (r->y + r->h))) ? SDL_TRUE : SDL_FALSE;
}

void App_Init(int sdlFlags, int imgFlags)
{
    // Initialise la SDL2
    if (SDL_Init(sdlFlags) < 0)
    {
        printf("ERROR - SDL_Init %s\n", SDL_GetError());
        assert(false);
        abort();
    }

    // Initialise la SDL2 image
    if (IMG_Init(imgFlags) != imgFlags)
    {
        printf("ERROR - IMG_Init %s\n", IMG_GetError());
        assert(false);
        abort();
    }
}

void App_Quit()
{
    IMG_Quit();
    SDL_Quit();
}
