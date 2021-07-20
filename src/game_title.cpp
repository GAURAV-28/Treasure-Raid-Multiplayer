#include "game.hpp"
//#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <sstream>
#include "global.hpp"
//#include "enemy.hpp"
#include "font.hpp"
//#include "food.hpp"
#include "image.hpp"
#include "input.hpp"
#include "image.hpp"
#include "map.hpp"
//#include "mixer_manager.hpp"
#include "player.hpp"
//#include "wipe.hpp"

void Game::game_title(){
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  const Point title_pos = Point{200, 100};
  const Point p1_mode_pos = Point{180, 300};
  const Point vs_mode_pos = Point{180, 420};
  const char *title_str = "Treasure Raid";
  const char *p1_mode_str = "SINGLE PLAYER MODE";
  const char *vs_mode_str = "DOUBLE PLAYER MODE";
  const SDL_Rect dst = {0, 0, screen::width, screen::height};
  const SDL_Rect p1_str_dst = {250, 298, 112, 26};
  const SDL_Rect vs_str_dst = {250, 348, 112, 26};

  SDL_Texture *trans_texture = image_manager_->get(image::fullbg);
  SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
  
  switch (game_count_) {
    
    case 0: {
      text(font_size::x96, rgb::tc, title_pos, title_str);
      if (blink_count_ < 30) {
        text(font_size::x48, rgb::tc, Point{240,540}, "press enter to continue");
        ++blink_count_;
      } else if (blink_count_ < 60) {
        ++blink_count_;
      } else {
        blink_count_ = 0;
      }

      switch (menu_option_){

        case 0:{
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          //SDL_RenderFillRect(renderer, &p1_str_dst);
          text(font_size::x64, rgb::op2, Point{360,300}, "START");
          text(font_size::x64, rgb::blink, Point{370,420}, "HELP");
          
          break;
        }

        case 1:{
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          //SDL_RenderFillRect(renderer, &p1_str_dst);
          text(font_size::x64, rgb::blink, Point{360,300}, "START");
          text(font_size::x64, rgb::op2, Point{370,420}, "HELP");
          
          break;
        }

        default:
          break;
      }

      //move to next or prev option
      if (input_manager_->press_key_p(player_type::p1, input_device::down)
      || input_manager_->press_key_p(player_type::p2, input_device::down)) {
          menu_option_=1;
      } else if (input_manager_->press_key_p(player_type::p1, input_device::up)
      || input_manager_->press_key_p(player_type::p2, input_device::up)) {
          menu_option_=0;
      }

      //move to next state
      if (input_manager_->edge_key_p(player_type::p1, input_device::x)
      || input_manager_->press_key_p(player_type::p2, input_device::x)
      || input_manager_->edge_key_p(player_type::p1, input_device::enter)) {
        ++game_count_;
        blink_count_ = 0;
      }

      break;
    }
    case 1: {
      text(font_size::x96, rgb::tc, title_pos, title_str);
      if (!input_manager_->press_key_p(player_type::p1, input_device::x)
          && !input_manager_->press_key_p(player_type::p2, input_device::x)
          && !input_manager_->press_key_p(player_type::p1, input_device::enter)) {
        ++game_count_;
      }
      break;
    }
    case 2: {
      text(font_size::x96, rgb::tc, Point{120,100}, "Select Game Mode");
      if (blink_count_ < 30) {
        text(font_size::x48, rgb::tc, Point{240,540}, "press enter to continue");
        ++blink_count_;
      } else if (blink_count_ < 60) {
        ++blink_count_;
      } else {
        blink_count_ = 0;
      }

      switch (game_mode_) {
        case 0: {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          //SDL_RenderFillRect(renderer, &p1_str_dst);
          text(font_size::x64, rgb::op2, p1_mode_pos, p1_mode_str);
          text(font_size::x64, rgb::blink, vs_mode_pos, vs_mode_str);
          break;
        }
        case 1: {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          //SDL_RenderFillRect(renderer, &vs_str_dst);
          text(font_size::x64, rgb::blink, p1_mode_pos, p1_mode_str);
          text(font_size::x64, rgb::op2, vs_mode_pos, vs_mode_str);
          break;
        }
      }

      //continue to next screen
      if (input_manager_->press_key_p(player_type::p1, input_device::x)
          || input_manager_->press_key_p(player_type::p2, input_device::x)
          || input_manager_->press_key_p(player_type::p1, input_device::enter)) {
        ++game_count_;
      }

      //select between the game modes
      if (input_manager_->press_key_p(player_type::p1, input_device::down)
          || input_manager_->press_key_p(player_type::p2, input_device::down)) {
        game_mode_ = 1;
      } else if (input_manager_->press_key_p(player_type::p1, input_device::up)
                 || input_manager_->press_key_p(player_type::p2, input_device::up)) {
        game_mode_ = 0;
      }
      break;
    }
    case 3: {
    //   switch (game_mode_) {
    //     case 0: {
    //       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //       //SDL_RenderFillRect(renderer, &p1_str_dst);
    //       text(font_size::x36, rgb::black, p1_mode_pos, p1_mode_str);
    //       text(font_size::x36, rgb::black, vs_mode_pos, vs_mode_str);
    //       break;
    //     }
    //     case 1: {
    //       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //       //SDL_RenderFillRect(renderer, &vs_str_dst);
    //       text(font_size::x36, rgb::black, p1_mode_pos, p1_mode_str);
    //       text(font_size::x36, rgb::black, vs_mode_pos, vs_mode_str);
    //       break;
    //     }
    //   }

      // initialize globals
      
      map->init();
        //food_->init(*map);
        //enemy_->init();
      p1->init(map.get());
      p2->init(map.get());
        //p1_>set_life(2);
        //p2_->set_life(2);
        //p1_->set_score(0);
        //p2_->set_score(0);
        //p1_->set_damaged(false);
        //p2_->set_damaged(false);
        //p1_->set_power_mode(0);
        //p2_->set_power_mode(0);

      game_count_ = 0;
      game_state_ = game_state::start;
    
      srand((unsigned int)time(nullptr));

      //Mix_PlayMusic(mixer_manager_->get_music(), -1);
      break;
    }
    default:
      break;
  }

  SDL_DestroyTexture(trans_texture);
}