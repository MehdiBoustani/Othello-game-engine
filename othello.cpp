/**
 * INFO0004-2: - Object-oriented programming projects
 *  Project 1: Othello Game Engine
 * 
 * \file othello.cpp
 * \brief Implementation of Othello class
 * \author Boustani Mehdi (s221594)
 * \date 11/04/2025
 * 
 */

#include "othello.h"
#include <stdexcept>
#include <sstream>

// We use this namespace to avoid writing a lot "std::"
using namespace std;

// Initialize the directions array
const int Othello::directions[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
};

/**
 * Constructor for the Othello class
 * Initializes the board with empty spaces and sets up the initial pieces
 */
Othello::Othello() {

    // Initialize 8x8 board with empty spaces
    board = vector<vector<Colour>>(GRID_SIZE, vector<Colour>(GRID_SIZE, EMPTY));

    // Set up initial pieces using POSITION
    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;

    // BLACK always start playing first
    activePlayer = BLACK;
}

/**
 * Parameterized constructor for the Othello class
 * Initializes the board with a given board and player as parameters
 * 
 * Parameters:
 * - board: the board to initialize the game with
 * - player: the player to initialize the game with
 * 
 * Exceptions:
 * - length_error if the board is not 8x8
 * - invalid_argument if the player is not WHITE or BLACK
 */
Othello::Othello(const vector<vector<Colour>>& board, Colour player){

    // Check if the board is 8x8 (rows)
    if(board.size() != GRID_SIZE){
        throw length_error("Othello board must have " + to_string(GRID_SIZE) + " rows");
    }

    // Check if the board is 8x8 (columns)
    for (size_t i = 0; i < board.size(); ++i){
        if(board[i].size() != GRID_SIZE){
            throw length_error("Othello board must have " + to_string(GRID_SIZE) + " columns");
        }
    }

    // Check if the player is valid
    if(player != WHITE && player != BLACK){
        throw invalid_argument("Player must be WHITE or BLACK");
    }

    // Initialize the board
    this->board = board;

    // Initialize the player
    this->activePlayer = player;
}

/**
 * Get the current active player
 * 
 * Returns the color of the current player, EMPTY if nobody can play
 * 
 */
Othello::Colour Othello::getActivePlayer() {
    // Check if current player has any moves
    if (!getMoves().empty()) {
        return activePlayer;
    }
    
    // If current player has no moves, check if opponent has moves
    activePlayer = (activePlayer == BLACK) ? WHITE : BLACK;

    // If opponent has moves, return the opponent
    if (!getMoves().empty()) {
        return activePlayer;
    }
    
    // If neither player has moves, return EMPTY
    return EMPTY;
}

/**
 * Get the available moves for a given player
 * 
 * Parameters:
 * - player: the color of the player to get the available moves for
 * 
 * Returns:
 * - a set of positions where the player can play
 * 
 */
set<Othello::POSITION> Othello::getAvailableMoves(Colour player) {

    set<POSITION> moves;
    
    // Save the original player
    Colour originalPlayer = activePlayer;

    // Set the current player
    activePlayer = player;

    // Check all positions on the board
    for(size_t i = 0; i < GRID_SIZE; ++i) {
        for(size_t j = 0; j < GRID_SIZE; ++j) {
            
            POSITION pos = make_pair(i, j);

            if(value(pos) == EMPTY) {

                if(isValidMove(pos)) {

                    // Insert the position into the set
                    moves.insert(pos);
                }
            }
        }
    }

    // Restore the original player
    activePlayer = originalPlayer;
    
    return moves;
}

/**
 * Check if the game is over
 * 
 * Returns true if the game is over, false otherwise
 * 
 */
bool Othello::isGameOver() {

    // Save the original player
    Colour originalPlayer = activePlayer;
    
    // Check for BLACK
    activePlayer = BLACK;
    bool hasBlackMoves = !getAvailableMoves(activePlayer).empty();
    
    // Check for WHITE
    activePlayer = WHITE;
    bool hasWhiteMoves = !getAvailableMoves(activePlayer).empty();
    
    // Restore original player
    activePlayer = originalPlayer;
    
    // Return true if both players have no moves
    return !hasBlackMoves && !hasWhiteMoves;
}

/**
 * Get the winner of the game
 * 
 * Returns the winner of the game, EMPTY if the game is not finished
 * 
 */
