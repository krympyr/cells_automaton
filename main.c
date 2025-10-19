#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

//#include <SDL_main.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "./character.c"

#define CELL 10
#define DOWNPANELPOINT 50
#define HEIGHTWINDOW 600
#define WIDTHWINDOW 800
#define COLUMNS (WIDTHWINDOW / CELL)
#define ROWS (HEIGHTWINDOW - DOWNPANELPOINT) / CELL
#define HEIGHT (CELL * ROWS)
#define WIDTH (CELL * COLUMNS)
#define LEN_TABLE (COLUMNS * ROWS)



bool START = 0;
SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;
TTF_Font *font = NULL;
TTF_Text *textObj = NULL;

bool fronttable[COLUMNS][ROWS] = {0};
bool backtable[COLUMNS][ROWS] = {0};

int game_over = 0;
int score = 0;

int init_game() {

    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) == 0) {
        printf("SDL_Init error: %s", SDL_GetError());
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == 0) {
        printf("TTF init error: %s", SDL_GetError());
        fprintf(stderr,"TTF init error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    win = SDL_CreateWindow("Cells_Authomaton - SDL3", WIDTHWINDOW, HEIGHTWINDOW, 0);
    if (!win) { fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError()); SDL_Quit(); return 1; }

    ren = SDL_CreateRenderer(win, NULL);
    if (!ren) { fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError()); SDL_DestroyWindow(win); SDL_Quit(); return 1; }

    font = TTF_OpenFont("./fonts/arial.ttf", 24);
    if (!font) {
        fprintf(stderr,"Could not open font: %s", SDL_GetError());
        // очистка и выход...
    }
    textObj = TTF_CreateText(NULL, font, "SPACE is RUN/STOP",5);
    if (!textObj) {
        printf("Could not create text: %s", SDL_GetError());
        fprintf(stderr,"Could not create text: %s", SDL_GetError());
        // очистка и выход...
    }

    fronttable[0][1] = 1;
    fronttable[1][2] = 1;
    fronttable[2][0] = 1;
    fronttable[2][1] = 1;
    fronttable[2][2] = 1;

    return 0;
}

void render(void) {
    // clear
    // SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
    SDL_RenderClear(ren);

    //draw FILL RECT
    SDL_FRect r = {0, HEIGHT, WIDTHWINDOW, HEIGHTWINDOW};
    SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
    SDL_RenderFillRect(ren, &r);
    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS; y++) {
            SDL_FRect r = { x * CELL, y * CELL, CELL, CELL };
            if (fronttable[x][y]) SDL_SetRenderDrawColor(ren, 30, 160, 30, 255);
            else SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
            SDL_RenderFillRect(ren, &r);
        }
    }
    Symbol sym;
    sym = CHARACTERS[21];
    SDL_SetRenderDrawColor(ren, 30, 160, 30, 255);
    int rc = 2;
    for (size_t y = 0; y < CHARHIGHT ; ++y){
        for (size_t x = 0; x < CHARWIDTH; ++x){
            if (sym.SYM[y][x]) {
                //printf("sym.SYM\n");
                SDL_FRect r = { (0 + 10 + x * rc), (HEIGHTWINDOW - 40 + y*rc), rc, rc }; 
                //SDL_RenderPoint(ren, (WIDTHWINDOW + 10 + x), (HEIGHTWINDOW - 40 + y));
                SDL_RenderFillRect(ren, &r);
            }
        }
    }
    // draw grid (optional)
    SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
    for (int x = 0; x <= WIDTH; x += CELL) SDL_RenderLine(ren, x, 0, x, HEIGHT);
    for (int y = 0; y <= HEIGHT; y += CELL) SDL_RenderLine(ren, 0, y, WIDTH, y);

    //================Text

    // if (!TTF_DrawRendererText(textObj, 50, 50)){
    //     printf("Could not DrawRenderer text: %s", SDL_GetError());
    //     fprintf(stderr,"Could not DrawRenderer text: %s", SDL_GetError());
    // }
// //============================= 
    
    if (!SDL_RenderPresent(ren)){
        fprintf(stderr,"SDL_RenderPresent error: %s", SDL_GetError());
    }
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
    TTF_DestroyText(textObj);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

