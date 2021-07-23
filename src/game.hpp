#pragma once

#include <time.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include "font.hpp"
#include "coins.hpp"
#include "input.hpp"
#include "image.hpp"
#include "map.hpp"
#include "global.hpp"
//#include "mixer_manager.hpp"
#include "player.hpp"

//pixel definition
struct RGB {
  Uint8 red;
  Uint8 green;
  Uint8 blue;
};

//standard colors
namespace rgb {
const RGB black =   RGB{0x00, 0x00, 0x00};
const RGB red   =   RGB{0xff, 0x00, 0x00};
const RGB white =   RGB{0xff, 0xff, 0xff};
const RGB tc    =   RGB({0xff,0xc9,0x47});
const RGB blink =   RGB({0xdf, 0x76, 0x0b});
const RGB op2   =   RGB({0xde,0xff,0xff});
const RGB foot  =   RGB{0xff,0xeb,0xaf};
}

//game class
class Game{
    //game states
    enum class game_state {
        title,
        start,
        win,
        miss,
        playing,
        gameover,
        pause,
    };

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    game_state game_state_;
    //unsigned int game_level_;
    //game_mode game_mode_;
    unsigned int game_mode_;
    unsigned int blink_count_;
    unsigned int game_count_;
    unsigned int menu_option_;

    //pointers to all components of the game
    std::unique_ptr<ImageManager> image_manager_;
    std::unique_ptr<InputManager> input_manager_;
    Font font_;
    std::unique_ptr<Map> map;
    std::unique_ptr<Coin> coin;
    //std::unique_ptr<Enemy> enemy;
    std::unique_ptr<Player> p1;
    std::unique_ptr<Player> p2;

    //game functions
    void game_title();  //game_title.cpp
    void game_start();  //game_start.cpp
    void play_game();   //play_game.cpp
    void game_miss();   //game_over.cpp
    void game_over();   //game_over.cpp
    void game_win();    //game.cpp
    void game_pause();  //game.cpp

    //text and score functions
    void text(const unsigned char font_size, const RGB &rgb, const Point &p, const char *str) const; //game.cpp
    void score() const;  //score.cpp
    void translucence(); //game.cpp
    
    //initialization functions
    bool init();            //Starts up SDL and creates window (game.cpp)
    inline void close(){    //Frees media and shuts down SDL
	    //Destroy window	
	    SDL_DestroyRenderer( renderer );
	    SDL_DestroyWindow( window );
	    window = NULL;
	    renderer = NULL;
	    //Quit SDL subsystems
	    IMG_Quit();
	    SDL_Quit();
    }  

    //to wait for the game to render all features
    inline void wait_game(){
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

    //if the user presses escape or exit button
    inline bool poll_event() const {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return false;
                }
                break;
            default: //do nothing
            break;
        }
    }
    return true;
  }

  public:
    Game():
        window(nullptr),
        renderer(nullptr),
        game_state_(game_state::title),
        game_mode_(0),
        blink_count_(0),
        game_count_(0),
        menu_option_(0)
    {
        init();
	    image_manager_ = std::make_unique<ImageManager>(renderer);
	    input_manager_ = std::make_unique<InputManager>();
        map = std::make_unique<Map>(renderer,image_manager_.get());
        coin = std::make_unique<Coin>(image_manager_.get());
	    p1 = std::make_unique<Player>(player_type::p1, renderer, image_manager_.get(), input_manager_.get());
	    p2 = std::make_unique<Player>(player_type::p2, renderer, image_manager_.get(), input_manager_.get());

        SDL_ShowCursor(SDL_DISABLE);
    }

    inline void play(){
        //game loop
        while(1){
            input_manager_->update();

            if (!poll_event()) return; //exit out of game loop
            
            switch (game_state_) {
            case game_state::title:
                game_title();
                break;
            case game_state::start:
                game_start();
                break;
            case game_state::miss:
                game_miss();
                break;
            case game_state::playing:
                play_game();
                break;
            case game_state::win:
                game_win();
                break;
            case game_state::gameover:
                game_over();
                break;
            case game_state::pause:
                game_pause();
                break;
            }

		    //Update screen
	    	SDL_RenderPresent(renderer);
		    wait_game();
        }
    }

    ~Game(){
        close();
    }

};
