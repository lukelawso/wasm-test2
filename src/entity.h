#pragma once

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "shape.h"
#include "sprite.h"

class Entity
{
protected:
  float x;
  float y;
  float velocity_x;
  float velocity_y;
  std::unique_ptr<Shape> m_shape;
  std::unique_ptr<Sprite> m_sprite;

  bool map_bound;

public:
  Entity(float x, float y, float vel_x, float vel_y, bool map_bound = false);

  virtual void draw() = 0;

  void set_shape(std::unique_ptr<Shape> shape) { m_shape = std::move(shape); }

  void set_velocity(float x_v, float y_v)
  {
    velocity_x = x_v;
    velocity_y = y_v;
  }

  std::pair<float, float> get_pos() const { return {x, y}; }

  void update_object_pos();
};

class PlayerObject : public Entity
{
  SDL_Color color;

public:
  PlayerObject(float x, float y, float vel_x, float vel_y, SDL_Color color);

  void draw() override { m_sprite->draw(x, y); }

  friend class PlayerSprite;
};
