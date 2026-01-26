#ifndef PHYSICS_H
#define PHYSICS_H

#include <SDL2/SDL.h>

#define HEIGHT 1000
#define WIDTH  800

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    Vec2 pos;
    Vec2 vel;
    Vec2 acc;
    int half;
    Uint8 R, G, B, A;
} Square;

typedef struct {
    Vec2 v[3];
    Uint8 R, G, B, A;
} Triangle;

void updatePhysics(Vec2 *pos, Vec2 *vel, Vec2 *acc, int radius, float dt);

void updateSquare(Square *s, float dt);

void resolveKirkleSquareCollision(Vec2 *cpos, Vec2 *cvel,
                                  int r, Square *sq);

void clampKirkleToBounds(Vec2 *pos, Vec2 *vel, int r);
void clampSquareToBounds(Square *s);

#endif
