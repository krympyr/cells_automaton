#define SDL_MAIN_HANDLED
#include <SDL.h>

/*#include <SDL_main.h>
#include "D:/Dev/SDL-release-3.2.22/include/SDL3/SDL.h" */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CELL 10
#define BACKPANELPOINT 100
#define COLUMNS 80
#define ROWS 60

#define WIDTHWINDOW ((CELL* COLUMNS)) + BACKPANELPOINT
#define WIDTH (CELL * COLUMNS)
#define HEIGHT (CELL * ROWS)
#define LEN_TABLE (COLUMNS * ROWS)

typedef struct { int x, y; } Point;

bool START = 0;
SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

bool fronttable[COLUMNS][ROWS] = {0};
bool backtable[COLUMNS][ROWS] = {0};
Point mouse_xy;
int game_over = 0;
int score = 0;

void init_game() {
    fronttable[0][1] = 1;
    fronttable[1][2] = 1;
    fronttable[2][0] = 1;
    fronttable[2][1] = 1;
    fronttable[2][2] = 1;   

}

void render() {
    // clear
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
    SDL_RenderClear(ren);

    //draw FILL RECT
    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS; y++) {
            SDL_FRect r = { x * CELL, y * CELL, CELL, CELL };
            if (fronttable[x][y]) SDL_SetRenderDrawColor(ren, 30, 160, 30, 255);
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

int mod(int a, int b)
{
    return (a%b + b)%b;
}

int count_nbors(int cx, int cy)
{
    int nbors = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (!(dx == 0 && dy == 0)) {
                int x = mod(cx + dx, COLUMNS);
                int y = mod(cy + dy, ROWS);
                if (fronttable[x][y]) nbors += 1;
            }
        }
    }
    return nbors;
}


void update(void) {
    // new table
    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS; y++) {
            int nbors = count_nbors(x, y);
            backtable[x][y] = fronttable[x][y] ? (nbors == 2 || nbors == 3) : nbors == 3;
        }
    }
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
                    if (x <= COLUMNS && y <= ROWS) {
                        if (fronttable[x][y]) fronttable[x][y] = 0;
                        else fronttable[x][y] = 1;
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_SPACE) {
                        if (START) START = 0;
                        else START = 1;
                        printf("START = %d\n", START);
                    }
                    break;
            }
        }

        //SDL_RenderClear(ren);
        //SDL_RenderPresent(ren);
        Uint32 now = SDL_GetTicks();
        if (now - last >= (Uint32)tick_ms) {
            if (START) {
                printf("STARTING_UPDATE\n");
                update();
                memcpy(fronttable, backtable, sizeof(fronttable));
            }
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

