// #include "game.hpp"
// #include <SDL2/SDL_mixer.h>
// #include <time.h>
// #include <sstream>
// #include "global.hpp"
// //#include "enemy.hpp"
// #include "font.hpp"
// //#include "food.hpp"
// #include "image.hpp"
// #include "input.hpp"
// #include "map.hpp"
// //#include "mixer_manager.hpp"
// #include "player.hpp"

// void Game::game_over(){
//   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//   SDL_RenderClear(renderer);

//   const Point gameover_pos = Point{165, 100};
//   const char *gameover_str = "Game Over";
//   switch (game_mode_) {
//     case 0: {
//       switch (game_count_) {
//         case 0: {
//           draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
//           wipe_->set_wipe_in();
//           wipe_->draw(screen::width);
//           ++game_count_;
//           break;
//         }
//         case 1: {
//           draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
//           wipe_->draw(screen::width);
//           if (wipe_->update()) {
//             ++game_count_;
//           }
//           break;
//         }
//         case 2: {
//           draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
//           std::stringstream ss;
//           ss << "Y o u r  S c o r e   " << p1_->get_score();
//           draw_text(font_size::x36, rgb::black, Point{120, 220},
//                     ss.str().c_str());

//           if (blink_count_ < 30) {
//             draw_text(font_size::x16, rgb::black, Point{210, 350},
//                       "P r e s s  S p a c e  K e y");
//             ++blink_count_;
//           } else if (blink_count_ < 60) {
//             ++blink_count_;
//           } else {
//             blink_count_ = 0;
//           }

//           if (input_manager_->press_key_p(player_type::p1, input_device::x)
//               || input_manager_->press_key_p(player_type::en, input_device::x)
//               || input_manager_->press_key_p(player_type::p1, input_device::space)) {
//             ++game_count_;
//             wipe_->set_wipe_out();
//             wipe_->draw(screen::width);
//           }
//           break;
//         }
//         case 3: {
//           wipe_->draw(screen::width);
//           if (wipe_->update()) {
//             blink_count_ = 0;
//             game_count_ = 0;
//             game_state_ = game_state::title;
//             Mix_HaltMusic();
//           }
//           break;
//         }
//         default:
//           // NOTREACHED
//           break;
//       }
//       break;
//     }
//     case 1: {
//       switch (game_count_) {
//         case 0: {
//           draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
//           wipe_->set_wipe_in();
//           wipe_->draw(screen::width);
//           ++game_count_;
//           break;
//         }
//         case 1: {
//           draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
//           wipe_->draw(screen::width);
//           if (wipe_->update()) {
//             ++game_count_;
//           }
//           break;
//         }
//         case 2: {
//           draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
//           std::stringstream ss;
//           const unsigned int p1_score = p1_->get_score();
//           //const unsigned int p2_score = p2_->get_score();
//           // if (p1_score > p2_score) {
//           //   ss << "1 P  W I N  " << p1_score;
//           //   draw_text(font_size::x36, rgb::black, Point{170, 240},
//           //             ss.str().c_str());
//           // } else if (p1_score < p2_score) {
//           //   ss << "2 P  W I N  " << p2_score;
//           //   draw_text(font_size::x36, rgb::black, Point{170, 240},
//           //             ss.str().c_str());
//           // } else {
//           //   ss << "D R A W  " << p1_score;
//           //   draw_text(font_size::x36, rgb::black, Point{170, 240},
//           //             ss.str().c_str());
//           // }

//           if (blink_count_ < 30) {
//             draw_text(font_size::x16, rgb::black, Point{210, 380},
//                       "P r e s s  S p a c e  K e y");
//             ++blink_count_;
//           } else if (blink_count_ < 60) {
//             ++blink_count_;
//           } else {
//             blink_count_ = 0;
//           }

//           if (input_manager_->press_key_p(player_type::p1, input_device::x)
//               || input_manager_->press_key_p(player_type::en, input_device::x)
//               || input_manager_->press_key_p(player_type::p1, input_device::space)) {
//             ++game_count_;
//             wipe_->set_wipe_out();
//             wipe_->draw(screen::width);
//           }
//           break;
//         }
//         case 3: {
//           wipe_->draw(screen::width);
//           if (wipe_->update()) {
//             blink_count_ = 0;
//             game_count_ = 0;
//             game_state_ = game_state::title;
//           }
//           break;
//         }
//         default:
//           // NOTREACHED
//           break;
//       }
//       break;
//     }
//   }
// }