#include "game.hpp"
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <sstream>
#include "global.hpp"
//#include "enemy.hpp"
#include "font.hpp"
//#include "food.hpp"
#include "image.hpp"
#include "input.hpp"
#include "map.hpp"
//#include "mixer_manager.hpp"
#include "player.hpp"


bool Game::init(){
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		//Create window
		window = SDL_CreateWindow( "Pyramid SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen::width, screen::height, SDL_WINDOW_SHOWN );
		if( window == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			//Create renderer for window
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
			if( renderer == NULL ){
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else{
				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

//text function
void Game::text(const unsigned char font_size, const RGB &rgb, const Point &p, const char *str) const{
    const SDL_Color color = {rgb.red, rgb.green, rgb.blue, 255};
    SDL_Surface *font_surface = TTF_RenderUTF8_Blended(font_.get(font_size), str, color);
    SDL_Texture *font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    const SDL_Rect src = {0, 0, static_cast<Uint16>(font_surface->w),static_cast<Uint16>(font_surface->h)};
    SDL_Rect dst;
    dst.x = static_cast<Sint16>(p.x);
    dst.y = static_cast<Sint16>(p.y);
    SDL_QueryTexture(font_texture, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, font_texture, &src, &dst);
    SDL_DestroyTexture(font_texture);
}

void Game::translucence(){     //to draw the translucent surface on top when paused
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface *trans_surface = 
        (SDL_CreateRGBSurface(SDL_SWSURFACE, screen::width, screen::height, 32, rmask, gmask, bmask, amask));
    if (trans_surface == nullptr) {
      std::cerr << "CreateRGBSurface failed: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    SDL_Texture *trans_texture = SDL_CreateTextureFromSurface(renderer, trans_surface);
    //SDL_Texture *trans_texture = image_manager_->get(image::fullbg);
    SDL_FreeSurface(trans_surface);

    const SDL_Rect dst = {0, 0, screen::width, screen::height};
    SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
    SDL_DestroyTexture(trans_texture);
}

//pause function
void Game::game_pause(){
    map->draw();
    coin->draw();
    //enemy_->draw();
    p1->draw(game_mode_);
    p2->draw(game_mode_);
    score();
    translucence();
    if (input_manager_->edge_key_p(player_type::p1, input_device::space)) {
      game_state_ = game_state::playing;
    }

    if (blink_count_ < 30) {
      text(font_size::x96, rgb::op2, Point{280, 280}, "PAUSED");
      text(font_size::x48, rgb::op2, Point{220, 400}, "press space to continue");
      ++blink_count_;
    } else if (blink_count_ < 60) {
      ++blink_count_;
    } else {
      blink_count_ = 0;
    }
}

void Game::game_win(){
    
    SDL_Texture *trans_texture = image_manager_->get(image::fullbg);
    const SDL_Rect dst = {0, 0, screen::width, screen::height};
    SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
    SDL_DestroyTexture(trans_texture);

    switch (game_mode_){
      case 0:{
        std::stringstream score;
        score << "YOUR SCORE : " << p1->get_score();
        text(font_size::x96, rgb::tc, Point{240, 200}, "YOU WIN !!!");
        text(font_size::x64, rgb::op2, Point{240,360}, score.str().c_str());
        break;
      }
      case 1:{
        std::stringstream score1;
        std::stringstream score2;
        const unsigned int p1s =  p1->get_score();
        const unsigned int p2s =  p2->get_score();
        score1 << "PLAYER A SCORE : " << p1s;
        score2 << "PLAYER B SCORE : " << p2s;
        text(font_size::x64, rgb::op2, Point{200,360}, score1.str().c_str());
        text(font_size::x64, rgb::op2, Point{200,440}, score2.str().c_str());

        if(p1s>p2s){
          text(font_size::x96, rgb::tc, Point{130, 200}, "PLAYER A WINS !!!");
        } else if(p1s == p2s){
          text(font_size::x96, rgb::tc, Point{340, 200}, "TIE !!!");
        } else{
          text(font_size::x96, rgb::tc, Point{130, 200}, "PLAYER B WINS !!!");
        }
        break;
      }
      default:
        break;
    }

    if (blink_count_ < 30) {
      text(font_size::x48, rgb::tc, Point{240, 540}, "press enter to continue");
      ++blink_count_;
    } else if (blink_count_ < 60) {
      ++blink_count_;
    } else {
      blink_count_ = 0;
    }

    if (input_manager_->edge_key_p(player_type::p1, input_device::enter)) {
      blink_count_= 0;
      game_count_ = 0;
      menu_option_= 0;
      game_mode_  = 0;
      game_state_ = game_state::title;
    }
}

