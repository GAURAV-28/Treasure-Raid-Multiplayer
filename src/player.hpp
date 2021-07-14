#pragma once

#include "map.hpp"
#include "image.hpp"
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
    int pos_x , pos_y;
    int curr_x , curr_y;
    int next_x , next_y;
    SDL_Renderer *gRenderer;
    const ImageManager *image_manager_;

    public:
      Player(const unsigned char ptype, SDL_Renderer *renderer, const ImageManager *image_manager) : gRenderer(renderer) , type(ptype), image_manager_(image_manager) {}
    
      inline void init(){
      switch (type){
        case player_type::p1:{
          pos_x = size*1;
          pos_y = size*1;
          curr_x = curr_y = 1;
          next_x = next_y = 1;
          dir = 0;
          break;
        }
        case player_type::p2:{
          pos_x = size*2;
          pos_y = size*1;
          curr_x = curr_y = 1;
          next_x = next_y = 1;
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
          const SDL_Rect src = {0,0,size,size};
          const SDL_Rect dst = {5+static_cast<Sint16>(pos_x),5+static_cast<Sint16>(pos_y),30,30};
          image_manager_->render_copy(*p_texture, src, dst);
          SDL_DestroyTexture(p_texture);
          break;
        }
        case player_type::p2:{
          if(mode != 1) break;
          SDL_Texture *p_texture = image_manager_->get(image::p2);
          const SDL_Rect src = {0,0,size,size};
          const SDL_Rect dst = {5+static_cast<Sint16>(pos_x),5+static_cast<Sint16>(pos_y),30,30};
          image_manager_->render_copy(*p_texture, src, dst);
          SDL_DestroyTexture(p_texture);
          break;
        }
        case player_type::enem:{
          break;
        }
      }
    }

};