#include "sprite.h"
#include "entity.h"

Sprite::Sprite(std::shared_ptr<Entity> owner) : m_owner(owner) {}

void PlayerSprite::draw(float x, float y, bool draw_from_center) {
  auto &player = *reinterpret_cast<PlayerObject const *>(m_owner.get());
  // only draw from top left for now
  if (!draw_from_center) {
    SDL_SetRenderDrawColor(g_pRenderer, player.color.r, player.color.g,
                           player.color.b, player.color.a);
    auto [x, y] = player.get_pos();
    SDL_Rect player_rect = {static_cast<int>(x), static_cast<int>(y),
                            static_cast<int>(c_player_width),
                            static_cast<int>(c_player_height)};
    SDL_RenderFillRect(g_pRenderer, &player_rect);
  }
}
