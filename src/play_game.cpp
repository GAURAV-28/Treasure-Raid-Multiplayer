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
#include "sound_manager.hpp"
#include "player.hpp"

  
//game function when play game mode is on..
void Game::play_game(){
  
  map->draw();
  coin->draw();
  //enemy_->draw();
  //enemy->check_hit_enemy(game_mode_, *p1, *p2);
  p1->draw(game_mode_);
  p2->draw(game_mode_);
  enemy->draw();
  score();
  p1->move(*map);
  p2->move(*map);
  bool hit_enemy = false;
  if (!p1->get_power_mode()){
    enemy->move(*map, *p1, *p2);
    hit_enemy = enemy->check_hit_enemy(game_mode_, *p1, *p2);
  }
  


  if (p1->get_power_mode()) {
    p1->set_power_mode(p1->get_power_mode() - 1);
  }
  if (p2->get_power_mode()) {
    p2->set_power_mode(p2->get_power_mode() - 1);
  }

const bool coin_state = coin->check_state(game_mode_, *p1, *p2);
//const bool 
if(game_mode_ == 0){
  Point p = p1->get_curr();
  if((p.x==19 && p.y==19)) game_state_ = game_state::win;
}
else{
  Point pa = p1->get_curr();
  Point pb = p2->get_curr();
  if(pa.x==19 && pa.y == 19 && pb.x == 19 && pb.y == 19) game_state_ = game_state::win;
}

if (hit_enemy) {
    game_state_ = game_state::miss;
}

  if (input_manager_->edge_key_p(player_type::p1, input_device::space)) {
    game_state_ = game_state::pause;
  }

}