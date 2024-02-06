#include "Bishop.h"

namespace Chess
{
  bool Bishop::legal_move_shape(const Position& start, const Position& end) const {
    if(start == end){
      return false;
    }

    if((start.first - end.first) == (start.second - end.second) || (end.first - start.first) == (start.second - end.second)){
      return true;
    }
    else{
      return false;
    }
  }
}

