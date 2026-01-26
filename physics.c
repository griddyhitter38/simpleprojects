#include "headers/physics.h"
#include <math.h>

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void updatePhysics(Vec2 *pos, Vec2 *vel, Vec2 *acc, int radius, float dt)
{
    const float drag = 0.98f;
    const float inputAccel = 20000.0f;
    const float g0 = 1000.0f;
    const float k  = 9.81f;
    const float ayLimit = 40000.0f;
    static float y0 = 0.0f;

    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    acc->x = 0.0f;
    acc->y = 0.0f;

    if (keys[SDL_SCANCODE_LEFT])  acc->x -= inputAccel;
    if (keys[SDL_SCANCODE_RIGHT]) acc->x += inputAccel;
    if (keys[SDL_SCANCODE_UP])    acc->y -= inputAccel;
    if (keys[SDL_SCANCODE_DOWN])  acc->y += inputAccel;

    float dy = pos->y - y0;
    if (dy < 0.0f) dy = 0.0f;

    float gravityAy = g0 + k * dy;
    if (gravityAy > ayLimit) gravityAy = ayLimit;

    acc->y += gravityAy;

    vel->x += acc->x * dt;
    vel->y += acc->y * dt;

    vel->x *= drag;
    vel->y *= drag;

    pos->x += vel->x * dt;
    pos->y += vel->y * dt;
}

void updateSquare(Square *s, float dt)
{
    const float drag = 0.98f;
    const float inputAccel = 20000.0f;
    const float g0 = 1000.0f;
    const float k  = 9.81f;
    const float ayLimit = 40000.0f;
    static float y0 = 0.0f;

    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    s->acc.x = 0.0f;
    s->acc.y = 0.0f;

    if (keys[SDL_SCANCODE_A]) s->acc.x -= inputAccel;
    if (keys[SDL_SCANCODE_D]) s->acc.x += inputAccel;
    if (keys[SDL_SCANCODE_W]) s->acc.y -= inputAccel;
    if (keys[SDL_SCANCODE_S]) s->acc.y += inputAccel;

    float dy = s->pos.y - y0;
    if (dy < 0.0f) dy = 0.0f;

    float gravityAy = g0 + k * dy;
    if (gravityAy > ayLimit) gravityAy = ayLimit;

    s->acc.y += gravityAy;

    s->vel.x += s->acc.x * dt;
    s->vel.y += s->acc.y * dt;

    s->vel.x *= drag;
    s->vel.y *= drag;

    s->pos.x += s->vel.x * dt;
    s->pos.y += s->vel.y * dt;
}

void resolveKirkleSquareCollision(Vec2 *cpos, Vec2 *cvel,
                                  int r, Square *sq)
{
    float left   = sq->pos.x - sq->half;
    float right  = sq->pos.x + sq->half;
    float top    = sq->pos.y - sq->half;
    float bottom = sq->pos.y + sq->half;

    float closestX = clampf(cpos->x, left, right);
    float closestY = clampf(cpos->y, top, bottom);

    float dx = cpos->x - closestX;
    float dy = cpos->y - closestY;

    float dist2 = dx*dx + dy*dy;
    float rF = (float)r;

    if (dist2 >= rF*rF) return;

    float dist = sqrtf(dist2);

    float nx = (dist > 0.0f) ? dx / dist : 0.0f;
    float ny = (dist > 0.0f) ? dy / dist : 1.0f;

    float penetration = rF - dist;

    cpos->x += nx * penetration * 0.5f;
    cpos->y += ny * penetration * 0.5f;
    sq->pos.x -= nx * penetration * 0.5f;
    sq->pos.y -= ny * penetration * 0.5f;

    float rvx = cvel->x - sq->vel.x;
    float rvy = cvel->y - sq->vel.y;

    float velAlongNormal = rvx*nx + rvy*ny;
    if (velAlongNormal > 0.0f) return;

    const float restitution = 0.6f;
    float j = -(1.0f + restitution) * velAlongNormal * 0.5f;

    cvel->x += j * nx;
    cvel->y += j * ny;
    sq->vel.x -= j * nx;
    sq->vel.y -= j * ny;
}

void clampKirkleToBounds(Vec2 *pos, Vec2 *vel, int r)
{
    if (pos->x < r) { pos->x = r; if (vel->x < 0.0f) vel->x = -vel->x; }
    if (pos->x > WIDTH - r) { pos->x = WIDTH - r; if (vel->x > 0.0f) vel->x = -vel->x; }
    if (pos->y < r) { pos->y = r; if (vel->y < 0.0f) vel->y = -vel->y; }
    if (pos->y > HEIGHT - r) { pos->y = HEIGHT - r; if (vel->y > 0.0f) vel->y = -vel->y; }
}

void clampSquareToBounds(Square *s)
{
    if (s->pos.x < s->half) { s->pos.x = s->half; if (s->vel.x < 0.0f) s->vel.x = -s->vel.x; }
    if (s->pos.x > WIDTH - s->half) { s->pos.x = WIDTH - s->half; if (s->vel.x > 0.0f) s->vel.x = -s->vel.x; }
    if (s->pos.y < s->half) { s->pos.y = s->half; if (s->vel.y < 0.0f) s->vel.y = -s->vel.y; }
    if (s->pos.y > HEIGHT - s->half) { s->pos.y = HEIGHT - s->half; if (s->vel.y > 0.0f) s->vel.y = -s->vel.y; }
}
