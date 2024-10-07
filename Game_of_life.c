#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 20
#define CELL_SIZE 10
#define WINDOW_SIZE (SIZE * CELL_SIZE)
#define DELAY 100

typedef struct {
    int grid[SIZE][SIZE];
} GameOfLife;

void randomize(GameOfLife *game) {
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            game->grid[i][j] = rand() % 2;
        }
    }
}

void next_generation(GameOfLife *game) {
    int new_grid[SIZE][SIZE] = {0};
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int live_neighbors = 0;
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (x == 0 && y == 0) continue;
                    int ni = i + x;
                    int nj = j + y;
                    if (ni >= 0 && ni < SIZE && nj >= 0 && nj < SIZE) {
                        live_neighbors += game->grid[ni][nj];
                    }
                }
            }
            if (game->grid[i][j] == 1 && (live_neighbors == 2 || live_neighbors == 3)) {
                new_grid[i][j] = 1;
            } else if (game->grid[i][j] == 0 && live_neighbors == 3) {
                new_grid[i][j] = 1;
            }
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            game->grid[i][j] = new_grid[i][j];
        }
    }
}

void draw(SDL_Renderer *renderer, GameOfLife *game) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (game->grid[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderFillRect(renderer, &cell);
        }
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    GameOfLife game;
    randomize(&game);

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        next_generation(&game);
        draw(renderer, &game);
        SDL_Delay(DELAY);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}