#include "headers/input_drag.h"
#include <SDL2/SDL.h>

bool pointInKirkle(int mx, int my, Vec2 c, int r)
{
    float dx = mx - c.x;
    float dy = my - c.y;
    return dx*dx + dy*dy <= (float)(r*r);
}

bool pointInSquare(int mx, int my, const Square *s)
{
    return mx >= s->pos.x - s->half && mx <= s->pos.x + s->half &&
           my >= s->pos.y - s->half && my <= s->pos.y + s->half;
}

void handleEvents(bool *running,
                  DragTarget *dragTarget,
                  float *grabOffsetX, float *grabOffsetY,
                  Vec2 *cpos, Vec2 *cvel, int cr,
                  Square *square)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) *running = false;
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x;
            int my = e.button.y;
            if (pointInSquare(mx, my, square)) {
                *dragTarget = DRAG_SQUARE;
                *grabOffsetX = mx - square->pos.x;
                *grabOffsetY = my - square->pos.y;
                square->vel.x = square->vel.y = 0.0f;
            } else if (pointInKirkle(mx, my, *cpos, cr)) {
                *dragTarget = DRAG_KIRKLE;
                *grabOffsetX = mx - cpos->x;
                *grabOffsetY = my - cpos->y;
                cvel->x = cvel->y = 0.0f;
            }
        }
        if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
            *dragTarget = DRAG_NONE;
    }
}
