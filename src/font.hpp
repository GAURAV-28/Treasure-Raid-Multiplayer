#pragma once

#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace font_size {
enum {
  x64 =0,
  x36,
  x16,
  count,
};
}

class Font{
  TTF_Font *fonts[font_size::count];

  inline void load(const char *path, unsigned int size,const unsigned char font_size){
    fonts[font_size] = TTF_OpenFont(path, size);
    if (!fonts[font_size]) {
      std::cerr << "error: " << TTF_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
  }

 public:
  Font(){
    if (TTF_Init() == -1) {
      std::cerr << "error: " << TTF_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
    load("./data/GenEiGothicP-Heavy.otf", 64, font_size::x64);
    load("./data/GenEiGothicP-Heavy.otf", 36, font_size::x36);
    load("./data/GenEiGothicP-Regular.otf", 16, font_size::x16);
  }

  inline TTF_Font *get(const unsigned char font_size) const{
    return fonts[font_size];
  }

  ~Font(){
    for (const auto &font : fonts) {
      TTF_CloseFont(font);
    }
    atexit(TTF_Quit);
  }
};