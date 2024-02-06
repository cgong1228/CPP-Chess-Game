#include "Rook.h"

namespace Chess
{
  //straight lines only
  bool Rook::legal_move_shape(const Position& start, const Position& end) const {

    //check end != start
    if (end == start) {
      return false;
    }
    
    //straight if same col or same row (not both)
    if (end.first == start.first || end.second == start.second) {
      return true;
    }

    //move is not straight line
    return false;
  }
}
