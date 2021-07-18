#pragma once

#include <iostream>
#include <string>
#include "maze.hpp"
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
    MazeGenerator mg;
    const std::string block_src = mg.generate();

    for (unsigned int y = 0; y < 21; ++y) {
        for (unsigned int x = 0; x < 21; ++x) {
            block[y][x] = char_to_map_state(block_src[y * 21 + x]);
        }
    }

	block[1][1] = map_state::player1_init_pos;
  if(block[2][1]!=map_state::block) block[2][1]=map_state::player2_init_pos;
  else block[1][2]=map_state::player2_init_pos;

}

void Map::draw() const{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
    
  SDL_Texture *p_texture = nullptr;
  p_texture = image_manager_->get(image::bg);

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

    for (unsigned int y = 0; y < 21; ++y) {
    	for (unsigned int x = 0; x < 21; ++x) {
          const SDL_Rect dst = {static_cast<Sint16>(block::size * x), static_cast<Sint16>(block::size * y), block::size, block::size};

          switch (block[y][x]) {
			      case map_state::block:
              image_manager_->render_copy(*p_texture, src[0], dst);
              //SDL_RenderCopy(renderer, gTexture1, &src[0], &dst);
              break;
			      case map_state::coin:
			      case map_state::player1_init_pos:
            case map_state::player2_init_pos:	
            case map_state::path:
              //image_manager_->render_copy(*p_texture, src[0], dst);
              image_manager_->render_copy(*p_texture, src[1], dst);
              break;
          }
        }
    }
  SDL_DestroyTexture(p_texture);
}