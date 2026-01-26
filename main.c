#include <SDL2/SDL.h>
#include <stdbool.h>

#include "headers/sdl_app.h"
#include "headers/physics.h"
#include "headers/render.h"
#include "headers/input_drag.h"

int main(void)
{
    bool running = true;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (!init(&window, &renderer)) return 1;

    Square sq = {
        .pos = { 300.0f, 300.0f },
        .vel = { 0.0f, 0.0f },
        .acc = { 0.0f, 0.0f },
        .half = 50,
        .R = 255, .G = 255, .B = 255, .A = 255
    };

    Triangle tri = {
        .v = {
            { 120.0f, 140.0f },
            { 60.0f,  240.0f },
            { 200.0f, 240.0f }
        },
        .R = 255, .G = 255, .B = 255, .A = 255
    };

    int radius = 70;

    Vec2 pos = { WIDTH / 2.0f, HEIGHT / 2.0f };
    Vec2 vel = { 0.0f, 0.0f };
    Vec2 acc = { 0.0f, 0.0f };

    DragTarget drag = DRAG_NONE;
    float grabOffX = 0.0f, grabOffY = 0.0f;

    int prevMx = 0, prevMy = 0;
    SDL_GetMouseState(&prevMx, &prevMy);

    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        handleEvents(&running, &drag,
                     &grabOffX, &grabOffY,
                     &pos, &vel, radius,
                     &sq);

        float dt = getDeltaTime(&lastTime);

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        float mdx = (float)(mx - prevMx);
        float mdy = (float)(my - prevMy);
        prevMx = mx;
        prevMy = my;

        float throwVx = (dt > 0.0f) ? mdx / dt : 0.0f;
        float throwVy = (dt > 0.0f) ? mdy / dt : 0.0f;

        if (drag == DRAG_KIRKLE) {
            pos.x = (float)mx - grabOffX;
            pos.y = (float)my - grabOffY;
            vel.x = throwVx;
            vel.y = throwVy;
        } else {
            updatePhysics(&pos, &vel, &acc, radius, dt);
        }

        if (drag == DRAG_SQUARE) {
            sq.pos.x = (float)mx - grabOffX;
            sq.pos.y = (float)my - grabOffY;
            sq.vel.x = throwVx;
            sq.vel.y = throwVy;
        } else {
            updateSquare(&sq, dt);
        }

        resolveKirkleSquareCollision(&pos, &vel, radius, &sq);
        clampKirkleToBounds(&pos, &vel, radius);
        clampSquareToBounds(&sq);

        renderr(renderer, pos, radius, &sq, &tri);
        SDL_Delay(1);
    }

    cleanup(window, renderer);
    return 0;
}
