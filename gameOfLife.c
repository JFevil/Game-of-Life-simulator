#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>

#define CELL_SIZE 2
#define SIZE_Y 400
#define SIZE_X 400
#define DELAY 10
#define CIRCULAR_GRID 0

typedef struct GameOfLife {
    int sizeY;
    int sizeX;
    int **grid;
} GameOfLife;

char* rules[] = {
    "B3/S23",
    "B36/S23",
    "B3678/S34678",
    "B2/S",
    "B3/S012345678",
    "B36/S125",
    "B368/S245",
    "B35678/S5678",
    "B357/S1358",
    "B1/S1",
    "B1357/S1357",
    "B234/S",
    "B3/S45678",
    "B45678/S2345",
    "B3/S12345",
    "B345/S5",
    "B368/S245",
    "B0123478/S34678",
    "B4678/S35678",
    "B3678/S235678",
    "B357/S238",
    "B2/S23",
    "B378/S235678",
    "B45678/S5678",
    "B0/S012345678",
    "B2/S03456",
    "B35678/S45678",
    "B345/S5",
    "B3/S023",
    "B3/S12",
    "B3/S12345"
};

char* names[] = {
    "Life",                 // 0
    "HighLife",             // 1
    "Day & Night",          // 2
    "Seeds",                // 3
    "Life without Death",   // 4
    "2x2",                  // 5
    "Morley",               // 6
    "Diamoeba",             // 7
    "Amoeba",               // 8
    "Gnarl",                // 9
    "Replicator",           // 10
    "Serviettes",           // 11
    "Coral",                // 12
    "Walled Cities",        // 13
    "Maze",                 // 14
    "Long Life",            // 15
    "Move",                 // 16
    "InverseLife",          // 17
    "Anneal",               // 18
    "Stains",               // 19
    "Pseudo Life",          // 20
    "Just Friends",         // 21
    "Coagulations",         // 22
    "Vote for Life",        // 23
    "Life without Life",    // 24
    "Faders",               // 25
    "Slow Blob",            // 26
    "Long Long Life",       // 27
    "DotLife",              // 28
    "Flock",                // 29
    "Maze with Mice"        // 30
};

int in(int state,  char* rule, int nbNeighbors) {
	int sep = 1;
	if (!state) {
		while (rule[sep] != '/') {
			if (nbNeighbors == rule[sep]-48) return 1;
            sep++;
		}
	}
	else {
        while (rule[sep] != '/') sep++;
		while (rule[sep] != '\0') {
			if (nbNeighbors == rule[sep]-48) return 1;
            sep++;
		}
	}
	return 0;
}

GameOfLife* newGame(int sizeX, int sizeY) {
    GameOfLife* res = malloc(sizeof(GameOfLife));
    res->sizeX = sizeX;
    res->sizeY = sizeY;
    res->grid = (int**) malloc(sizeY * sizeof(int*));
    for (int i = 0; i < sizeY; i++) {
        res->grid[i] = malloc(sizeX * sizeof(int));
        for (int j = 0; j < sizeX; j++) {
            res->grid[i][j] = 0;
        }
    }
    return res;
}

void randomize(GameOfLife* G) {
    srand(time(NULL));
    for (int i = 0; i < G->sizeY; i++) {
        for (int j = 0; j < G->sizeX; j++) {
            G->grid[i][j] = rand() % 2;
        }
    }
}

void freeGame(GameOfLife* G) {
    for (int i = 0; i < G->sizeY; i++) {
        free(G->grid[i]);
    }
    free(G->grid);
    free(G);
}

int neighbors(GameOfLife* G, int i, int j, int circularGrid) {
    int res = -G->grid[i][j];
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            if (circularGrid) {
                int i2 = (i + y + G->sizeY) % G->sizeY; // Assurez-vous que les indices sont positifs
                int j2 = (j + x + G->sizeX) % G->sizeX;
                res += G->grid[i2][j2];
            }
            else if (i + y >= 0 && i + y < G->sizeY && j + x >= 0 && j + x < G->sizeX) {
                res += G->grid[i + y][j + x];
            }
        }
    }
    return res;
}

void nextGen(GameOfLife* G, int mod, int circularGrid) {
    GameOfLife* res = newGame(G->sizeX, G->sizeY);
    for (int i = 0; i < G->sizeY; i++) {
        for (int j = 0; j < G->sizeX; j++) {
            int n = neighbors(G, i, j, circularGrid);
	        res->grid[i][j] = in(G->grid[i][j], rules[mod], n);
        }
    }
    for (int i = 0; i < G->sizeY; i++) {
        for (int j = 0; j < G->sizeX; j++) {
            G->grid[i][j] = res->grid[i][j];
        }
    }
    freeGame(res);
}

