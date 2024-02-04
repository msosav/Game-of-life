#include "screen.h"
#include <array>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void drawPixel(int x, int y, vector<SDL_FPoint>& points, vector<SDL_Color>& colors) {
  points.emplace_back(x, y);
  colors.emplace_back(255, 255, 255, 255);
}

void clearPixels(vector<SDL_FPoint>& points) {
  points.clear();
}

void update(SDL_Renderer* renderer, vector<SDL_FPoint>& points, vector<SDL_Color>& colors) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  for (long unsigned int i = 0; i < points.size(); i++) {
    SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
    SDL_RenderDrawPointF(renderer, points[i].x, points[i].y);
  }

  SDL_RenderPresent(renderer);
}

void input(SDL_bool& done) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        done = SDL_TRUE;
        break;
    }
  }
}

bool isAlive(const int x, const int y, std::array<std::array<int, SCREEN_HEIGHT>, SCREEN_WIDTH>& screen) {
  int alive = 0;

  // Check all 8 neighbors
  if (x > 0 && screen[x - 1][y] == 1) {
    ++alive;
  }
  if (x < SCREEN_WIDTH - 1 && screen[x + 1][y] == 1) {
    ++alive;
  }
  if (y > 0 && screen[x][y - 1] == 1) {
    ++alive;
  }
  if (y < SCREEN_HEIGHT - 1 && screen[x][y + 1] == 1) {
    ++alive;
  }
  if (x > 0 && y > 0 && screen[x - 1][y - 1] == 1) {
    ++alive;
  }
  if (x < SCREEN_WIDTH - 1 && y < SCREEN_HEIGHT - 1 && screen[x + 1][y + 1] == 1) {
    ++alive;
  }
  if (x > 0 && y < SCREEN_HEIGHT - 1 && screen[x - 1][y + 1] == 1) {
    ++alive;
  }
  if (x < SCREEN_WIDTH - 1 && y > 0 && screen[x + 1][y - 1] == 1) {
    ++alive;
  }

  // Apply the rules of the game
  // 1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
  if (screen[x][y] == 1 && alive < 2) {
    return false;
  }
  // 2. Any live cell with two or three live neighbors lives on to the next generation.
  if (screen[x][y] == 1 && (alive == 2 || alive == 3)) {
    return true;
  }
  // 3. Any live cell with more than three live neighbors dies, as if by overpopulation.
  if (screen[x][y] == 1 && alive > 3) {
    return false;
  }
  // 4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
  if (screen[x][y] == 0 && alive == 3) {
    return true;
  }
  return false;
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_bool done;
  SDL_CreateWindowAndRenderer(640 * 4, 480 * 4, SDL_WINDOW_ALLOW_HIGHDPI, &window, &renderer);
  SDL_RenderSetScale(renderer, 4, 4);
  SDL_SetWindowTitle(window, "Game of Life");

  int windowWidth, windowHeight;
  windowWidth = SCREEN_WIDTH;
  windowHeight = SCREEN_HEIGHT;
  SDL_SetWindowSize(window, windowWidth, windowHeight);

  vector<SDL_FPoint> points;
  vector<SDL_Color> colors;

  std::array<std::array<int, SCREEN_HEIGHT>, SCREEN_WIDTH> game;
  std::array<std::array<int, SCREEN_HEIGHT>, SCREEN_WIDTH> swap;

  for (auto& row : game) {
    std:generate(row.begin(), row.end(), []() { return rand() % 10 == 0 ? 1 : 0; });
  }

  while (true) {
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
      for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        swap[x][y] = isAlive(x, y, game) ? 1 : 0;
      }
    }
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
      for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        if (swap[x][y]) {
          drawPixel(x, y, points, colors);
        }
      }
    }

    std::copy(swap.begin(), swap.end(), game.begin());
    update(renderer, points, colors);
    SDL_Delay(20);
    input(done);
    clearPixels(points);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
