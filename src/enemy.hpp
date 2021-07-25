#pragma once

#include "map.hpp"
#include "image.hpp"
#include <iostream>
#include <utility>
#include <stdlib.h>
#include <vector>

namespace enemy_character{
   enum{
       e0=0,
       e1=1,
       e2,
       e3,
       e4,
       e5,
       e6,
       count
   };
}

class Enemy{
    enum class enemy_state{
        normal,
        lose,
    };
    struct Enemy_data{
        const unsigned char type;
        unsigned char dir;
        Point pos_;
        Point curr_;
        Point nxt_;
        enemy_state state;

        Enemy_data(const unsigned char enemy_type) : type(enemy_type) {}
  
    };
    
    std::vector<Enemy_data> enemies_;
    SDL_Renderer *gRenderer;
    const ImageManager *image_manager_;

    void move_normal_enemy(Enemy_data &enemy, const Map &map, const Player &p1, const Player &p2) noexcept;

    public:
      Enemy(SDL_Renderer *renderer, const ImageManager *image_manager) : gRenderer(renderer) , image_manager_(image_manager) {
        enemies_.reserve(enemy_character::count);
        for (unsigned char i = 0; i < enemy_character::count; ++i) {
            enemies_.push_back(Enemy_data(i));
        }
      }
    
      inline void init(){
          Point start_block[enemy_character::count] = {
              {1, 19}, {19, 1}, {19, 19},{1,19}, {19,1}, {19,19}};
        for (auto &enemy : enemies_) {
            enemy.pos_.x = block::size * start_block[enemy.type].x;
            enemy.pos_.y = block::size * start_block[enemy.type].y;
            //enemy.block = start_block[enemy.type];
            //enemy.next_block = start_block[enemy.type];
            //enemy.dir = 2;
            //enemy.anime_count = 0;
            //enemy.anime_weight = 0;
            enemy.dir = 0;
            enemy.curr_= start_block[enemy.type];
            enemy.nxt_ = start_block[enemy.type];
            enemy.state = enemy_state::normal;
            }
    }

    inline void draw() const{
        SDL_Texture *enemies_texture = image_manager_->get(image::enemy);
        for (const auto &enemy : enemies_) {
        const SDL_Rect dst = {5+static_cast<Sint16>(enemy.pos_.x),
                                5+static_cast<Sint16>(enemy.pos_.y),
                                30,
                                30};
        switch (enemy.state) {
            case enemy_state::normal: {
            const SDL_Rect src = {static_cast<Sint16>(0),
                                    static_cast<Sint16>(0),
                                    block::size,
                                    block::size};
            image_manager_->render_copy(*enemies_texture, src, dst);
            break;
            }
            case enemy_state::lose: {
                //write
            }
        }
        
        
        
        //   switch (type){
        //     case player_type::p1:{
        //       SDL_Texture *p_texture = image_manager_->get(image::p1);
        //       const SDL_Rect src = {0,0,size,size};
        //       const SDL_Rect dst = {5+static_cast<Sint16>(pos_x),5+static_cast<Sint16>(pos_y),30,30};
        //       image_manager_->render_copy(*p_texture, src, dst);
        //       SDL_DestroyTexture(p_texture);
        //       break;
        //     }
        //     case player_type::p2:{
        //       if(mode != 1) break;
        //       SDL_Texture *p_texture = image_manager_->get(image::p2);
        //       const SDL_Rect src = {0,0,size,size};
        //       const SDL_Rect dst = {5+static_cast<Sint16>(pos_x),5+static_cast<Sint16>(pos_y),30,30};
        //       image_manager_->render_copy(*p_texture, src, dst);
        //       SDL_DestroyTexture(p_texture);
        //       break;
        //     }
        //     case player_type::enem:{
        //       break;
        //     }
        //   }
        }
        SDL_DestroyTexture(enemies_texture);
    }
    inline void move(const Map &map,
                   const Player &p1, const Player &p2) noexcept {
        for (auto &enemy : enemies_) {
        if (enemy.state == enemy_state::lose) {
            //move_lose_enemy(enemy, map, p1, p2);
        } else {
            move_normal_enemy(enemy, map, p1, p2);
        }
        }
    }




    void check_hit_enemy(unsigned int game_mode_, Player &p1, Player &p2) noexcept{
        
        for (auto &enemy : enemies_) {
        if (enemy.state == enemy_state::lose) {
            //move_lose_enemy(enemy, map, p1, p2);
        } else {
            if(p1.get_pos() == enemy.pos_){
                p1.set_life(p1.get_life()-1);
                p1.set_pos((Point){1,1});
                break;
            }
        }
        }
        if(game_mode_==1){
            for (auto &enemy : enemies_) {
        if (enemy.state == enemy_state::lose) {
            //move_lose_enemy(enemy, map, p1, p2);
        } else {
            if(p2.get_pos() == enemy.pos_){
                p2.set_life(p2.get_life()-1);
                p2.set_pos((Point){1,1});
                break;
            }
        }
        }
        }
    }

};