Othello::Colour Othello::getWinner(){

    // Return EMPTY if the game is not finished
    if(!isGameOver()) return EMPTY;

    // Count the number of pieces of each color
    unsigned int blackCount = 0;
    unsigned int whiteCount = 0;

    // Count the number of pieces of each color
    for(size_t i = 0; i < GRID_SIZE; ++i){
        for(size_t j = 0; j < GRID_SIZE; ++j){

            POSITION pos = make_pair(i, j);
            
            if(value(pos) == BLACK){
                blackCount++;
            }
            
            else if(value(pos) == WHITE){
                whiteCount++;
            }
        }
    }

    // Return the winner based on the number of pieces of each color
    return (blackCount > whiteCount) ? BLACK :
       (whiteCount > blackCount) ? WHITE : EMPTY;

}

/**
 * Get the available moves for the current player
 * 
 * Returns a set of positions where the current player can play, empty set if game is finished
 * 
 */
set<Othello::POSITION> Othello::getMoves() {

    // Return empty set if game is finished
    if (isGameOver()) {
        return set<POSITION>();
    }

    // Return the available moves for the current player
    return getAvailableMoves(activePlayer);
}

/**
 * Flip the disc in a given direction
 * 
 * Parameters:
 * - pos: the position of the disc to flip
 * - x: the x direction to flip the disc
 * - y: the y direction to flip the disc
 */
void Othello::flipDisc(POSITION pos, int x, int y) {

    // Get the current position
    int row = pos.first + x;
    int col = pos.second + y;
    
    POSITION currentPosition = make_pair(row, col);
    
    // List of pieces to flip
    vector<POSITION> piecesToFlip;

    // While we are on the board
    while(isInGrid(currentPosition)) {

        Colour cellValue = value(currentPosition);

        if(cellValue == EMPTY) break;
        
        if(cellValue == activePlayer) {

            // Flip all pieces in this direction
            for(const auto& piece : piecesToFlip) {
                board[piece.first][piece.second] = activePlayer;
            }

            break;
        }
        
        // Add the current position to the list of pieces to flip (at the end)
        piecesToFlip.push_back(currentPosition);

        // Move to the next position
        currentPosition.first += x;
        currentPosition.second += y;
    }
}

/**
 * Play a disc at a given position and update the game status
 * 
 * Parameters:
 * - pos: the position of the disc to play
 * 
 * Returns:
 * - true if the move is valid, false otherwise
 */
bool Othello::play(POSITION pos) {

    // Check if position is within grid
    if(!isInGrid(pos)) {
        return false;
    }

    set<POSITION> moves = getMoves();
    
    // The find method return the end if it is not in the set
    if(moves.find(pos) == moves.end()) {
        return false;
    }

    // Place the piece
    board[pos.first][pos.second] = activePlayer;

    // Flip pieces in all directions
    for(int i = 0; i < 8; ++i) {
        flipDisc(pos, directions[i][0], directions[i][1]);
    }

    // Switch player
    activePlayer = (activePlayer == BLACK) ? WHITE : BLACK;
    
    return true;
}

/**
 * Get the value (colour) of a given position
 * 
 * Parameters:
 * - pos: the position to get the value of
 * 
 * Returns:
 * - the value (colour) of the position
 */
Othello::Colour Othello::value(POSITION pos){

    // Check if the position is within the grid
    if(!isInGrid(pos)){
        throw out_of_range("Position out of bounds");
    }

    return board[pos.first][pos.second];
}

/**
 * Check if a given position is a valid move
 * 
 * Parameters:
 * - pos: the position to check
 * 
 * Returns:
 * - true if the position is a valid move, false otherwise
 */
bool Othello::isValidMove(POSITION pos){

    // Check if the position is empty
    if(value(pos) != EMPTY) {
        return false;
    }

    // Check if the position is a valid move (by checking all directions)
    for(int i = 0; i < 8; ++i) {
        int row = pos.first + directions[i][0];
        int col = pos.second + directions[i][1];

        POSITION currentPosition = make_pair(row, col);

        // Check if the opponent is found
        bool foundOpponent = false;

        // While we are on the limit of the board
        while(isInGrid(currentPosition)){

            Colour cellValue = value(currentPosition);
        
            if(cellValue == EMPTY) break;
            
            if(cellValue == activePlayer){

                if(foundOpponent) return true;
            
                break;
            }
            
            foundOpponent = true;

            // Move to the next position
            currentPosition.first += directions[i][0];
            currentPosition.second += directions[i][1];
        }
    }
    
    return false;
}

/**
 * Check if a given position is within the grid of the board
 * 
 * Parameters:
 * - pos: the position to check
 * 
 * Returns:
 * - true if the position is within the grid, false otherwise
 */
bool Othello::isInGrid(POSITION pos) {

    return pos.first >= 0 && pos.first < GRID_SIZE && 
           pos.second >= 0 && pos.second < GRID_SIZE;
}