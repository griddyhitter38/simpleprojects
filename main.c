#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <bits/types/cookie_io_functions_t.h>
#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include "stdbool.h"

#define HEIGHT 1000
#define WIDTH 800

void drawFilledKirkle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        int y = cy + dy;

        double t = (double)radius * radius - (double)dy * dy;
        int dx = (int)sqrt(t);

        int x1 = cx - dx;
        int x2 = cx + dx;

        SDL_RenderDrawLine(renderer, x1, y, x2, y);
    }
}

void drawKirkle(SDL_Renderer *renderer, int cx, int cy, int radius, int mass, float acceleration){
    int segments = 360;
    float gravity = acceleration * mass;

    for (int i = 0; i < segments; i++){
        double theta = i * (2.00 * M_PI / segments);
    int x = cx + (int)(radius * cos(theta));
    int y = cy + (int)(radius * sin(theta));
    SDL_RenderDrawPoint(renderer,x,y);

    }
}

int main(){
    bool running = true;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT,  SDL_WINDOW_SHOWN, &window, &renderer);

    //kirle white
    int r = 255;
    int green = 255;
    int b = 255;
    int a = 255;
    int radius = 70;

    //physics
    float x = WIDTH / 2;
    float y = HEIGHT / 2;
    float vx = 0.0f;
    float vy = 0.0f;

    const float control = 20000.0f;
    const float drag = 0.98f;
    const float g = 10000.0f;

    Uint32 lastTime = SDL_GetTicks();

    //kikr movement
    while (running){
        while (SDL_PollEvent(&event)) {

        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = false;

        }
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (deltaTime > 0.05f)
            deltaTime = 0.05f;

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LEFT]) vx -= control * deltaTime;
        if (keys[SDL_SCANCODE_RIGHT]) vx += control * deltaTime;
        if (keys[SDL_SCANCODE_UP]) vy -= control * deltaTime;
        if (keys[SDL_SCANCODE_DOWN]) vy += control * deltaTime;

    while (vy < HEIGHT - radius){
        vy += g * deltaTime;
    }
        vx *= drag;
        vy *= drag;

        x += vx * deltaTime;
        y += vy * deltaTime;

        // clamp x
        if (x < radius) x = radius;
        if (x > WIDTH - radius) x = WIDTH - radius;

        // clamp y
        if (y < radius) y = radius;
        if (y > HEIGHT - radius) y = HEIGHT - radius;

    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, r,green,b,a);
    drawFilledKirkle(renderer, (int)x, (int)y, radius);
    //drawKirkle(renderer, posx1, posy1, 100, 10, 0.1);

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
