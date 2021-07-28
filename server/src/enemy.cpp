#include "game.hpp"
//#include <SDL2/SDL_mixer.h>
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
#include "player.hpp"
#include <iostream>
#include <utility>
#include <stdlib.h>
#include <vector>

    bool check_move_to(const map_state dst_block_state){
        return dst_block_state == map_state::path
        || dst_block_state == map_state::coin
        || dst_block_state == map_state::end
        || dst_block_state == map_state::player1_init_pos
        || dst_block_state == map_state::player2_init_pos;
    }

    void Enemy::move_normal_enemy(Enemy_data &enemy, const Map &map, const Player &p1, const Player &p2) noexcept{
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
        //const Point up_block = enemy.curr_+(Point){0,-1};
        //const Point right_block = enemy.curr_+(Point){1,0};
        //const Point down_block = enemy.curr_+{0,1};
        //const Point left_block = enemy.curr_ + {-1,0};
        const Point next_block[] = {enemy.curr_+(Point){0,-1},enemy.curr_+(Point){1,0},enemy.curr_+(Point){0,1},enemy.curr_+(Point){-1,0}};
        const map_state up_block_state = map.check_state(next_block[0].x,next_block[0].y);
        const map_state right_block_state = map.check_state(next_block[1].x,next_block[1].y);
        const map_state down_block_state = map.check_state(next_block[2].x,next_block[2].y);
        const map_state left_block_state = map.check_state(next_block[3].x,next_block[3].y);
        // const bool move_to_up_block = move_to(up_block_state);
        // const bool move_to_right_block = move_to(right_block_state);
        // const bool move_to_down_block = move_to(down_block_state);
        // const bool move_to_left_block = move_to(left_block_state);
        const bool move_to_block[] = {check_move_to(up_block_state), check_move_to(right_block_state), check_move_to(down_block_state), check_move_to(left_block_state)};
        std::vector<Point> possible;
        std::vector<unsigned char> direc;
        for(int i=0; i < 4; i++){
            if((enemy.dir+2)%4!=i && move_to_block[i]){
                possible.push_back(next_block[i]);
                direc.push_back(i);
            }
        }
        if(possible.size()==0){
            possible.push_back(next_block[(enemy.dir+2)%4]);
            direc.push_back((enemy.dir+2)%4);
        }
        const unsigned next = rand()%possible.size();
        enemy.nxt_= possible[next];
        enemy.dir = direc[next];
        return;

        
    }

    //bool Enemy::check_hit_enemy(unsigned int game_mode_, Player &p1, Player &p2) noexcept{

    //}