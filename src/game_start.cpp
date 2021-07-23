#include "game.hpp"
#include <time.h>
#include <sstream>
#include "global.hpp"
#include "enemy.hpp"
//#include <SDL2/SDL_mixer.h>
//#include "mixer_manager.hpp"
#include "font.hpp"
#include "map.hpp"
#include "player.hpp"
#include "input.hpp"


void Game::game_start(){
  map->draw();
  coin->draw();
  //enemy_->draw();
  p1->draw(game_mode_);
  p2->draw(game_mode_);
  enemy->draw();
  score();
  //translucence();
  
  switch (game_count_) {
    case 0: {
      if (p1->get_life() == 2  && p2->get_life() == 2) {
        //Mix_PlayChannel(se_type::beginning,mixer_manager_->get_se(se_type::beginning),0);
      }
      ++game_count_;
      break;
    }
    default:
      ++game_count_;
      break;
  }
  if (game_count_ < 160){
      //text(font_size::x96, rgb::op2, Point{250,240}, "LET'S GO !!!");
      if (blink_count_ < 40) {
        //text(font_size::x64, rgb::op2, Point{220,360}, "press enter to play");
        text(font_size::x96, rgb::op2, Point{250,240}, "LET'S GO !!!");
        ++blink_count_;
      } else if (blink_count_ < 60) {
        ++blink_count_;
      } else {
        blink_count_ = 0;
      }
  }
  else if (game_count_ > 180) {
    game_count_ = 0;
    game_state_ = game_state::playing;
    //p1->set_power_mode(0);
    //p2->set_power_mode(0);
  }
  if (input_manager_->edge_key_p(player_type::p1, input_device::enter)) {
    game_state_ = game_state::playing;
    game_count_ = 0;
  }

}