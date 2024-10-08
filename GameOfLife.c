#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define CELL_SIZE 2
#define WINDOW_SIZE 800
#define DELAY 10
#define MOD 2

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
            if (MOD == 0) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==3);
	            else res->grid[i][j] = (n==3);
	        }
	        else if (MOD == 1) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==3);
	            else res->grid[i][j] = (n==3 || n==6);
	        }
	  		else if (MOD == 2) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==3 || n==4 || n==6 || n==7 || n==8);
	            else res->grid[i][j] = (n==3 || n==6 || n==7 || n==8);
	        }
	        else if (MOD == 3) {
	            if (G->grid[i][j]) res->grid[i][j] = 0;
	            else res->grid[i][j] = (n==2);
	        }
	        else if (MOD == 4) {
	            if (G->grid[i][j]) res->grid[i][j] = 1;
	            else res->grid[i][j] = (n==3);
	        }
	        else if (MOD == 5) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==1 || n==2 || n==5);
	            else res->grid[i][j] = (n==3 || n==6);
	        }
	        else if (MOD == 6) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==4 || n==5);
	            else res->grid[i][j] = (n==3 || n==6 || n==8);
	        }
	        else if (MOD == 7) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==5 || n==6 || n==7 || n==8);
	            else res->grid[i][j] = (n==3 || n==5 || n==6 || n==7 || n==8);
	        }
	        else if (MOD == 8) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==1 || n==3 || n==5 || n==8);
	            else res->grid[i][j] = (n==3 || n==5 || n==7);
	        }
	        else if (MOD == 9) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==1);
	            else res->grid[i][j] = (n==1);
	        }
	        else if (MOD == 10) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==1 || n==3 || n==5 || n==7);
	            else res->grid[i][j] = (n==1 || n==3 || n==5 || n==7);
	        }
	        else if (MOD == 11) {
	            if (G->grid[i][j]) res->grid[i][j] = 0;
	            else res->grid[i][j] = (n==2 || n==3 || n==4);
	        }
	        else if (MOD == 12) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==4 || n==5 || n==6 || n==7 || n==8);
	            else res->grid[i][j] = (n==3);
	        }
	        else if (MOD == 13) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==3 || n==4 || n==5);
	            else res->grid[i][j] = (n==4 || n==5 || n==6 || n==7 || n==8);
	        }
	        else if (MOD == 14) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==1 || n==2 || n==3 || n==4 || n==5);
	            else res->grid[i][j] = (n==3);
	        }
	        else if (MOD == 15) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==5);
	            else res->grid[i][j] = (n==3 || n==4 || n==5);
	        }
	        else if (MOD == 16) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==4 || n==5);
	            else res->grid[i][j] = (n==3 || n==6 || n==8);
	        }
	        else if (MOD == 17) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==3 || n==4 || n==6 || n==7 || n==8);
	            else res->grid[i][j] = (n==0 || n==1 || n==2 || n==3 || n==4 || n==7 || n==8);
	        }
	        else if (MOD == 18) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==3 || n==5 || n==6 || n==7 || n==8);
	            else res->grid[i][j] = (n==4 || n==6 || n==7 || n==8);
	        }
	        else if (MOD == 19) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==3 || n==5 || n==6 || n==7 || n==8);
	            else res->grid[i][j] = (n==3 || n==6 || n==7 || n==8);
	        }
	        else if (MOD == 20) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==3 || n==8);
	            else res->grid[i][j] = (n==3 || n==5 || n==7);
	        }
	        else if (MOD == 21) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==3);
	            else res->grid[i][j] = (n==2);
	        }
	        else if (MOD == 22) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==2 || n==3 || n==5 || n==6 || n==7 || n==8);
	            else res->grid[i][j] = (n==3 || n==7 || n==8);
	        }
	        else if (MOD == 23) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==5 || n==6 || n==7 || n==8);
	            else res->grid[i][j] = (n==4 || n==5 || n==6 || n==7 || n==8);
	        }
	        else if (MOD == 24) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==0 || n==3 || n==4 || n==5 || n==6);
	            else res->grid[i][j] = (n==2);
	        }
	        else if (MOD == 25) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==4 || n==5 || n==6 || n==7 || n==8);
	            else res->grid[i][j] = (n==3 || n==5 || n==6 || n==7 || n==8);
	        }
	        else if (MOD == 26) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==5);
	            else res->grid[i][j] = (n==3 || n==4 || n==5);
	        }
	        else if (MOD == 27) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==0 || n==2 || n==3);
	            else res->grid[i][j] = (n==3);
	        }
	        else if (MOD == 28) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==1 || n==2);
	            else res->grid[i][j] = (n==3);
	        }
	        else if (MOD == 29) {
	            if (G->grid[i][j]) res->grid[i][j] = (n==1 || n==2 || n==3 || n==4 || n==5);
	            else res->grid[i][j] = (n==3);
	        }
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
    // G->grid[0][0] = 1;
    // G->grid[0][1] = 1;
    // G->grid[0][2] = 1;
    // G->grid[1][1] = 1;
    // G->grid[2][2] = 1;
    
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

