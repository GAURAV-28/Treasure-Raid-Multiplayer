#include "game.hpp"
#include <time.h>
#include <sstream>
#include "global.hpp"
//#include "enemy.hpp"
//#include <SDL2/SDL_mixer.h>
//#include "mixer_manager.hpp"
#include "font.hpp"
#include "map.hpp"
#include "player.hpp"
#include "input.hpp"
#include "image.hpp"


void Game::game_start(){
  map->draw();
  coin->draw();
  //enemy_->draw();
  p1->draw(game_mode_);
  p2->draw(game_mode_);
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
      //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      //SDL_RenderClear(renderer);

      if (blink_count_ < 40) {
        SDL_Texture *trans_texture = image_manager_->get(image::up);
        SDL_Rect dst = {295, 360, 80, 80};
        SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
        trans_texture = image_manager_->get(image::down);
        dst = {380, 360, 80, 80};
        SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
        trans_texture = image_manager_->get(image::left);
        dst = {465, 360, 80, 80};
        SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
        trans_texture = image_manager_->get(image::right);
        dst = {550, 360, 80, 80};
        SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
        SDL_DestroyTexture(trans_texture);
        text(font_size::x64, rgb::white, Point{220,360}, "A : ");
        text(font_size::x96, rgb::white, Point{250,240}, "LET'S GO !!!");

        if(game_mode_==1){
          SDL_Texture *trans_texture = image_manager_->get(image::w);
          SDL_Rect dst = {295, 480, 80, 80};
          SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
          trans_texture = image_manager_->get(image::s);
          dst = {380, 480, 80, 80};
          SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
          trans_texture = image_manager_->get(image::a);
          dst = {465, 480, 80, 80};
          SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
          trans_texture = image_manager_->get(image::d);
          dst = {550, 480, 80, 80};
          SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
          SDL_DestroyTexture(trans_texture);
          text(font_size::x64, rgb::white, Point{220,480}, "B : ");
        }

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