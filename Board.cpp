#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Board::Board(){}

  //destructor
  Board::~Board() {
    clearBoard();
  }

  //frees and removes piece from map
  void Board::removePiece(Position p) {
    Piece* piece;
    if (occ.find(p) != occ.end()) {
      piece = occ.at(p);
    }
    delete piece;
    piece = nullptr;
    occ.erase(p);
  }

  const Piece* Board::operator()(const Position& position) const {

    //check if it exists
    if (occ.find(position) != occ.end()) {
      return occ.at(position);
    }

    return NULL;
  }

  void Board::clearBoard() {
    //frees memory and clears map
    for(std::map<Position, Piece*>::iterator it = occ.begin(); it != occ.end(); ++it) {
      delete it->second;
    }
    occ.clear();
  }

  void Board::copyBoard(const Board& b) {
    //copies another board
    for(std::map<Position, Piece*>::const_iterator it = b.occ.cbegin(); it != b.occ.cend(); ++it) {
      this->add_piece(it->first, it->second->to_ascii());
    }
  }

  void Board::add_piece(const Position& position, const char& piece_designator) {

    //check piece designator
    char temp = std::tolower(piece_designator);
    if (temp != 'k' && temp != 'q' && temp != 'r' && temp != 'b' && temp != 'n' && temp != 'p' && temp != 'm') {
      throw Exception("invalid designator\n");
    }
    //check position
    if (position.first < 'A' || position.first > 'H' || position.second < '1' || position.second > '8') {
      throw Exception("invalid position\n");
    }
    //check if position is occupied
    if (occ.find(position) != occ.end()) {
      throw Exception("position occupied\n");
    }

    //add piece
    occ[position] = create_piece(piece_designator);
  }

  void Board::display() const {

    //red for white pieces
    //blue for black pieces

    std::cout << " |---------------|\n";
    //iterate through entire board
    for(char row = '8'; row >= '1'; row--) {
      std::cout << row << "|";
      for(char col = 'A'; col <= 'H'; col++) {

	//check for piece
	if (occ.find(Position(col, row)) != occ.end()) {

	  //check if white
	  if (occ.at(Position(col, row))->is_white()) {
	    Terminal::color_fg(true, Terminal::RED);
	    std::cout << occ.at(Position(col, row))->to_ascii();
	    Terminal::set_default();
	    std::cout << "|";
	  }
	  else {
	    Terminal::color_fg(true, Terminal::BLUE);
	    std::cout << occ.at(Position(col,row))->to_ascii();
	    Terminal::set_default();
	    std::cout << "|";
	  }
	}
	else {
	  std::cout << " |";
	}
      }
      std::cout << "\n |---------------|\n";
    }

    std::cout << "  A B C D E F G H\n";
  }

  bool Board::has_valid_kings() const {

    //iterate through map and look for kings (must be 2)
    int count = 0;
    for (std::map<Position, Piece*>::const_iterator it = occ.begin(); it != occ.end(); ++it) {
      if (it->second->to_ascii() == 'K' || it->second->to_ascii() == 'k') {
	count++;
      }
      if (count > 2) {
	return false;
      }
    }

    return count == 2;
  }

  //removes the pieces of the current board



  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }
}
