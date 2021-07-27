#include "game.hpp"
#include <time.h>
#include <sstream>
#include "global.hpp"
#include "enemy.hpp"
#include "font.hpp"
#include "coins.hpp"
#include "image.hpp"
#include "input.hpp"
#include "map.hpp"
//#include "mixer_manager.hpp"
//#include <SDL2/SDL_mixer.h>
#include "player.hpp"

void Game::game_over(){
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  SDL_Texture *trans_texture = image_manager_->get(image::fullbg);
  const SDL_Rect dst = {0, 0, screen::width, screen::height};
  SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
  SDL_DestroyTexture(trans_texture);

  switch (game_mode_) {
    case 0: {
      std::stringstream score;
      score << "YOUR SCORE : " << p1->get_score();
      text(font_size::x96, rgb::tc, Point{200, 200}, "GAME OVER !!!");
      text(font_size::x64, rgb::blink, Point{160, 320}, "BETTER LUCK NEXT TIME...");
      text(font_size::x64, rgb::op2, Point{240,400}, score.str().c_str());

      break;
    }
    case 1: {
      std::stringstream score1;
      std::stringstream score2;
      const unsigned int p1s =  p1->get_score();
      const unsigned int p2s =  p2->get_score();
      score1 << "PLAYER A SCORE : " << p1s;
      score2 << "PLAYER B SCORE : " << p2s;
      text(font_size::x96, rgb::tc, Point{200, 200}, "GAME OVER !!!");
      text(font_size::x64, rgb::blink, Point{160, 320}, "BETTER LUCK NEXT TIME...");
      text(font_size::x64, rgb::op2, Point{200,400}, score1.str().c_str());
      text(font_size::x64, rgb::op2, Point{200,480}, score2.str().c_str());

      break;
    }

    default: break;
  }

    if (blink_count_ < 30) {
      text(font_size::x48, rgb::tc, Point{240, 600}, "press enter to continue");
      ++blink_count_;
    } else if (blink_count_ < 60) {
      ++blink_count_;
    } else {
      blink_count_ = 0;
    }

    if (input_manager_->edge_key_p(player_type::p1, input_device::enter)) {
        blink_count_= 0;
        game_count_ = 0;
        menu_option_= 0;
        game_mode_  = 0;
        game_state_ = game_state::title;
    }
}

void Game::game_miss(){
  map->draw();
  coin->draw();
  //enemy_->draw();
  p1->draw(game_mode_);
  p2->draw(game_mode_);
  enemy->draw();
  score();

  if (game_count_ == 0) {
    //Mix_HaltChannel(-1);
    //Mix_PlayChannel(-1, mixer_manager_->get_se(se_type::death), 0);
    ++game_count_;
    return;
  }

  // if (p1->get_life() == 0 || p2->get_life() == 0) {
  //   game_count_  = 0;
  //   blink_count_ = 0;
  //   game_state_  = game_state::win;
  // } 


  Player *p = p1->get_damaged() ? p1.get() : p2.get();
  const Point pos = p->get_pos();
  p->set_curr(Point{1, 1});
  
  const int life = p->get_life()-1;
  p->set_life(life);

  if (life >=0 ) {
    game_count_ = 0;
    game_state_ = game_state::start;
    //enemy_->init();
    p->init(map.get(), false);
    //p2->init(map.get());
    p1->set_damaged(false);
    p2->set_damaged(false);
    } else {
      game_count_  = 0;
      blink_count_ = 0;
      game_state_  = game_state::gameover;
    }
  
}