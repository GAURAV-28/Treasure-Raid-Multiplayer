#pragma once

#include <SDL2/SDL.h>
#include <math.h>

namespace screen {

enum {
  width = 840,
  height = 840,
};

}  // namespace screen

namespace block {

enum {
  size = 40,
  x_count = 21,
  y_count = 21,
};

}  // namespace block

// enum class game_mode {
//   single,
//   battle,
// };

struct Point {
  int x;
  int y;

public:

  inline bool operator==(const Point &rhs) const noexcept {
    return x == rhs.x && y == rhs.y;
  }

  inline bool operator!=(const Point &rhs) const noexcept {
    return x != rhs.x || y != rhs.y;
  }

  inline Point operator+(const Point &rhs) const noexcept {
    return Point{x + rhs.x, y + rhs.y};
  }

  inline void operator+=(const Point &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
  }

  inline unsigned int distance(const Point &rhs) const noexcept {
    return static_cast<unsigned int>(
        hypot(static_cast<double>(x - rhs.x), static_cast<double>(y - rhs.y)));
  }
};
