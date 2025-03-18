#pragma once

#include <SDL2/SDL.h>
#include <memory>

class Entity;

extern SDL_Renderer *g_pRenderer;

class Sprite {
protected:
  std::shared_ptr<Entity> m_owner;

public:
  Sprite(std::shared_ptr<Entity> owner);
  virtual void draw(float x, float y, bool draw_from_center = false) = 0;
};

class PlayerSprite : public Sprite {
  // for now
  const int c_player_height = 50.0f;
  const int c_player_width = 50.0f;

  void draw(float x, float y, bool draw_from_center) override;
};
