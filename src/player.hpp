#pragma once

#include "map.hpp"
#include "image.hpp"
#include "input.hpp"
#include "global.hpp"
#include <iostream>

namespace player_type{
   enum{
       p1=0,
       p2=1,
       enem=2
   };
}

class Player{
    const unsigned char type;
    unsigned char dir;
    int life;
    unsigned int score;
    Point pos_;
    Point curr_;
    Point nxt_;
    SDL_Renderer *gRenderer;
    const ImageManager *image_manager_;
    const InputManager *input_manager_;
    //const Map m;

    public:
      Player(const unsigned char ptype, SDL_Renderer *renderer, const ImageManager *image_manager, const InputManager *input_manager) : gRenderer(renderer) , type(ptype), image_manager_(image_manager), input_manager_(input_manager) {}
    
      inline void init(Map* map){
      switch (type){
        case player_type::p1:{
          pos_ = {1*block::size, 1*block::size};
          curr_ = {1,1};
          nxt_ = {1,1};
          dir = 0;
          break;
        }
        case player_type::p2:{
          if(map->check_state(2,1) == map_state::path){
            pos_ = {2*block::size, 1*block::size};
            curr_= {2,1};
            nxt_ = {2,1};
          }
            
          else {
            pos_ = {1*block::size, 2*block::size};
            curr_= {1,2};
            nxt_ = {1,2};
          }
          
          dir = 0;
          break;
        }
        case player_type::enem:{
          break;
        }
      }
    }

    inline void draw(int mode) const{
      switch (type){
        case player_type::p1:{
          SDL_Texture *p_texture = image_manager_->get(image::p1);
          const SDL_Rect src = {0,0,block::size,block::size};
          const SDL_Rect dst = {5+static_cast<Sint16>(pos_.x),5+static_cast<Sint16>(pos_.y),30,30};
          image_manager_->render_copy(*p_texture, src, dst);
          SDL_DestroyTexture(p_texture);
          break;
        }
        case player_type::p2:{
          if(mode != 1) break;
          
          SDL_Texture *p_texture = image_manager_->get(image::p2);
          const SDL_Rect src = {0,0,block::size,block::size};
          
          const SDL_Rect dst = {5+static_cast<Sint16>(pos_.x),5+static_cast<Sint16>(pos_.y),30,30};
          image_manager_->render_copy(*p_texture, src, dst);
          
          SDL_DestroyTexture(p_texture);
          break;
        }
        case player_type::enem:{
          break;
        }
      }
    }

    inline void move(const Map &map) noexcept {
    // if (type_ == player_type::en && mode != game_mode::battle) {
    //   return;
    // }

    const Point dst_pos = {nxt_.x*block::size, nxt_.y*block::size};

    if (pos_.x != dst_pos.x || pos_.y != dst_pos.y) {
      // ++anime_weight_;
      // if (anime_weight_ > 4) {
      //   anime_count_ = 1 - anime_count_;
      //   anime_weight_ = 0;
      // }
      const unsigned int move_value = 2;
      if (dst_pos.x > pos_.x) {
        pos_.x += move_value;
      }
      if (dst_pos.y > pos_.y) {
        pos_.y += move_value;
      }
      if (dst_pos.x < pos_.x) {
        pos_.x -= move_value;
      }
      if (dst_pos.y < pos_.y) {
        pos_.y -= move_value;
      }
      return;
    }

    curr_ = nxt_;

    // The button precedence is Down > Left > Up > Right.
    Point mut_dst_block = nxt_;
    if (input_manager_->press_key_p(type, input_device::down)) {
      dir = 0;
      ++mut_dst_block.y;
    } else if (input_manager_->press_key_p(type, input_device::left)) {
      dir = 1;
      --mut_dst_block.x;
    } else if (input_manager_->press_key_p(type, input_device::up)) {
      dir = 2;
      --mut_dst_block.y;
    } else if (input_manager_->press_key_p(type, input_device::right)) {
      dir = 3;
      ++mut_dst_block.x;
    }
    const Point dst_block = mut_dst_block;

    const map_state dst_block_state = map.check_state(dst_block.x,dst_block.y);
    const map_state dst_right_block_state =
        map.check_state(dst_block.x + 1, dst_block.y);
    const map_state dst_left_block_state =
        map.check_state(dst_block.x - 1, dst_block.y);
    // TODO: make private function
    if (dst_block_state == map_state::path
        || dst_block_state == map_state::coin
        //|| dst_block_state == map_state::player
        || dst_block_state == map_state::player1_init_pos
        || dst_block_state == map_state::player2_init_pos
      ) {
      nxt_ = dst_block;
    }

    // Circle corner
    // if (map.check_state(Point{dst_block.x + 2, dst_block.y})
    //     == map_state::left_warp_pos) {
    //   next_block_.x = block::count_x;
    //   pos_.x = block::size * next_block_.x;
    // }
    // if (map.check_state(Point{dst_block.x - 2, dst_block.y})
    //     == map_state::right_warp_pos) {
    //   next_block_.x = -1;
    //   pos_.x = block::size * next_block_.x;
    // }
  }

};