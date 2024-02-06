#include "King.h"
#include <cmath>

namespace Chess
{
  bool King::legal_move_shape(const Position& start, const Position& end) const {
    
    int distanceR = abs(start.second - end.second);
    int distanceC = abs(start.first - end.first);

    if (start == end){
      return false;
    }
    
    //if king moves up or down, cols is zero
    //if king moves left or right, rows  is zero
    if (distanceR <= 1 && distanceC <=1){
      return true;
    } 

    return false;
  }
}
