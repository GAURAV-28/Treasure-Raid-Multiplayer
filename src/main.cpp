#include <iostream>
#include <iomanip>
#include <memory>
#include "map.hpp"
#include "global.hpp"
#include "player.hpp"
#include "image.hpp"
#include "input.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Map pointer
std::unique_ptr<ImageManager> image_manager_;
std::unique_ptr<InputManager> input_manager_;
std::unique_ptr<Map> map;
std::unique_ptr<Player> p1,p2;

enum class game_mode {
  single,
  battle,
  playenem
};

void wait_game(){
    static Uint32 pre_count;
    const double wait_time = 1000.0 / 60;
    const Uint32 wait_count = (wait_time + 0.5);
    if (pre_count) {
      const Uint32 now_count = SDL_GetTicks();
      const Uint32 interval = now_count - pre_count;
      if (interval < wait_count) {
        const Uint32 delay_time = wait_count - interval;
        SDL_Delay(delay_time);
      }
    }
    pre_count = SDL_GetTicks();
  }

int main(){

    init();
	image_manager_ = std::make_unique<ImageManager>(renderer);
	input_manager_ = std::make_unique<InputManager>();
    map = std::make_unique<Map>(renderer,image_manager_.get());
	p1 = std::make_unique<Player>(player_type::p1, renderer, image_manager_.get(), input_manager_.get());
	p2 = std::make_unique<Player>(player_type::p2, renderer, image_manager_.get(), input_manager_.get());
    //SDL_ShowCursor(SDL_DISABLE);

    bool quit = false;
	int mode = 1;
	//Event handler
	SDL_Event e;
    map->init();
	p1->init(map.get());
	p2->init(map.get());
	//While application is running
	while( !quit ){
	    //Handle events on queue
		while( SDL_PollEvent( &e ) != 0 ){
			//User requests quit
			if( e.type == SDL_QUIT ) quit = true;		
		}
		input_manager_->update();

        map->draw();
		p1->draw(mode);
		p2->draw(mode);
		//if(input_manager_->edge_key_p(player_type::p1, input_device::x) || input_manager_->edge_key_p(player_type::p1,input_device::up)) std::cout<<"up key pressed!\n";
		p1->move(*map);
		p2->move(*map);
		//Update screen
		SDL_RenderPresent( renderer );
		wait_game();
	}

    close();
    return 0;
}

bool init(){
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
		window = SDL_CreateWindow( "My Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen::width, screen::height, SDL_WINDOW_SHOWN );
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

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
