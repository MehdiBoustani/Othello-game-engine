/**
 * INFO0004-2: - Object-oriented programming projects
 *  Project 1: Othello Game Engine
 * 
 * \file othello.h
 * \brief Header file for Othello
 * \author Boustani Mehdi (s221594)
 * \date 11/04/2025
 * 
 */

#include <set>
#include <vector>
#include <utility>
#include <stdexcept>
using namespace std;

class Othello {
      public:
         /* used to represent the (row, column) position on the board
            pair.first is row (0-7), pair.second is column (0-7)*/
         typedef pair<int, int> POSITION;
         enum Colour {EMPTY, WHITE, BLACK};

         /* creates a new, default game */
         Othello();
         
         /* board must represent a 2-dimensional "array" of 8-by-8
            player must be Colour::WHITE or Colour::BLACK, otherwise invalid_argument is thrown
            throws length_error if board is not at least 8-by-8 */
         Othello(const vector<vector<Colour>>& board, Colour player);

         /* returns colour of player whose turn it is; Colour::EMPTY if nobody can play */
         Colour getActivePlayer();

         bool isGameOver();

         /* returns Colour::EMPTY if game is not finished */
         Colour getWinner();

         /* returns all available moves to the active player; empty set if game is finished */
         set<POSITION> getMoves();

         /* plays disc of active player at pos, and updates game status; 
            returns true on success, false if move not allowed */
         bool play(POSITION pos);

         /* returns colour at pos */
         Colour value(POSITION pos);

      private:
         // Board size constant (8x8)
         static constexpr int GRID_SIZE = 8; // Using constexpr to ensure compile-time constant

         // Current board state
         vector<vector<Colour>> board;

         // Player currently playing
         Colour activePlayer;

         // Helper method to check if a position is within bounds of the board
         bool isInGrid(POSITION pos);

         // Helper method to check if a move is valid
         bool isValidMove(POSITION pos);

         // Helper method to flip discs in a given direction
         void flipDisc(POSITION pos, int x, int y);

         // The 8 possible directions for moves
         static constexpr int directions[8][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
         };

         // Get all available moves of the current player
         set<POSITION> getAvailableMoves(Colour player);
};