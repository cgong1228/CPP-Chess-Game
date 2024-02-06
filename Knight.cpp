#include "Knight.h"
#include <cmath>

using std::abs;

namespace Chess
{
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {
    double x_distance = abs(start.first-end.first);
    double y_distance = abs(start.second - end.second);
    if((x_distance == 1 && y_distance == 2) || (x_distance == 2 && y_distance == 1)){
      return true;
    }

    return false;
  }
}
