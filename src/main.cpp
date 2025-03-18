// game.cpp - Main game file

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "entity.h"
#include "shape.h"
#include "sprite.h"

// Game constants
const int c_screen_width = 800;
const int c_screen_height = 600;
const char *GAME_TITLE = "WebAssembly Game";

// Game variables
SDL_Window *g_pWindow = nullptr;
SDL_Renderer *g_pRenderer = nullptr;
SDL_Event g_event;
const Uint8 *g_keystates = nullptr;
bool g_quit = false;

class Game {
  std::shared_ptr<PlayerObject> m_player;
  std::vector<std::shared_ptr<Entity>> m_entities;

public:
  Game() {
    SDL_Color color = {255, 0, 255};
    m_player = std::make_unique<PlayerObject>(c_screen_width / 2.0f - 25.0f,
                                              c_screen_height / 2.0f - 25.0f,
                                              0.0f, 0.0f, color);
    m_entities.push_back(m_player);
  }

  void run_updates() {
    for (auto &ent : m_entities) {
      ent->update_object_pos();
    }
  }

  void process_input();

  void render();
};

// Initialize the game
bool init_globals() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  // Create window
  g_pWindow = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, c_screen_width,
                               c_screen_height, SDL_WINDOW_SHOWN);

  if (g_pWindow == nullptr) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  // Create renderer
  g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, SDL_RENDERER_ACCELERATED);
  if (g_pRenderer == nullptr) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  g_keystates = SDL_GetKeyboardState(nullptr);

  // Set renderer color
  SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);

  return true;
}

// Process input
void Game::process_input() {
  // Process events
  while (SDL_PollEvent(&g_event)) {
    if (g_event.type == SDL_QUIT) {
      g_quit = true;
    } else if (g_event.type == SDL_KEYDOWN) {
      switch (g_event.key.keysym.sym) {
      case SDLK_ESCAPE:
        g_quit = true;
        break;
      default:
        break;
      }
    }
  }

  float x_vel = 0, y_vel = 0;

  if (g_keystates[SDL_SCANCODE_LEFT] || g_keystates[SDL_SCANCODE_A]) {
    x_vel = -5.0f;
  }
  if (g_keystates[SDL_SCANCODE_RIGHT] || g_keystates[SDL_SCANCODE_D]) {
    x_vel = 5.0f;
  }
  if (g_keystates[SDL_SCANCODE_UP] || g_keystates[SDL_SCANCODE_W]) {
    y_vel = -5.0f;
  }
  if (g_keystates[SDL_SCANCODE_DOWN] || g_keystates[SDL_SCANCODE_S]) {
    y_vel = 5.0f;
  }
  m_player->set_velocity(x_vel, y_vel);
}

// Render game
void Game::render() {
  // Clear screen
  SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
  SDL_RenderClear(g_pRenderer);

  // TODO: render other entities minus player

  m_player->draw();

  // Present renderer
  SDL_RenderPresent(g_pRenderer);
}

// Main game loop
void main_loop(void *pGame) {
  if (g_quit) {
    emscripten_cancel_main_loop();
    return;
  }
  Game &game = *static_cast<Game *>(pGame);

  game.process_input();
  game.run_updates();
  game.render();
}

// Clean up resources
void close() {
  SDL_DestroyRenderer(g_pRenderer);
  SDL_DestroyWindow(g_pWindow);
  SDL_Quit();
}

// Main function
int main(int argc, char *args[]) {
  // Initialize game
  if (!init_globals()) {
    printf("Failed to initialize!\n");
    return -1;
  }

  Game game{};

  // Set main loop
  // 0 fps means using requestAnimationFrame, -1 means synchronous
  emscripten_set_main_loop_arg(main_loop, &game, 0, 1);

  // Clean up (this will not actually be called in Emscripten)
  close();

  return 0;
}
