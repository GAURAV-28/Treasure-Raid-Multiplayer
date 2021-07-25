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
       //e3,
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
          std::pair<int,int> start_block[enemy_character::count] = {
              {1, 19}, {19, 1}, {19, 19}};
        for (auto &enemy : enemies_) {
            enemy.pos_.x = block::size * start_block[enemy.type].first;
            enemy.pos_.y = block::size * start_block[enemy.type].second;
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

    bool move_to(const map_state dst_block_state){
        return dst_block_state == map_state::path
        || dst_block_state == map_state::coin
        || dst_block_state == map_state::end
        || dst_block_state == map_state::player1_init_pos
        || dst_block_state == map_state::player2_init_pos
    }

    void move_normal_enemy(Enemy_data &enemy, const Map &map, const Player &p1, const Player &p2) noexcept{
        const Point dst_pos = {enemy.nxt_.x * block::size,
                                enemy.nxt_.y * block::size};
        if(enemy.pos_ != dst_pos){
            const unsigned int move_value = 2;
            if (dst_pos.x > enemy.pos_.x) {
                enemy.pos_.x += move_value;
                }
            if (dst_pos.y > enemy.pos_.y) {
                enemy.pos_.y += move_value;
            }
            if (dst_pos.x < enemy.pos_.x) {
                enemy.pos_.x -= move_value;
            }
            if (dst_pos.y < enemy.pos_.y) {
                enemy.pos_.y -= move_value;
            }
            return;
        }

        enemy.curr_ = enemy.nxt_;
        const Point up_block = enemy.curr_+{0,-1};
        const Point right_block = enemy.curr_+{1,0};
        const Point down_block = enemy.curr_+{0,1};
        const Point left_block = enemy.curr_ + {-1,0};
        const Point[] next_block = {enemy.curr_+{0,-1},enemy.curr_+{1,0},enemy.curr_+{0,1},enemy.curr_+{-1,0}};
        const map_state up_block_state = map.check_state(up_block);
        const map_state right_block_state = map.check_state(right_block);
        const map_state down_block_state = map.check_state(down_block);
        const map_state left_block_state = map.check_state(left_block);
        // const bool move_to_up_block = move_to(up_block_state);
        // const bool move_to_right_block = move_to(right_block_state);
        // const bool move_to_down_block = move_to(down_block_state);
        // const bool move_to_left_block = move_to(left_block_state);
        const bool[] move_to_block = {move_to(up_block_state), move_to(right_block_state), move_to(down_block_state), move_to(left_block_state)};
        vector<Point> possible;
        vector<unsigned char> direc;
        for(int i=0; i < 4; i++){
            if((enemy.dir+2)%4!=i && move_to_block[i]){
                possible.push_back(next_block[i]);
                direc.push_back(i);
            }
        }
        if(possible.size()==0){
            possible.push_back(next_block[enemy.dir]);
            direc.push_back(i);
        }
        const unsigned next = rand()%possible.size();
        enemy.nxt_= possible[next];
        enemy.dir = direc[next];
        return;

        
    }

    //bool check_hit_enemy(const game_mode mode, Player &p1, Player &p2) noexcept;

};