#pragma once

#include <iostream>
#include <string>
#include "map.hpp"
#include "global.hpp"
#include "image.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <random>

map_state char_to_map_state(const char c) {
  switch (c) {
    case '#':
      return map_state::block;
    case ' ':
      return map_state::path;
	case 'C':
		return map_state::coin;
	case 'P':
		return map_state::player1_init_pos;
    default:
      std::cerr << "error: undefined character of map" << '\n';
      exit(EXIT_FAILURE);
  }
}

void Map::init(){
  
    //const ImageManager *image_manager_;
    //map_state block_[x_count][21];
    
    const std::string block_src = mg.generate();

    for (unsigned int y = 0; y < block::y_count; ++y) {
        for (unsigned int x = 0; x < block::x_count; ++x) {
            block[y][x] = char_to_map_state(block_src[y * block::y_count + x]);
        }
    }

    // for (unsigned int y = 1; y < block::y_count-1; ++y) {
    //   block[y][1] = map_state::path;
    //   block[y][19] = map_state::path;
    // }

    // for (unsigned int x = 1; x < block::x_count-1; ++x) {
    //   block[1][x] = map_state::path;
    //   block[19][x] = map_state::path;
    // }

  block[1][2] = map_state::path;

	block[1][1] = map_state::player1_init_pos;
  block[1][2] = map_state::player2_init_pos;
  

  block[19][19]=map_state::end;
}

void Map::draw() const{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
    
  SDL_Texture *p_texture = nullptr;
  p_texture = image_manager_->get(image::bg);

  SDL_Texture *crown_tex = NULL;
  crown_tex = image_manager_->get(image::crown);

    //SDL_CreateTextureFromSurface(renderer_, image);

  	// const unsigned int mod = game_level % 4;
  	// if (mod == 1) {
    // 	gTexture1 = loadTexture("9.png");
  	// } else if (mod == 2) {
    // 	gTexture1 = loadTexture("9.png");
  	// } else if (mod == 3) {
    // 	gTexture1 = loadTexture("9.png");
  	// } else {
    // 	gTexture1 = loadTexture("9.png");
  	// }

    const SDL_Rect src[2] = {{0, 0, block::size, block::size}, {block::size, 0, block::size, block::size}};

    for (unsigned int y = 0; y < block::y_count; ++y) {
    	for (unsigned int x = 0; x < block::x_count; ++x) {
          const SDL_Rect dst = {static_cast<Sint16>(block::size * x), static_cast<Sint16>(block::size * y), block::size, block::size};

          switch (block[y][x]) {
			      case map_state::block:
              image_manager_->render_copy(*p_texture, src[0], dst);
              //SDL_RenderCopy(renderer, gTexture1, &src[0], &dst);
              break;
			      case map_state::player1_init_pos:
            case map_state::player2_init_pos:	
            case map_state::coin:
            case map_state::path:
              image_manager_->render_copy(*p_texture,src[1],dst);
              break;
            case map_state::end:
              image_manager_->render_copy(*p_texture,src[1],dst);
              image_manager_->render_copy(*crown_tex,src[0],dst);
              break;
          }
        }
    }
  SDL_DestroyTexture(p_texture);
  SDL_DestroyTexture(crown_tex);
}