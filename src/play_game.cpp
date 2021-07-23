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
  enemy->draw();
  score();
  //enemy_->move(debug_lose_enemy_, *map_, *p1_, game_mode_);
  p1->move(*map);
  p2->move(*map);
//   if (p1_->get_power_mode()) {
//     p1_->set_power_mode(p1_->get_power_mode() - 1);
//   }
  // if (p2_->get_power_mode()) {
  //   p2_->set_power_mode(p2_->get_power_mode() - 1);
  // }

const bool coin_state = coin->check_state(game_mode_, *p1, *p2);
//   const bool hit_enemy = enemy_->check_hit_enemy(game_mode_, *p1_);
if(game_mode_ == 0){
  Point p = p1->get_pos();
  if(coin_state || (p.x==19 && p.y==19)) game_state_ = game_state::win;
}
else{
  if(coin_state) game_state_ = game_state::win;
}

//} else if (hit_enemy) {
//     game_state_ = game_state::miss;
//   }

  if (input_manager_->edge_key_p(player_type::p1, input_device::space)) {
    game_state_ = game_state::pause;
  }

}