#include "sprite.h"
#include "entity.h"

PlayerSprite::PlayerSprite(PlayerObject &player) : m_player(player) {}

void PlayerSprite::draw(float x, float y, bool draw_from_center)
{
  // only draw from top left for now
  if (!draw_from_center)
  {
    SDL_SetRenderDrawColor(g_pRenderer, m_player.color.r, m_player.color.g,
                           m_player.color.b, m_player.color.a);
    auto [x, y] = m_player.get_pos();
    SDL_Rect player_rect = {static_cast<int>(x), static_cast<int>(y),
                            static_cast<int>(c_player_width),
                            static_cast<int>(c_player_height)};
    SDL_RenderFillRect(g_pRenderer, &player_rect);
  }
}
