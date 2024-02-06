#include <cassert>
#include "Game.h"
#include <cctype>
#include <cmath>
#include <map>

using std::abs;
using std::tolower;

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Game::Game() : is_white_turn(true) {
    // Add the pawns
    for (int i = 0; i < 8; i++) {
      board.add_piece(Position('A' + i, '1' + 1), 'P');
      board.add_piece(Position('A' + i, '1' + 6), 'p');
    }

    // Add the rooks
    board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
    board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
    board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
    board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

    // Add the knights
    board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
    board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
    board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
    board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

    // Add the bishops
    board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
    board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
    board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
    board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

    // Add the kings and queens
    board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
    board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
    board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
    board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
  }

  //copy constructor
  Game::Game(const Game& g) {
    this->is_white_turn = g.is_white_turn;
    this->board.copyBoard(g.board);
  }
  
  //helper function to check if path is clear between two pieces on a given map
  bool Game::path_clear(const Position& start, const Position& end) const{
    std::map<Position, Piece*> pieces = board.getMap();
    char piece_type = tolower(pieces.at(start)->to_ascii());

    if(piece_type == 'q' || piece_type == 'r' || piece_type == 'b' || piece_type == 'p'){
      int x_direction = 0;
      int y_direction = 0;
      Position temp;

      if(end.first - start.first != 0){
	x_direction = (end.first - start.first)/abs(end.first - start.first);
      }

      if(end.second - start.second != 0){
	y_direction = (end.second - start.second)/abs(end.second - start.second);
      }

      for(int i = start.first + x_direction, j = start.second + y_direction; i != end.first || j != end.second; i += x_direction, j += y_direction){
	temp.first = (char)i;
	temp.second = (char)j;
	if(pieces.find(temp) != pieces.end()){
	  return false;
	}
      }
    }

    return true;
  }

  void Game::make_move(const Position& start, const Position& end) {

    //check start
    if (start.first < 'A' || start.first > 'H' || start.second < '1' || start.second > '8') {
      throw Exception("start position is not on board");
    }

    //check end
    if (end.first < 'A' || end.first > 'H' || end.second < '1' || end.second > '8') {
      throw Exception("end position is not on board");
    }

    std::map<Position, Piece*> pieces = board.getMap();

    //make sure piece is at start
    if (pieces.find(start) == pieces.end()) {
      throw Exception("no piece at start position");
    }


    //check piece color and turn color
    if (pieces.at(start)->is_white() != is_white_turn) {
      throw Exception("piece color and turn do not match");
    }


    //make sure move shape is legal
    if (pieces.find(end) == pieces.end()){
      if (!pieces.at(start)->legal_move_shape(start, end)) {
	throw Exception("illegal move shape");
      }
    }
    else{
      //check if capture shape is legal
      if (!pieces.at(start)->legal_capture_shape(start, end)) {
	throw Exception("illegal capture shape");
      }

      //if capturing then check captured piece
      if (pieces.at(start)->is_white() == pieces.at(end)->is_white()) {
	throw Exception("cannot capture own piece");
      }
    }

    //check if path is clear
    if(!path_clear(start, end)){
      throw Exception("path is not clear");
    }


    if (pieces.find(end) == pieces.end()){
      //make move
      char start_piece = pieces.at(start)->to_ascii();
      board.add_piece(end, start_piece);
      board.removePiece(start);

      //check if move exposes check, restore move if so
      if(this->in_check(is_white_turn)){
	board.removePiece(end);
	board.add_piece(start,start_piece);
	throw Exception("move exposes check");
      }

    }
    else{
      //make capture
      char start_piece = pieces.at(start)->to_ascii();
      char end_piece = pieces.at(end)->to_ascii();
      board.removePiece(end);
      board.add_piece(end, start_piece);
      board.removePiece(start);

      //check if move exposes check, restore move if so
      if(this->in_check(is_white_turn)){

	board.removePiece(end);
	board.add_piece(start,start_piece);
	board.add_piece(end, end_piece);
	throw Exception("move exposes check");
      }
    }
    
    //prommote pawns to queens
    if (board.getMap().find(end) != board.getMap().end()) {

      if (board.getMap().at(end)->to_ascii() == 'p' && end.second == '1') {
	board.removePiece(end);
	board.add_piece(end, 'q');
      }
      else if (board.getMap().at(end)->to_ascii() == 'P' && end.second == '8') {
	board.removePiece(end);
	board.add_piece(end, 'Q');
      }
    }
    
    if(is_white_turn){
      is_white_turn = false;
    }
    else{
      is_white_turn = true;
    }
  }

  bool Game::in_check (const bool& white) const {

    Position king;
    std::map<Position, Piece*> pieces = board.getMap();

    //find king pos
    for (std::map<Position, Piece*>::const_iterator it = pieces.cbegin(); it != pieces.cend(); ++it) {

      if ((it->second->to_ascii() == 'k' || it->second->to_ascii() == 'K') && it->second->is_white() == white) {

	king = it->first;
	break;
      }
    }

    //check if any piece can capture king's pos
    for (std::map<Position, Piece*>::const_iterator it = pieces.cbegin(); it != pieces.cend(); ++it) {

      //check opposing pieces
      if (it->second->is_white() != white) {
	if (it->second->legal_capture_shape(it->first, king)) {
	  if(path_clear(it->first, king)){
	    return true;
	  }
	}
      }
    }

    return false;
  }


  bool Game::in_mate(const bool& white) const {
    if(!in_check(white)){
      return false;
    }

    Game copy(*this);
    Position cur;
    std::map<Position, Piece*> pieces = copy.board.getMap();
    for (std::map<Position, Piece*>::const_iterator it = pieces.cbegin(); it != pieces.cend(); ++it) {
      for(char i = 'A'; i < 'I'; i++){
	for(char j = '1'; j < '9'; j++){
	  cur.first = i;
	  cur.second = j;
	  try{
	    copy.make_move(it->first, cur);
	    return false;
	  }
	  catch (const Exception &e){
	    continue;
	  }
	}
      }
    }

    return true;
  }


  bool Game::in_stalemate(const bool& white) const {
    if(in_check(white)){
      return false;
    }

    Game copy(*this);
    Position cur;
    std::map<Position, Piece*> pieces = copy.board.getMap();
    for (std::map<Position, Piece*>::const_iterator it = pieces.cbegin(); it != pieces.cend(); ++it) {
      for(char i = 'A'; i < 'I'; i++){
	for(char j = '1'; j < '9'; j++){
	  cur.first = i;
	  cur.second = j;
	  try{
	    copy.make_move(it->first, cur);
	    return false;
	  }
	  catch (const Exception &e){
	    continue;
	  }
	}
      }
    }

    return true;
  }

  
  std::istream& operator>> (std::istream& is, Game& game) {
    //check that istream is not empty
    if (is.eof()) {
      throw Exception("empty file");
    }

    //calling the clearBoard fuction to clear the board before loading a file
    game.board.clearBoard();
    //load in pieces
    for (char row = '8'; row >= '1'; row--) {
      std::string line;
      std::getline(is, line);

      //check if reached end of file
      if (is.eof()) {
	throw Exception("incomplete board");
      }
      int index = 0;
      for (char col = 'A'; col <= 'H'; col++) {

	if (line[index] != '-') {
	  game.board.add_piece(Position(col, row), line[index]);
	}
	index++;
      }
    }

    //get whos turn it is
    char temp;
    is.get(temp);
    if (temp == 'w') {
      game.is_white_turn = true;
    }
    else {
      game.is_white_turn = false;
    }

    return is;
  }

  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<< (std::ostream& os, const Game& game) {
    // Write the board out and then either the character 'w' or the character 'b',
    // depending on whose turn it is
    return os << game.board << (game.turn_white() ? 'w' : 'b');
  }
}
