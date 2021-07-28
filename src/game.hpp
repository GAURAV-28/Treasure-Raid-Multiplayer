#pragma once
//#define ENET_IMPLEMENTATION
#include "enet.h"
#include <time.h>
#include <iomanip>
#include <iostream>
#include <string>
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
#include "enemy.hpp"

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
    std::unique_ptr<SoundManager> sound_manager_;
    Font font_;
    std::unique_ptr<Map> map;
    std::unique_ptr<Coin> coin;
    std::unique_ptr<Enemy> enemy;
    std::unique_ptr<Player> p1;
    std::unique_ptr<Player> p2;
    ENetAddress address;
    ENetHost* server;
    ENetEvent event;
    ENetPeer* peer1;
    int testcount;
    bool testf;


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
        sound_manager_ = std::make_unique<SoundManager>();
        address = {0};
        address.host = ENET_HOST_ANY; /* Bind the server to the default localhost.     */
        address.port = 8123; /* Bind the server to port 7777. */
        server = enet_host_create(&address, 3, 2, 0, 0);
        if (server == NULL) {
            printf("An error occurred while trying to create an ENet server host.\n");
            //return 1;
        }
        printf("Started a server...\n");
        map = std::make_unique<Map>(renderer,image_manager_.get());
        coin = std::make_unique<Coin>(image_manager_.get(),sound_manager_.get());
	    p1 = std::make_unique<Player>(player_type::p1, renderer, image_manager_.get(), input_manager_.get(),sound_manager_.get());
	    p2 = std::make_unique<Player>(player_type::p2, renderer, image_manager_.get(), input_manager_.get(),sound_manager_.get());
        enemy = std::make_unique<Enemy>(renderer, image_manager_.get(),sound_manager_.get());
        SDL_ShowCursor(SDL_DISABLE);
        testcount = 1;
        testf = false;
    }
        int byteToInt(unsigned char* byte) {

        int n = 0;

        n = n + (byte[0] & 0x000000ff);
        n = n + ((byte[1] & 0x000000ff) << 8);
        n = n + ((byte[2] & 0x000000ff) << 16);
        n = n + ((byte[3] & 0x000000ff) << 24);


        return n;
    }

    void intToByte(int n, unsigned char* result) {

        result[0] = n & 0x000000ff;
        result[1] = (n & 0x0000ff00) >> 8;
        result[2] = (n & 0x00ff0000) >> 16;
        result[3] = (n & 0xff000000) >> 24; 
    }

    inline void decode(ENetPacket* packet){
        if(packet->data[0]=='P'){
           p2->set_pos((Point) {byteToInt(&packet->data[1]), byteToInt(&packet->data[5])}) ;
           if(packet->data[9]=='1'){
               if(p2->power_up>0){
                Mix_PlayChannel(se_type::siren, sound_manager_->get_se(se_type::siren),0);
                //tobesent[9]='1';
                p1->power_mode = 400;
                p2->power_up--;
                }
           }
        }
    }

    inline void play(){
        //game loop
        while(1){
            input_manager_->update();
            while (enet_host_service(server, &event, 0) > 0) {
                switch (event.type) {
                    case ENET_EVENT_TYPE_CONNECT:
                        printf("A new client connected from %x:%u.\n",  event.peer->address.host, event.peer->address.port);
                        /* Store any relevant client information here. */
                        //event.peer->data = "Client information";
                        peer1 = event.peer;
                        testf = true;
                        break;

                    case ENET_EVENT_TYPE_RECEIVE:
                        printf("A packet of length %lu containing %s was received from %s on channel %u.\n",
                                event.packet->dataLength,
                                event.packet->data,
                                event.peer->data,
                                event.channelID);
                        /* Clean up the packet now that we're done using it. */
                        decode(event.packet);
                        enet_packet_destroy (event.packet);
                        break;

                    case ENET_EVENT_TYPE_DISCONNECT:
                        printf("%s disconnected.\n", event.peer->data);
                        /* Reset the peer's client information. */
                        event.peer->data = NULL;
                        break;

                    case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                        printf("%s disconnected due to timeout.\n", event.peer->data);
                        /* Reset the peer's client information. */
                        event.peer->data = NULL;
                        break;

                    case ENET_EVENT_TYPE_NONE:
                        break;
                }
            }
            //if(testf){
            //ENetPacket * packet = enet_packet_create ("packet", 
             //                             strlen ("packet") + 1 ,0);
            ///testcount +=2;
            //testcount  =testcount%10;
            //enet_peer_send (peer1, 0, packet);}

            if (!poll_event()){
                enet_host_destroy(server);
                return; //exit out of game loop
            } 
            
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