void draw(SDL_Renderer *renderer, GameOfLife *G, int grille, int cellSize) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < G->sizeY; i++) {
        for (int j = 0; j < G->sizeX; j++) {
            if (G->grid[i][j]) {
                SDL_Rect rect = {j * cellSize, i * cellSize, cellSize, cellSize};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    if (grille) {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        for (int i = 0; i <= G->sizeY; i++) SDL_RenderDrawLine(renderer, 0, i * cellSize, G->sizeX * cellSize, i * cellSize);
        for (int j = 0; j <= G->sizeX; j++) SDL_RenderDrawLine(renderer, j * cellSize, 0, j * cellSize, G->sizeY * cellSize);
    }

    SDL_RenderPresent(renderer);
}

int completeDemo(int mod, int showGrid) {
    SDL_Window *window = SDL_CreateWindow("Fenêtre SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SIZE_X * CELL_SIZE, SIZE_Y * CELL_SIZE, SDL_WINDOW_SHOWN);
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

    GameOfLife* G = newGame(SIZE_X, SIZE_Y);
    randomize(G);
    printf("%s (%s)\n", names[mod], rules[mod]);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        draw(renderer, G, showGrid, CELL_SIZE);
        nextGen(G, mod, CIRCULAR_GRID);
        SDL_Delay(DELAY);
    }

    freeGame(G);
    return 0;
}

void square(GameOfLife* G, int i, int j) {
    int pattern[4][2] = {{i, j}, {i, j+1}, {i+1, j}, {i+1, j+1}};
    for (int k = 0; k < 4; k++) G->grid[pattern[k][0]][pattern[k][1]] = 1;
}

void beacon(GameOfLife* G, int i, int j) {
    int pattern[8][2] = {
        {i, j}, {i, j+1}, {i+1, j}, {i+1, j+1},
        {i+2, j+2}, {i+2, j+3}, {i+3, j+2}, {i+3, j+3}
    };
    for (int k = 0; k < 8; k++) G->grid[pattern[k][0]][pattern[k][1]] = 1;
}

void toad(GameOfLife* G, int i, int j) {
    int pattern[6][2] = {
        {i, j+1}, {i, j+2}, {i, j+3},
        {i+1, j}, {i+1, j+1}, {i+1, j+2}
    };
    for (int k = 0; k < 6; k++) G->grid[pattern[k][0]][pattern[k][1]] = 1;
}

void boat(GameOfLife* G, int i, int j) {
    int pattern[5][2] = {{i, j}, {i, j+1}, {i+1, j}, {i+2, j+1}, {i+1, j+2}};
    for (int k = 0; k < 5; k++) G->grid[pattern[k][0]][pattern[k][1]] = 1;
}

void loaf(GameOfLife* G, int i, int j) {
    int pattern[7][2] = {{i, j}, {i, j+1}, {i+1, j}, {i+2, j+1}, {i+1, j+3}, {i+2, j+2}, {i+3, j+1}};
    for (int k = 0; k < 7; k++) G->grid[pattern[k][0]][pattern[k][1]] = 1;
}

void pulsar(GameOfLife* G, int i, int j) {
    int pattern[48][2] = {
        {0, 2}, {0, 3}, {0, 4}, {0, 8}, {0, 9}, {0, 10},
        {2, 0}, {2, 5}, {2, 7}, {2, 12},
        {3, 0}, {3, 5}, {3, 7}, {3, 12},
        {4, 0}, {4, 5}, {4, 7}, {4, 12},
        {5, 2}, {5, 3}, {5, 4}, {5, 8}, {5, 9}, {5, 10},
        {7, 2}, {7, 3}, {7, 4}, {7, 8}, {7, 9}, {7, 10},
        {8, 0}, {8, 5}, {8, 7}, {8, 12},
        {9, 0}, {9, 5}, {9, 7}, {9, 12},
        {10, 0}, {10, 5}, {10, 7}, {10, 12},
        {12, 2}, {12, 3}, {12, 4}, {12, 8}, {12, 9}, {12, 10}
    };

    for (int k = 0; k < 48; k++) G->grid[i + pattern[k][0]][j + pattern[k][1]] = 1;
}

void glider(GameOfLife* G, int i, int j) {
    int pattern[5][2] = {{0, 1},{1, 2},{2, 0},{2, 1},{2, 2}};
    for (int k = 0; k < 5; k++) G->grid[i + pattern[k][0]][j + pattern[k][1]] = 1;
}

void lightweightSpaceship(GameOfLife* G, int i, int j) {
    int pattern[9][2] = {{0, 1},{0, 2},{0, 3},{0, 4},{1, 0},{1, 4},{2, 4},{3, 0},{3, 3}};
    for (int k = 0; k < 9; k++) G->grid[i + pattern[k][0]][j + pattern[k][1]] = 1;
}

void gliderGun(GameOfLife* G, int i, int j) {
    int pattern[36][2] = {
        {5, 1}, {5, 2}, {6, 1}, {6, 2},
        {5, 11}, {6, 11}, {7, 11}, {4, 12}, {8, 12}, {3, 13}, {9, 13}, {3, 14}, {9, 14}, {6, 15}, {4, 16}, {8, 16}, {5, 17}, {6, 17}, {7, 17}, {6, 18},
        {3, 21}, {4, 21}, {5, 21}, {3, 22}, {4, 22}, {5, 22}, {2, 23}, {6, 23}, {1, 25}, {2, 25}, {6, 25}, {7, 25},
        {3, 35}, {4, 35}, {3, 36}, {4, 36}
    };

    for (int k = 0; k < 36; k++) G->grid[i + pattern[k][0]][j + pattern[k][1]] = 1;
}

void cordership(GameOfLife* G, int i, int j) {
    int pattern[56][2] = {
        {i, j+2}, {i, j+3}, {i+1, j+1}, {i+1, j+3}, {i+2, j}, {i+2, j+1}, {i+2, j+3}, 
        {i+3, j}, {i+3, j+1}, {i+3, j+2}, {i+4, j+3}, {i+4, j+4}, {i+5, j+4}, {i+6, j+4},
        {i+6, j+5}, {i+6, j+6}, {i+5, j+6}, {i+4, j+7}, {i+3, j+7}, {i+2, j+7},
        {i+1, j+8}, {i, j+9}, {i+1, j+10}, {i+1, j+11}, {i, j+11}, {i-1, j+11}, 
        {i-2, j+10}, {i-3, j+9}, {i-2, j+8}, {i-2, j+7}, {i-2, j+6}, {i-1, j+6}, 
        {i, j+6}, {i+1, j+5}, {i+1, j+4}, {i, j+4}, {i-1, j+4}, {i-2, j+4}, {i-3, j+5},
        {i-4, j+5}, {i-4, j+4}, {i-4, j+3}, {i-3, j+2}, {i-3, j+1}, {i-2, j}, 
        {i-2, j-1}, {i-1, j-2}, {i, j-3}, {i+1, j-2}, {i+2, j-1}, {i+2, j}, 
        {i+1, j+1}, {i, j+2}, {i+1, j+2}, {i, j+3}
    };
    for (int k = 0; k < 56; k++) G->grid[pattern[k][0]][pattern[k][1]] = 1;
}

int image(int showGrid) {
    int sizeX = 500;
    int sizeY = 500;
    int cellSize = 2;
    int step = 1;
    int infinite = 1;
    SDL_Window *window = SDL_CreateWindow("Fenêtre SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sizeX * cellSize+1, sizeY * cellSize+1, SDL_WINDOW_SHOWN);
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

    GameOfLife* G = newGame(sizeX, sizeY);
    cordership(G, 100, 100);

    int start = 0;
    int running = 0;
    SDL_Event event;
    while (!start) {
        while (SDL_PollEvent(&event)) {
            // Appui sur une espace
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                start = 1;
                running = 1;
            }
            if (event.type == SDL_QUIT) {
                start = 1;
                running = 0;
            }
        }
    }
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        if (infinite || step > 0) {
            draw(renderer, G, showGrid, cellSize);
            nextGen(G, 0, CIRCULAR_GRID);
            SDL_Delay(DELAY);
            step--;
        }
    }

    freeGame(G);
    return 0;
}

int main(int argc, char *argv[]) {
    int mod;
    if (argc != 2) {
        printf("Usage: %s <rule>\n", argv[0]);
        return 1;
    } else {
        char *endptr;
        mod = strtol(argv[1], &endptr, 10);
        if (*endptr != '\0' || mod < -1 || mod > 30) {
            printf("Rule must be between -1 and 30\n");
            return 1;
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }

    int x = 0;
    
    if (mod == -1) x = image(1);
    else x = completeDemo(mod, 0);
    
    SDL_Quit();

    return x;
}
