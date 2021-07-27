
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "maze/maze.hpp"
#include "../shared/src/game.hpp"

#define  PORT "7788";


int main(int argc, char *argv[]){

	int sockfd, newsockfd, port_no, bindfd, listenfd, bytes_sent, bytes_recvd;
	char sbuffer[512], cli_ip[16], sname[64], cname[64]; 
	char *ptr_buff, *ptr_port;
	const char *ptr_cli_ip;
	struct sockaddr_in serv_addr, cli_addr;	
	socklen_t serv_size, cli_size;	

	int inp_true = 0, count = 0, inp, ni, x, y, toss;
	char serv_choice, cli_choice, nc;
	char choice_buffer[2], co_ordinates_buffer[2], toss_buffer;
	
	system("clear");
	ptr_buff = &sbuffer[0];
	ptr_port = (char *)&PORT;

	//creating sever side socket	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{	
		perror("Server side listening Socket could not be created!");
		return 1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));	
	port_no = atoi(ptr_port);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	//binding socket
	bindfd = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (bindfd == -1)
	{	
		perror("Failed to bind!");
		return 1;
	}
	
	//listening for incoming connections
	// std::cout<<"Enter your Name : ";
	// std::cin>>sname;
	sname = "server";
	std::cout<<"Server created!"<<std::endl<<"Waiting for a Player..."<<std::endl; 

	listenfd = listen(sockfd, 5);
	if (listenfd == -1){	
		perror("Failed to listen!");
		return 1;
	}

	serv_size = sizeof(serv_addr);
	cli_size = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_size);
		
	if (newsockfd == -1){	
		perror("Failed to accept from client!");
		return 1;
	}
		
	ptr_cli_ip = inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, cli_size);
	std::cout<<"Server received connections from "<<cli_ip<<std::endl;	

	memset(&cname, 0, sizeof(cname));
	do
	{	
		static int flag = 0;
		bytes_recvd = recv(newsockfd, &cname, sizeof(cname), 0);
		if (bytes_recvd == -1 && flag == 0)
		{
			memset(&cname, 0, sizeof(cname));
			std::cout<<"Could not ACQUIRE Player Information!"<<std::endl<<"Trying again..."<<std::endl; 
			continue;
		}
		else
		{	
			flag = 1;
			bytes_sent = send(newsockfd, &sname, sizeof(sname), 0);
			if (bytes_sent == -1)
				std::cout<<"Could not SEND Player Data!"<<"Trying Again..."<<std::endl; 
			else
				std::cout<<cname<<" has joined the game."<<std::endl;
		}
	} while(bytes_recvd == -1 || bytes_sent == -1);

	std::cout<<"Creating Game. Please wait..."<<std::endl;

	char map[512];
	MazeGenerator mg;
	std::string mapstr = mg.generate();
	strcpy(map,mapstr.c_str());

	do{	
		bytes_sent = send(newsockfd, &map, sizeof(map), 0);
		if (bytes_sent == -1)
				std::cout<<"Could not send Map Data!\n"<<"Trying Again..."<<std::endl; 
		else
			std::cout<<"Map data sent\n";
	} while(bytes_sent == -1);

	sleep(1);
	std::cout<<std::endl<<"Game created!"<<std::endl<<std::endl;


	
	//std::cout<<cname<<std::endl;
	//cout<<endl<<"Starting Game..."<<endl;

	Game game = Game();
	//game loop
    game.init();
	game.image_manager_ = std::make_unique<ImageManager>(game.renderer);
	game.input_manager_ = std::make_unique<InputManager>();
    game.map = std::make_unique<Map>(game.renderer,game.image_manager_.get(),mapstr);
    game.coin = std::make_unique<Coin>(game.image_manager_.get());
	game.p1 = std::make_unique<Player>(player_type::p1, game.renderer, game.image_manager_.get(), game.input_manager_.get());
	game.p2 = std::make_unique<Player>(player_type::p2, game.renderer, game.image_manager_.get(), game.input_manager_.get());
    SDL_ShowCursor(SDL_DISABLE);

        while(1){
            game.input_manager_->update();

            if (!game.poll_event()) break; //exit out of game loop
            
            switch (game.game_state_) {
            case Game::game_state::title:
                game.game_title();
			
				if(game.game_state_==Game::game_state::start){

					std::cout<<game.game_mode_<<std::endl;
					char gm = (char) game.game_mode_;
					//std::cout<<"gm is "<<std::to_string(gm)<<std::endl;
					//char gm = static_cast<char>(game.game_mode_);
					
						
					bytes_sent = send(newsockfd, &gm, sizeof(gm), 0);
					if (bytes_sent == -1)
						std::cout<<"Could not send game mode selected!\n"<<"Trying Again..."<<std::endl; 
					else{
						//std::cout<<std::to_string(gm)<<std::endl;
						std::cout<<"Game mode selection sent\n";
					}
							
				}

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
	
	std::cout<<"\nThank You for playing Treasure Raid!!!\n";
	close(newsockfd);
	close(sockfd);
	return 0;
}

