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
        crown,
        coin,
        up,
        down,
        left,
        right,
        a,
        s,
        w,
        d,
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

    load("../shared/data/cow.png", image::p1);
    load("../shared/data/qq.png", image::p2);
    load("../shared/data/9.png", image::bg);
    load("../shared/data/r2.png", image::fullbg);
    load("../shared/data/crown.png", image::crown);
    load("../shared/data/r.png", image::coin);
    load("../shared/data/down.png", image::down);
    load("../shared/data/up.png", image::up);
    load("../shared/data/left.png", image::left);
    load("../shared/data/right.png", image::right);
    load("../shared/data/a.png", image::a);
    load("../shared/data/s.png", image::s);
    load("../shared/data/w.png", image::w);
    load("../shared/data/d.png", image::d);
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
