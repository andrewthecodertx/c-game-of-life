#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define CELL_SIZE 5
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool grid[GRID_HEIGHT][GRID_WIDTH];
  bool running;
  bool paused;
} GameState;

void nextGeneration(GameState *game) {
  bool newGrid[GRID_HEIGHT][GRID_WIDTH] = {false};

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      int neighbors = 0;

      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          if (i == 0 && j == 0)
            continue;

          int newX = (x + i + GRID_WIDTH) % GRID_WIDTH;
          int newY = (y + j + GRID_HEIGHT) % GRID_HEIGHT;

          if (game->grid[newY][newX]) {
            neighbors++;
          }
        }
      }

      // apply game rules
      if (game->grid[y][x]) {
        // live cell
        newGrid[y][x] = (neighbors == 2 || neighbors == 3);
      } else {
        // dead cell
        newGrid[y][x] = (neighbors == 3);
      }
    }
  }

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      game->grid[y][x] = newGrid[y][x];
    }
  }
}

bool init(GameState *game) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL initialization failed: %s\n", SDL_GetError());
    return false;
  }

  game->window = SDL_CreateWindow(
      "Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (!game->window) {
    printf("Window creation failed: %s\n", SDL_GetError());
    return false;
  }

  game->renderer =
      SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
  if (!game->renderer) {
    printf("Renderer creation failed: %s\n", SDL_GetError());
    return false;
  }

  game->running = true;
  game->paused = true;

  srand(time(NULL));
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      game->grid[y][x] = (rand() % 100) < 45; // 45% chance of live cell
    }
  }

  return true;
}

void handleEvents(GameState *game) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      game->running = false;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_q:
        game->running = false;
        break;
      case SDLK_SPACE:
        game->paused = !game->paused;
        break;
      case SDLK_r: // Reset with random state
        for (int y = 0; y < GRID_HEIGHT; y++) {
          for (int x = 0; x < GRID_WIDTH; x++) {
            game->grid[y][x] = (rand() % 100) < 45;
          }
        }
        break;
      case SDLK_c: // Clear grid
        for (int y = 0; y < GRID_HEIGHT; y++) {
          for (int x = 0; x < GRID_WIDTH; x++) {
            game->grid[y][x] = false;
          }
        }
        break;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x / CELL_SIZE;
        int y = event.button.y / CELL_SIZE;
        if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
          game->grid[y][x] = !game->grid[y][x];
        }
      }
      break;
    }
  }
}

void render(GameState *game) {
  SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
  SDL_RenderClear(game->renderer);

  SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      if (game->grid[y][x]) {
        SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE - 1,
                         CELL_SIZE - 1};
        SDL_RenderFillRect(game->renderer, &cell);
      }
    }
  }

  SDL_RenderPresent(game->renderer);
}

void cleanup(GameState *game) {
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  SDL_Quit();
}

int main(int argc, char *argv[]) {
  GameState game;

  if (!init(&game)) {
    return 1;
  }

  while (game.running) {
    handleEvents(&game);
    if (!game.paused) {
      nextGeneration(&game);
    }
    render(&game);
    SDL_Delay(100);
  }

  cleanup(&game);
  return 0;
}
