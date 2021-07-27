#pragma once

#include <SDL2/SDL_ttf.h>
#include <iostream>


namespace font_size {
enum {
  x96,
  x64,
  x48,
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
    load("../shared/data/SF Atarian System Bold.ttf", 96, font_size::x96);
    load("../shared/data/SF Atarian System Bold.ttf", 48, font_size::x48);
    load("../shared/data/SF Atarian System Bold.ttf", 64, font_size::x64);
    load("../shared/data/SF Atarian System Bold.ttf", 36, font_size::x36);
    load("../shared/data/SF Atarian System Bold.ttf", 16, font_size::x16);
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