#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

namespace image {
    enum {
        p1 = 0,
        p2,
        bg,
        fullbg,
        count,
    };
}  

class ImageManager {
  std::vector<SDL_Surface *> all_images;
  SDL_Renderer *renderer_;

  inline void load(const char *path, const unsigned char image_type){
    SDL_Surface *image = IMG_Load(path);
    if (!image) {
      std::cerr << "error: " << IMG_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
    all_images[image_type] = image;
  }

 public:
  ImageManager(SDL_Renderer *renderer) noexcept : renderer_(renderer) {
    all_images.reserve(image::count);

    const int flag = IMG_INIT_PNG;
    if ((IMG_Init(flag) & flag) != flag) {
      std::cerr << "error: " << IMG_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    load("./data/cow.png", image::p1);
    load("./data/qq.png", image::p2);
    load("./data/9.png", image::bg);
    load("./data/r2.png", image::fullbg);
    // // load("./data/player2.png", image::p2);
    // load("./data/bg.png", image::bg);
    // load("./data/bg_red.png", image::bg_red);
    // load("./data/bg_green.png", image::bg_green);
    // load("./data/bg_blue.png", image::bg_blue);
    // load("./data/food.png", image::food);
    // load("./data/food_counter.png", image::food_counter);
    // load("./data/player1.png", image::akabei);
    // load("./data/player1.png", image::pinky);
    // load("./data/player1.png", image::aosuke);
    // //load("./data/guzuta.png", image::guzuta);
    // load("./data/mon_run.png", image::mon_run);
    // load("./data/plate.png", image::plate);
    // load("./data/ultron.png", image::ultron);
  }

  inline SDL_Texture *get(const unsigned char image_type) const{
    return SDL_CreateTextureFromSurface(renderer_, all_images[image_type]);
  }

  inline void render_copy(SDL_Texture &texture, const SDL_Rect &src, const SDL_Rect &dst) const {
    SDL_RenderCopy(renderer_, &texture, &src, &dst);
  }

  ~ImageManager(){ 
      atexit(IMG_Quit); 
    }
};
