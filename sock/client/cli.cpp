#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <utility>
#include "../shared/src/game.hpp"


#define  PORT "7788"

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, port_no, n, connectfd, bytes_sent, bytes_recvd;
	char cbuffer[512], sname[64], cname[64];
	char *ptr = &cbuffer[0];
	char *ptr_port = (char *)&PORT;
	struct sockaddr_in serv_addr;
	struct hostent *he;
	
	int count = 0, inp, x, y, ni, inp_true = 0, toss;
	char serv_choice, cli_choice, nc;
	char choice_buffer[2], co_ordinates_buffer[2], toss_buffer;

	system("clear");

	if (argc != 2){
		perror("Incomplete arguments!");
		return 1;
	}
	port_no = atoi(ptr_port);
	he = gethostbyname(argv[1]);
	if (he == NULL){
		perror("No Such Host!");
		return 1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//make socket non-blocking??
	if(sockfd == -1){	
		perror("Sorry. Socket could not be created!");
		return 1;
	}

    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr = *((struct in_addr *)he->h_addr);  

	connectfd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (connectfd == -1){	
		perror("Sorry. Could not connect to server.");
		return 1;
	}
		
	std::cout<<"Enter your name : ";
	std::cin>>cname;
	do{
		static int flag = 0;
		bytes_sent = send(sockfd, &cname, sizeof(cname), 0);
		if (bytes_sent == -1 && flag == 0)
		{
			std::cout<<"PLAYER DATA NOT SENT!"<<std::endl<<"Trying Again...";  
			continue;
		}
		else
		{		
			flag = 1;
			memset(&sname, 0, sizeof(sname));
			bytes_recvd = recv(sockfd, &sname, sizeof(sname), 0);
			if (bytes_recvd == -1)
				std::cout<<"COULD NOT ACQUIRE PLAYER INFORMATION!"<<std::endl<<"Trying Again..."<<std::endl; 
			else
				std::cout<<"You have joined "<<sname<<" for the game"<<std::endl;
		}
	}while(bytes_sent == -1 || bytes_recvd == -1);
	
	std::cout<<"Creating game. Please wait..."<<std::endl;

	char map[512];
	do{
		memset(&map, 0, sizeof(map));
		bytes_recvd = recv(sockfd, &map, sizeof(map), 0);
		if (bytes_recvd == -1)
			std::cout<<"COULD NOT ACQUIRE MAP DATA!"<<std::endl<<"Trying Again..."<<std::endl; 
		else
			std::cout<<"Map data recieved"<<std::endl;
	}while(bytes_recvd == -1);

	sleep(1);
	std::cout<<std::endl<<"Game created!"<<std::endl<<std::endl;
	std::string pp(map);
	
	char gm;
	memset(&gm, 0, sizeof(gm));
	bytes_recvd = recv(sockfd, &gm, sizeof(gm), 0);
	if (bytes_recvd == -1)
		std::cout<<"COULD NOT ACQUIRE GAME MODE DATA!"<<std::endl<<"Trying Again..."<<std::endl; 
	else{
		//std::string gm_(gm);
		std::cout<<"Game mode data recieved"<<std::endl;
	}

	std::string gm_ = std::to_string(gm);
	int tmp = std::stoi(gm_);
	std::cout<<gm_<<" "<<tmp<<std::endl;

	if(tmp==0){
		std::cout<<"Single Player Mode selected...\n";
		return 0;
	}

	Game game = Game();	
	game.init();
	game.image_manager_ = std::make_unique<ImageManager>(game.renderer);
	game.input_manager_ = std::make_unique<InputManager>();
    game.map = std::make_unique<Map>(game.renderer,game.image_manager_.get(),pp);
    game.coin = std::make_unique<Coin>(game.image_manager_.get());
	game.p1 = std::make_unique<Player>(player_type::p1, game.renderer, game.image_manager_.get(), game.input_manager_.get());
	game.p2 = std::make_unique<Player>(player_type::p2, game.renderer, game.image_manager_.get(), game.input_manager_.get());
    //SDL_ShowCursor(SDL_DISABLE);

	
	game.game_mode_= tmp;
	//std::cout<<game.game_mode_<<std::endl;
	if(tmp==0){
		std::cout<<"Single Player Mode selected...\n";
		return 0;
	}

	game.map->init();
    game.coin->init(game.map.get(), game.game_mode_);
    //enemy_->init();
    game.p1->init(game.map.get());
    game.p2->init(game.map.get());
    game.p1->set_life(2);
    game.p2->set_life(2);
    game.p1->set_score(0);
    game.p2->set_score(0);
    game.p1->set_damaged(false);
    game.p2->set_damaged(false);
    //p1_->set_power_mode(0);
    //p2_->set_power_mode(0);

    game.game_count_ = 0;
    game.game_state_ = Game::game_state::start;
    
    srand((unsigned int)time(nullptr));

	//game loop
        while(1){
            game.input_manager_->update();

            if (!game.poll_event()) break; //exit out of game loop
            
            switch (game.game_state_) {
            case Game::game_state::title:
                //game.game_title();
				//game.text()
                break;
            case Game::game_state::start:
                game.game_start();
                break;
            case Game::game_state::miss:
                game.game_miss();
                break;
            case Game::game_state::playing:
                game.play_game();
                break;
            case Game::game_state::win:
                game.game_win();
                break;
            case Game::game_state::gameover:
                game.game_over();
                break;
            case Game::game_state::pause:
                game.game_pause();
                break;
            }

		    //Update screen
	    	SDL_RenderPresent(game.renderer);
		    game.wait_game();

		}

	std::cout<<std::endl<<"Thank You for playing Treasure Raid!!!"<<std::endl;
	close(sockfd);
	return 0;
}
	

