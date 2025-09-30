#define SDL_MAIN_HANDLED
#include <SDL.h>

/*#include <SDL_main.h>
#include "D:/Dev/SDL-release-3.2.22/include/SDL3/SDL.h" */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CELL 20
#define BACKPANELCELL 5
#define COLUMNS 40
#define ROWS 30
#define WIDTHWINDOW ((CELL + BACKPANELCELL)* COLUMNS)
#define WIDTH (CELL * COLUMNS)
#define HEIGHT (CELL * ROWS)
#define LEN_TABLE (COLUMNS * ROWS)

typedef struct { int x, y; } Point;


SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

bool table[ROWS][COLUMNS];
Point mouse_xy;
int game_over = 0;
int score = 0;

void init_game() {
    for (int x = 0; x < COLUMNS; x ++) {
        for (int y = 0; y < ROWS; y ++) {
            table[x][y] = 0;
        }
    }    

}

void render() {
    // clear
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
    SDL_RenderClear(ren);

    //draw SPOT
    for (int x = 0; x < COLUMNS; x ++) {
        for (int y = 0; y < ROWS; y ++) {
            SDL_FRect r = { x * CELL, y * CELL, CELL, CELL };
            if (table[x][y]) SDL_SetRenderDrawColor(ren, 30, 160, 30, 255);
            else SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
            SDL_RenderFillRect(ren, &r);               
        }
    }

    // draw grid (optional)
    SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
    for (int x = 0; x <= WIDTH; x += CELL) SDL_RenderLine(ren, x, 0, x, HEIGHT);
    for (int y = 0; y <= HEIGHT; y += CELL) SDL_RenderLine(ren, 0, y, WIDTH, y);

    SDL_RenderPresent(ren);
}   


void update() {
    // new table

}

int main(int argc, char **argv) {
    bool loopShouldStop = false;
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) == 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    win = SDL_CreateWindow("Cells_Authomaton - SDL3", WIDTHWINDOW, HEIGHT, 0);
    if (!win) { fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError()); SDL_Quit(); return 1; }

    ren = SDL_CreateRenderer(win, NULL);
    if (!ren) { fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError()); SDL_DestroyWindow(win); SDL_Quit(); return 1; }

    init_game();

    Uint32 last = SDL_GetTicks();
    const int tick_ms = 120; // speed

    while (!loopShouldStop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    loopShouldStop = true;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    int x = event.button.x / CELL;
                    int y = event.button.y / CELL;
                    if (table[x][y])
                        table[x][y] = 0;
                    else table[x][y] = 1;
                    break;
            }
        }

        //SDL_RenderClear(ren);
        //SDL_RenderPresent(ren);
        Uint32 now = SDL_GetTicks();
        if (now - last >= (Uint32)tick_ms) {
            //update();
            render();
            last = now;
        }

        SDL_Delay(1);
    }

    // game over message in console
    printf("Game Over! Score: %d\n", score);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

