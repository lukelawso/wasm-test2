#pragma once

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <memory>

class Entity;
class PlayerObject;

extern SDL_Renderer *g_pRenderer;

class Sprite
{
public:
  Sprite() = default;
  virtual ~Sprite() = default;
  virtual void draw(float x, float y, bool draw_from_center = false) = 0;
};

class PlayerSprite : public Sprite
{
  PlayerObject &m_player;
  // for now
  const int c_player_height = 50.0f;
  const int c_player_width = 50.0f;

public:
  PlayerSprite(PlayerObject &player);

  void draw(float x, float y, bool draw_from_center) override;
};
