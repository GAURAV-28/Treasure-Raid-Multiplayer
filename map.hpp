#include <string>
#include <iomanip>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int x_count = 21;
const int y_count = 21;
const int size = 40;

//Screen dimension constants
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 840;

enum class map_state {
  block, 
  path,
  coin,
  player
};

class Map{

    SDL_Renderer *renderer;
    map_state block[y_count][x_count];

    public:
      Map(SDL_Renderer *renderer) : renderer(renderer) {}
            
      void init();
      void draw();

    //  inline map_state check_state(const Point &p) const noexcept {
    //     return block_[p.y][p.x];
    //   }

    //   inline map_state check_state(const Point &&p) const noexcept {
    //     return check_state(p);
    //   }
};