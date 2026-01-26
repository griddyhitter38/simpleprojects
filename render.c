#include "headers/render.h"
#include <math.h>

static float edgeXAtY(Vec2 a, Vec2 b, float y)
{
    float dy = b.y - a.y;
    if (fabsf(dy) < 0.0001f) return a.x;
    return a.x + (y - a.y) * (b.x - a.x) / dy;
}

void renderSquare(SDL_Renderer *renderer, const Square *s)
{
    SDL_SetRenderDrawColor(renderer, s->R, s->G, s->B, s->A);
    SDL_Rect r = { (int)(s->pos.x - s->half), (int)(s->pos.y - s->half),
                   s->half * 2, s->half * 2 };
    SDL_RenderFillRect(renderer, &r);
}

void renderTriangle(SDL_Renderer *renderer, const Triangle *t)
{
    SDL_SetRenderDrawColor(renderer, t->R, t->G, t->B, t->A);

    Vec2 v0 = t->v[0];
    Vec2 v1 = t->v[1];
    Vec2 v2 = t->v[2];

    if (v1.y < v0.y) { Vec2 tmp = v0; v0 = v1; v1 = tmp; }
    if (v2.y < v0.y) { Vec2 tmp = v0; v0 = v2; v2 = tmp; }
    if (v2.y < v1.y) { Vec2 tmp = v1; v1 = v2; v2 = tmp; }

    int yStart = (int)ceilf(v0.y);
    int yMid = (int)ceilf(v1.y);
    int yEnd = (int)floorf(v2.y);

    for (int y = yStart; y < yMid; y++) {
        float x1 = edgeXAtY(v0, v1, (float)y);
        float x2 = edgeXAtY(v0, v2, (float)y);
        int xi1 = (int)roundf(x1);
        int xi2 = (int)roundf(x2);
        if (xi1 > xi2) { int t = xi1; xi1 = xi2; xi2 = t; }
        SDL_RenderDrawLine(renderer, xi1, y, xi2, y);
    }

    for (int y = yMid; y <= yEnd; y++) {
        float x1 = edgeXAtY(v1, v2, (float)y);
        float x2 = edgeXAtY(v0, v2, (float)y);
        int xi1 = (int)roundf(x1);
        int xi2 = (int)roundf(x2);
        if (xi1 > xi2) { int t = xi1; xi1 = xi2; xi2 = t; }
        SDL_RenderDrawLine(renderer, xi1, y, xi2, y);
    }
}

void drawFilledKirkle(SDL_Renderer *renderer, Vec2 center, int radius)
{
    for (int dy = -radius; dy <= radius; dy++) {
        int y = (int)center.y + dy;
        int dx = (int)sqrt(radius*radius - dy*dy);
        SDL_RenderDrawLine(renderer, (int)center.x - dx, y, (int)center.x + dx, y);
    }
}

void renderr(SDL_Renderer *renderer, Vec2 center, int radius, const Square *sq, const Triangle *tri)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawFilledKirkle(renderer, center, radius);
    renderSquare(renderer, sq);
    renderTriangle(renderer, tri);

    SDL_RenderPresent(renderer);
}
