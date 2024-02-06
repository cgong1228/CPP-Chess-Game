#include "Pawn.h"
#include <cmath>

using std::abs;

namespace Chess
{
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
    if(start == end){
        return false;
    }

    if(is_white()){
        if(start.second == '2'){
            if((end.second - start.second == 1 || end.second - start.second == 2) && start.first == end.first){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            if(end.second - start.second == 1 && start.first == end.first){
                return true;
            }
            else{
                return false;
            }
        }
    }
    else{
        if(start.second == '7'){
             if((start.second - end.second == 1 || start.second - end.second == 2) && start.first == end.first){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            if(start.second - end.second == 1 && start.first == end.first){
                return true;
            }
            else{
                return false;
            }
        }
    }                                                                                                                                                                                     
  }

  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const {
    if(is_white()){
        if(abs(start.first - end.first) == 1 && (end.second - start.second) == 1){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(abs(start.first - end.first) == 1 && (start.second - end.second) == 1){
            return true;
        }
        else{
            return false;
        }
    }
  }
}
