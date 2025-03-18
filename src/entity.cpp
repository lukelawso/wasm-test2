#include "entity.h"

Entity::Entity(float x, float y, float vel_x, float vel_y, bool map_bound)
    : x(x), y(y), velocity_x(vel_x), velocity_y(vel_y), map_bound(map_bound) {}

void Entity::update_object_pos() {
  // Normalized
  float mag = sqrt(velocity_x * velocity_x + velocity_y * velocity_y);
  x += velocity_x / mag;
  y += velocity_y / mag;

  // Check bounds
  if (map_bound) {
    m_shape->clamp_to_map(x, y);
  }
}

PlayerObject::PlayerObject(float x, float y, float vel_x, float vel_y,
                           SDL_Color color)
    : Entity(x, y, vel_x, vel_y, true), color(color) {
  auto square = std::make_unique<Rect>(50.0f, 50.0f);
  set_shape(std::move(square));
}
