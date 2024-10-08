#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define CELL_SIZE 5
#define WINDOW_SIZE 800
#define DELAY 50

typedef struct GameOfLife {
    int size;
    int **grid;
} GameOfLife;

GameOfLife* newGame(int size) {
    GameOfLife* res = malloc(sizeof(GameOfLife));
    res->size = size;
    res->grid = (int**) malloc(size*sizeof(int*));
    for (int i = 0; i<size; i++) {
        res->grid[i] = malloc(size*sizeof(int));
        for (int j = 0; j<size; j++) {
            res->grid[i][j] = 0;
        }
    }
    return res;
}

void randomize(GameOfLife* G) {
    srand(time(NULL));
    for (int i=0; i<G->size; i++) for (int j=0; j<G->size; j++) G->grid[i][j] = rand()%2;
}

void freeGame(GameOfLife* G) {
    for (int i=0; i<G->size; i++) free(G->grid[i]);
    free(G->grid);
    free(G);
}

int neighbors(GameOfLife* G, int i, int j) {
    int res = -G->grid[i][j];
    for (int y = -1; y < 2; y++) {
        for (int x = -1; x < 2; x++) {
            if (i+y>=0 && i+y<G->size && j+x>=0 && j+x<G->size) res += G->grid[i+y][j+x];
        }
    }
    return res;
}

void nextGen(GameOfLife* G) {
    GameOfLife* res = newGame(G->size);
    for (int i = 0; i<G->size; i++) {
        for (int j = 0; j<G->size; j++) {
            int n = neighbors(G, i, j);
            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==3);
            else res->grid[i][j] = (n==3);
        }
    }
    for (int i = 0; i<G->size; i++) {
		for (int j = 0; j<G->size; j++) {
			G->grid[i][j] = res->grid[i][j];
		}
	}
	freeGame(res);
}

void display(GameOfLife* G) {
    for (int i = 0; i<G->size; i++) {
        for (int j = 0; j<G->size; j++) {
            if (G->grid[i][j]) printf("# ");
            else printf(". ");
        }
        printf("\n");
    }
    printf("\n");
}

void draw(SDL_Renderer* renderer, GameOfLife* G) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i<G->size; i++) {
        for (int j = 0; j<G->size; j++) {
            if (G->grid[i][j]) {
                SDL_Rect rect = {j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Fenêtre SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    GameOfLife* G = newGame(WINDOW_SIZE/CELL_SIZE);
    randomize(G);
    
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        draw(renderer, G);
        nextGen(G);
        SDL_Delay(DELAY);
    }

    freeGame(G);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

