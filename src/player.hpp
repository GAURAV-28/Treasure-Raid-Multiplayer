#pragma once

#include "map.hpp"
#include "image.hpp"
#include "input.hpp"
#include "global.hpp"
#include <iostream>
#include <SDL2/SDL_mixer.h>
#include "sound_manager.hpp"


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
    unsigned int score=0;
    Point pos_;
    Point curr_;
    Point nxt_;
    SDL_Renderer *gRenderer;
    const ImageManager *image_manager_;
    const InputManager *input_manager_;
    const SoundManager *sound_manager_;
    bool damaged;
    public:
    int power_mode;
    int power_up;
    //const Map m;

    public:
      Player(const unsigned char ptype, SDL_Renderer *renderer, const ImageManager *image_manager, const InputManager *input_manager, const SoundManager *sound_manager) : gRenderer(renderer) , type(ptype), image_manager_(image_manager), input_manager_(input_manager), sound_manager_(sound_manager){}
    
      inline void init(Map* map, const bool initial){
      switch (type){
        case player_type::p1:{
          pos_ = {1*block::size, 1*block::size};
          curr_ = {1,1};
          nxt_ = {1,1};
          dir = 0;
          power_mode = 0;
          if(initial){
            power_up = 2;
          }
          
          break;
        }
        case player_type::p2:{
          //if(map->check_state(2,1) == map_state::path){
            pos_ = {2*block::size, 1*block::size};
            curr_= {2,1};
            nxt_ = {2,1};
            power_mode = 0;
            if(initial){
              power_up = 2;
            }
          //}
            
          // else {
          //   pos_ = {1*block::size, 2*block::size};
          //   curr_= {1,2};
          //   nxt_ = {1,2};
          // }
          
          dir = 0;
          break;
        }
        case player_type::enem:{
          break;
        }
      }
    }

    inline void draw(unsigned int mode) const{
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

    void intToByte(int n, unsigned char* result) {

        result[0] = n & 0x000000ff;
        result[1] = (n & 0x0000ff00) >> 8;
        result[2] = (n & 0x00ff0000) >> 16;
        result[3] = (n & 0xff000000) >> 24; 
    }

    inline void move(const Map &map, ENetPeer* peer1, const bool testf) noexcept {
    // if (type_ == player_type::en && mode != game_mode::battle) {
    //   return;
    // }
    unsigned char tobesent[10];
    tobesent[9]='0';
    if (input_manager_->press_key_p(type, input_device::x) && !power_mode){
      if(power_up>0){
        Mix_PlayChannel(se_type::siren, sound_manager_->get_se(se_type::siren),0);
        tobesent[9]='1';
        power_mode = 400;
        power_up--;
      }
      
    }
    tobesent[0]='P';
      intToByte(pos_.x,&tobesent[1]);
      intToByte(pos_.y,&tobesent[5]);
      if(testf){
        ENetPacket* packet = enet_packet_create (tobesent, 
                                         10 ,0);
        enet_peer_send (peer1, 0, packet);
      }
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
        || dst_block_state == map_state::end
        || dst_block_state == map_state::player1_init_pos
        || dst_block_state == map_state::player2_init_pos
      ) {
      nxt_ = dst_block;
    }

  }


  inline Point get_pos() const { return pos_; }
  inline Point get_curr() const { return curr_;}
  inline void set_pos(const Point &p){pos_ = p; curr_ = {p.x/block::size, p.y/block::size}; nxt_ = curr_;}
  inline void set_curr(const Point &p) { pos_ = {p.x * block::size, p.y*block::size}; curr_ = p; nxt_ = p;}
    
  inline Point get_block() const{ return curr_; }

  inline int get_life() const { return life; }
  inline void set_life(const int l) { life = l; }

  inline unsigned int get_score() const { return score; }
  inline void set_score(const unsigned int s) { score = s; }

  inline bool get_damaged() const{ return damaged; }

  inline void set_damaged(const bool damaged_p){
    damaged = damaged_p;
  }

  inline int get_power_mode() const noexcept { return power_mode; }

  inline void set_power_mode(const int power_mode1) noexcept {
    power_mode = power_mode1;
  }

};