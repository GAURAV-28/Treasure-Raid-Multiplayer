#include "game.hpp"
#include <time.h>
#include <sstream>
#include "global.hpp"
#include "coins.hpp"
#include "image.hpp"

void Game::score() const{
    // {
    //   SDL_Texture *p_texture = image_manager_->get(image::plate);
    //   SDL_Rect dst;
    //   dst.x = screen::offset_x;
    //   dst.y = 0;
    //   SDL_QueryTexture(p_texture, nullptr, nullptr, &dst.w, &dst.h);
    //   SDL_RenderCopy(renderer_, p_texture, nullptr, &dst);
    //   SDL_DestroyTexture(p_texture);
    // }

    // Draw the score itself.
    {
      if(game_mode_==0){
        std::stringstream score;
        score << "SCORE : " << p1->get_score();
        text(font_size::x36, rgb::op2, Point{360, 0}, score.str().c_str());

        SDL_Texture *p_texture = image_manager_->get(image::p1);
        const SDL_Rect src = {0,0,block::size,block::size};
        const SDL_Rect dst = {5+static_cast<Sint16>(720),5+static_cast<Sint16>(0),30,30};
        image_manager_->render_copy(*p_texture, src, dst);
        SDL_DestroyTexture(p_texture);
        SDL_Texture *p_texture1 = image_manager_->get(image::enemy);
        const SDL_Rect src1 = {0,0,block::size,block::size};
        const SDL_Rect dst1 = {5+static_cast<Sint16>(600),5+static_cast<Sint16>(0),30,30};
        image_manager_->render_copy(*p_texture1, src1, dst1);
        SDL_DestroyTexture(p_texture1);
        std::stringstream pow;
        pow << "x " << p1->power_up;
        text(font_size::x36, rgb::op2, Point{640, 0}, pow.str().c_str());
      }
      

      std::stringstream life;
      life << "x " << p1->get_life();
      text(font_size::x36, rgb::op2, Point{760, 0}, life.str().c_str());

      if (game_mode_ == 1) {
        std::stringstream score;
        score << "SCORE  A : " << p1->get_score();
        text(font_size::x36, rgb::op2, Point{120, 0}, score.str().c_str());

        SDL_Texture *p_texture = image_manager_->get(image::p1);
        const SDL_Rect src = {0,0,block::size,block::size};
        const SDL_Rect dst = {5+static_cast<Sint16>(720),5+static_cast<Sint16>(0),30,30};
        image_manager_->render_copy(*p_texture, src, dst);
        SDL_DestroyTexture(p_texture);

        std::stringstream score1;
        score1 << " B : " << p2->get_score();
        text(font_size::x36, rgb::op2, Point{360, 0}, score1.str().c_str());

        SDL_Texture *p_texture1 = image_manager_->get(image::p2);
        const SDL_Rect src1 = {0,0,block::size,block::size};
        const SDL_Rect dst1 = {5+static_cast<Sint16>(560),5+static_cast<Sint16>(0),30,30};
        image_manager_->render_copy(*p_texture1, src1, dst1);
        SDL_DestroyTexture(p_texture);

        std::stringstream life;
        life << "x " << p2->get_life();
        text(font_size::x36, rgb::op2, Point{600,0},life.str().c_str());

        SDL_Texture *p_texture2 = image_manager_->get(image::enemy);
        const SDL_Rect src2 = {0,0,block::size,block::size};
        const SDL_Rect dst2 = {5+static_cast<Sint16>(640),5+static_cast<Sint16>(0),30,30};
        const SDL_Rect dst3 = {5+static_cast<Sint16>(480),5+static_cast<Sint16>(0),30,30};
        image_manager_->render_copy(*p_texture2, src2, dst2);
        image_manager_->render_copy(*p_texture2, src2, dst3);
        SDL_DestroyTexture(p_texture1);
        std::stringstream pow;
        pow << "x " << p1->power_up;
        text(font_size::x36, rgb::op2, Point{680, 0}, pow.str().c_str());
        std::stringstream pow1;
        pow1 << "x " << p2->power_up;
        text(font_size::x36, rgb::op2, Point{520, 0}, pow1.str().c_str());
      }
    }

    // Draw the rest time of power mode.
    {
    //   const unsigned int x = screen::offset_x + 10;
    //   const unsigned int y = screen::height / 6 * 4;
    //   if (p1_->get_power_mode()) {
    //     SDL_SetRenderDrawColor(renderer_, 255, 255, 0, 255);
    //     const SDL_Rect dst = {
    //         x, y, static_cast<Uint16>(p1_->get_power_mode() / 4), block::size};
    //     SDL_RenderFillRect(renderer_, &dst);
    //   }
      // if (p2_->get_power_mode()) {
      //   SDL_SetRenderDrawColor(renderer_, 128, 128, 0, 255);
      //   const SDL_Rect dst = {x, y + 30,
      //                         static_cast<Uint16>(p2_->get_power_mode() / 4),
      //                         block::size};
      //   SDL_RenderFillRect(renderer_, &dst);
      // }
    }
  }