#include "Input.h"
#include "Math.h"
#include "Common.h"

#define ORIGIN_X 174
#define ORIGIN_Y 709

Input *Input_New()
{
    Input *self = (Input *)calloc(1, sizeof(Input));
    AssertNew(self);

    return self;
}

void Input_Delete(Input *self)
{
    if (!self) return;
    free(self);
}

void Input_Update(Input *self)
{
    self->quitPressed = false;
    self->leftClick = false;

    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            self->quitPressed = true;
            break;

        case SDL_KEYDOWN:
            if (evt.key.repeat)
                break;

            switch (evt.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                self->quitPressed = true;
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            if (evt.key.repeat)
                break;

            switch (evt.key.keysym.scancode)
            {
            default:
                break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (evt.button.button == SDL_BUTTON_LEFT)
                self->leftClick = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (evt.button.button == SDL_BUTTON_LEFT)
                self->leftClick = true;

        case SDL_MOUSEMOTION:
           
            self->mouseXPosition = evt.motion.x;
            self->mouseYPosition = evt.motion.y;
        }
    }
}
