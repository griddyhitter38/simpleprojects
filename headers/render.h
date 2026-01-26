#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "physics.h"

void renderSquare(SDL_Renderer *renderer, const Square *s);
void renderTriangle(SDL_Renderer *renderer, const Triangle *t);
void drawFilledKirkle(SDL_Renderer *renderer, Vec2 center, int radius);
void renderr(SDL_Renderer *renderer, Vec2 center, int radius, const Square *sq, const Triangle *tri);

#endif
