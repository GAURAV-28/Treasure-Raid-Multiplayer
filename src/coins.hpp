#pragma once

#include "global.hpp"
#include "image.hpp"
#include "map.hpp"
#include "player.hpp"
//#include "enemy.hpp"
//#include <SDL2/SDL_mixer.h>
//#include "mixer_manager.hpp"


class Coin {
  enum class coin_state {
    coin,
    none,
    treasure
  };

  coin_state coin_[block::y_count][block::x_count];
  const ImageManager *image_manager_;
  //const MixerManager *mixer_manager_;

 public:
  Coin(const ImageManager *image_manager)
      : image_manager_(image_manager){}

  inline void init(Map* map, unsigned int game_mode_){
    for (int y = 0; y < block::y_count; ++y) {
      for (int x = 0; x < block::x_count; ++x) {
          
          switch (map->check_state(x,y))
          {
            case map_state::path:{
                coin_[y][x]=coin_state::coin;
                break;
            }
            case map_state::player2_init_pos:{
                if(game_mode_==0) coin_[y][x]=coin_state::coin;
                break;
            }
            default:{
                
                coin_[y][x] = coin_state::none;
                break;
            }
          }

          
        // switch (map->check_state(x, y)) {
        //     std::cout<<"in1\n";
        //   case map_state::path:
        //     coin_[y][x] = coin_state::coin;
        //     //block[y][x] = map_state::coin;
        //     break;
        //   default:
        //     coin_[y][x] = coin_state::empty;
        //     break;
        // }
      }
    }
  }

  inline void draw(){
    const SDL_Rect src = {0, 0, block::size, block::size};
    SDL_Texture *coin_texture = image_manager_->get(image::coin);
    //SDL_Texture *food_counter_texture = image_manager_->get(image::food_counter);
    for (unsigned int y = 0; y < block::y_count; ++y) {
      for (unsigned int x = 0; x < block::x_count; ++x) {
        SDL_Rect dst; //= {5+static_cast<Sint16>(block::size*x),5+static_cast<Sint16>(block::size*y),10,10};
        dst.x = 5+static_cast<Sint16>(block::size * x);
        dst.y = 5+static_cast<Sint16>(block::size * y);
        switch (coin_[y][x]) {
          case coin_state::coin: {
            SDL_QueryTexture(coin_texture, nullptr, nullptr, &dst.w, &dst.h);
            image_manager_->render_copy(*coin_texture, src, dst);
            break;
          }
        //   case food_state::counter_food: {
        //     SDL_QueryTexture(food_counter_texture, nullptr, nullptr, &dst.w,
        //                      &dst.h);
        //     image_manager_->render_copy(*food_counter_texture, src, dst);
        //     break;
        //   }
          default:
            break;
        }
      }
    }
    SDL_DestroyTexture(coin_texture);
    //SDL_DestroyTexture(food_counter_texture);
  }

  /**
   * Return true if all the foods are eaten, and false otherwise.
   */
//   inline bool check_state(const game_mode mode, Player &p1) noexcept {
//     const Point block = p1.get_block();
//     switch (food_[block.y][block.x]) {
//       case food_state::nothing: {
//         p1.set_score(p1.get_score() + 1);
//         break;
//       }
//       case food_state::counter_food: {
//         p1.set_score(p1.get_score() + 2);
//         p1.set_power_mode(400);
//         food_[block.y][block.x] = food_state::nothing;
//         break;
//       }
//       default:
//         // do nothing
//         break;
//     }

//     // if (mode == game_mode::battle) {
//     //   const Point block = p2.get_block();
//     //   switch (food_[block.y][block.x]) {
//     //     case food_state::food: {
//     //       Mix_PlayChannel(se_type::chomp,
//     //                       mixer_manager_->get_se(se_type::chomp), 0);
//     //       food_[block.y][block.x] = food_state::nothing;
//     //       p2.set_score(p2.get_score() + 10);
//     //       break;
//     //     }
//     //     case food_state::counter_food: {
//     //       p2.set_power_mode(400);
//     //       food_[block.y][block.x] = food_state::nothing;
//     //       break;
//     //     }
//     //     default:
//     //       // do nothing
//     //       break;
//     //   }
//     // }

//     if ((p1.get_power_mode() != 0 && p1.get_power_mode() % 80 == 0)) {
//       Mix_PlayChannel(se_type::siren, mixer_manager_->get_se(se_type::siren),
//                       0);
//     }

//     int rest_food = 0;
//     for (unsigned int y = 0; y < block::count_y; ++y) {
//       for (unsigned int x = 0; x < block::count_x; ++x) {
//         if (food_[y][x] == food_state::food
//             || food_[y][x] == food_state::counter_food) {
//           ++rest_food;
//         }
//       }
//     }

//     return rest_food == 0;
//   }

  ~Coin(){}
};
