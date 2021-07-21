#include "game.hpp"
#include <SDL2/SDL_mixer.h>
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

  
//game function when play game mode is on..
void Game::play_game(){
  map->draw();
  coin->draw();
  //enemy_->draw();
  p1->draw(game_mode_);
  p2->draw(game_mode_);
  //draw_score();
  //enemy_->move(debug_lose_enemy_, *map_, *p1_, game_mode_);
  p1->move(*map);
  p2->move(*map);
//   if (p1_->get_power_mode()) {
//     p1_->set_power_mode(p1_->get_power_mode() - 1);
//   }
  // if (p2_->get_power_mode()) {
  //   p2_->set_power_mode(p2_->get_power_mode() - 1);
  // }

//   const bool food_state =
//       food_->check_state(game_mode_, *p1_);
//   const bool hit_enemy = enemy_->check_hit_enemy(game_mode_, *p1_);
//   if (food_state) {
//     game_state_ = game_state::clear;
//   } else if (hit_enemy) {
//     game_state_ = game_state::miss;
//   }

  if (input_manager_->edge_key_p(player_type::p1, input_device::space)) {
    game_state_ = game_state::pause;
  }

}