#include "game.hpp"
//#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <sstream>
#include "global.hpp"
//#include "enemy.hpp"
#include "font.hpp"
#include "coins.hpp"
#include "image.hpp"
#include "input.hpp"
#include "image.hpp"
#include "map.hpp"
//#include "mixer_manager.hpp"
#include "player.hpp"


void Game::game_client(){
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  const Point title_pos = Point{200, 100};
  const Point p1_mode_pos = Point{185, 300};
  const Point vs_mode_pos = Point{180, 420};
  const char *title_str = "Treasure Raid";
  const char *p1_mode_str = "game is loading";
  const char *vs_mode_str = "DOUBLE PLAYER MODE";
  const SDL_Rect dst = {0, 0, screen::width, screen::height};
  const SDL_Rect p1_str_dst = {350, 290, 155, 80};
  const SDL_Rect p2_str_dst = {350, 410, 155, 80};

  SDL_Texture *trans_texture = image_manager_->get(image::fullbg);
  SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
  text(font_size::x96, rgb::tc, title_pos, title_str);
  text(font_size::x96, rgb::tc, vs_mode_pos, p1_mode_str);
  SDL_DestroyTexture(trans_texture);
}