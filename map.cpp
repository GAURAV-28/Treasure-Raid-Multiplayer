#include <bits/stdc++.h>
#include "maze.hpp"
#include "map.hpp"
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
		return map_state::player;
    default:
      std::cerr << "error: undefined character of map" << '\n';
      exit(EXIT_FAILURE);
  }
}

SDL_Texture* loadTexture( SDL_Renderer *renderer,std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

void Map::init(){
  
    //const ImageManager *image_manager_;
    //map_state block_[x_count][21];
    
    MazeGenerator mg;
    const std::string block_src = mg.generate();

	//block_src[22] = 'C';
      
    for (unsigned int y = 0; y < 21; ++y) {
        for (unsigned int x = 0; x < 21; ++x) {
            block[y][x] = char_to_map_state(block_src[y * 21 + x]);
        }
    }

	block[1][1] = map_state::player;

}

void Map::draw() {
    
    //SDL_CreateTextureFromSurface(renderer_, image);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  	SDL_RenderClear(renderer);

  	SDL_Texture *gTexture1 = nullptr;
	gTexture1 = loadTexture(renderer,"9.png");
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

    const SDL_Rect src[2] = {{0, 0, size, size}, {size, 0, size, size}};

    for (unsigned int y = 0; y < 21; ++y) {
    	for (unsigned int x = 0; x < 21; ++x) {
          const SDL_Rect dst = {static_cast<Sint16>(size * x), static_cast<Sint16>(size * y), size, size};

          switch (block[y][x]) {
			case map_state::block:
              //image_manager_->render_copy(*p_texture, src[1], dst);
              SDL_RenderCopy(renderer, gTexture1, &src[0], &dst);
              break;
			case map_state::coin:
			case map_state::player:	
            case map_state::path:
              //image_manager_->render_copy(*p_texture, src[0], dst);
              SDL_RenderCopy(renderer, gTexture1, &src[1], &dst);
              break;
          }
        }
    }
	//SDL_RenderPresent( gRenderer );
    SDL_DestroyTexture(gTexture1);
}