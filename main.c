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
#include "stdbool.h"

#define HEIGHT 800
#define WIDTH 600

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

void drawKirkle(SDL_Renderer *renderer, int cx, int cy, int radius){
    int segments = 360;

    for (int i = 0; i < segments; i++){
        double theta = i * (2.00 * M_PI / segments);
    int x = cx + (int)(radius * cos(theta));
    int y = cy + (int)(radius * sin(theta));
    SDL_RenderDrawLine(renderer,cx,cy,x,y);
    }
}

int main(){
    bool running = true;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT,  SDL_WINDOW_SHOWN, &window, &renderer);

    //kirle
    int posx1 = WIDTH / 2;
    int posy1 = HEIGHT / 2;
    int radius = 100;

    //kikr movement
    while (running){
        while (SDL_PollEvent(&event)) {

        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = false;

        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        int speed = 10;

        if (keys[SDL_SCANCODE_UP]) posy1 -= 10;
        if (keys[SDL_SCANCODE_DOWN]) posy1 += 10;
        if (keys[SDL_SCANCODE_LEFT]) posx1 -= 10;
        if (keys[SDL_SCANCODE_RIGHT]) posx1 += 10;

        // clamp x
        if (posx1 < radius)
            posx1 = radius;

        if (posx1 > WIDTH - radius)
            posx1 = WIDTH - radius;

        // clamp y
        if (posy1 < radius)
            posy1 = radius;

        if (posy1 > HEIGHT - radius)
            posy1 = HEIGHT - radius;



    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    drawFilledKirkle(renderer, posx1, posy1, 100);

    SDL_RenderPresent(renderer);
    SDL_Delay(100);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
