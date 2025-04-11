#include "othello.h"
#include <stdexcept>
#include <sstream>

using namespace std;

// Initialize static member
const int Othello::directions[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
};

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

Othello::Othello(const vector<vector<Colour>>& board, Colour player){
    if(board.size() != GRID_SIZE){
        throw length_error("Othello board must have " + to_string(GRID_SIZE) + " rows");
    }

    for (size_t i = 0; i < board.size(); ++i){
        if(board[i].size() != GRID_SIZE){
            throw length_error("Othello board must have" + to_string(GRID_SIZE) + " columns");
        }
    }

    if(player != WHITE && player != BLACK){
        throw invalid_argument("Player must be WHITE or BLACK");
    }

    this->board = board;

    this->activePlayer = player;
}

Othello::Colour Othello::getActivePlayer() {
    // Check if current player has any moves
    if (!getMoves().empty()) {
        return activePlayer;
    }
    
    // If current player has no moves, check if opponent has moves
    activePlayer = (activePlayer == BLACK) ? WHITE : BLACK;
    if (!getMoves().empty()) {
        return activePlayer;
    }
    
    // If neither player has moves, return EMPTY
    return EMPTY;
}

bool Othello::isGameOver() {
    // Check if both players have no moves
    Colour original = activePlayer;
    
    // Check for BLACK
    activePlayer = BLACK;
    bool hasBlackMoves = !getAvailableMoves(activePlayer).empty();
    
    // Check for WHITE
    activePlayer = WHITE;
    bool hasWhiteMoves = !getAvailableMoves(activePlayer).empty();
    
    // Restore original player
    activePlayer = original;
    
    return !hasBlackMoves && !hasWhiteMoves;
}

Othello::Colour Othello::getWinner(){
    if(!isGameOver()) return EMPTY;

    unsigned int blackCount = 0;
    unsigned int whiteCount = 0;

    for(size_t i = 0; i < GRID_SIZE; ++i){
        for(size_t j = 0; j < GRID_SIZE; ++j){

            if(board[i][j] == BLACK){
                blackCount++;
            }
            
            else if(board[i][j] == WHITE){
                whiteCount++;
            }
        }
    }

    return (blackCount > whiteCount) ? BLACK :
       (whiteCount > blackCount) ? WHITE : EMPTY;

}

set<Othello::POSITION> Othello::getMoves() {
    // Return empty set if game is finished
    if (isGameOver()) {
        return set<POSITION>();
    }

    return getAvailableMoves(activePlayer);
}

set<Othello::POSITION> Othello::getAvailableMoves(Colour player) {

    set<POSITION> moves;
    
    Colour originalPlayer = activePlayer;
    
    activePlayer = player;
    
    for(size_t i = 0; i < GRID_SIZE; ++i) {
        for(size_t j = 0; j < GRID_SIZE; ++j) {
            if(board[i][j] == EMPTY) {
                POSITION pos = std::make_pair(i, j);
                if(isValidMove(pos)) {
                    moves.insert(pos);
                }
            }
        }
    }
    
    activePlayer = originalPlayer;
    
    return moves;
}

void Othello::flipDisc(POSITION pos, int x, int y) {
    int row = pos.first + x;
    int col = pos.second + y;
    
    vector<POSITION> piecesToFlip;

    // While we are on the board
    while(row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE) {
        if(board[row][col] == EMPTY) break;
        
        if(board[row][col] == activePlayer) {
            // Flip all pieces in this direction
            for(const auto& piece : piecesToFlip) {
                if(piece.first >= 0 && piece.first < GRID_SIZE && 
                   piece.second >= 0 && piece.second < GRID_SIZE) {
                    board[piece.first][piece.second] = activePlayer;
                }
            }
            break;
        }
        
        piecesToFlip.push_back(make_pair(row, col));
        row += x;
        col += y;
    }
}

bool Othello::play(POSITION pos) {
    // Vérification des limites du tableau
    if(pos.first < 0 || pos.first >= GRID_SIZE || pos.second < 0 || pos.second >= GRID_SIZE) {
        return false;
    }

    set<POSITION> moves = getMoves();
    
    // The find method return the end if it is not in the set
    if(moves.find(pos) == moves.end()) {
        return false;
    }

    // Place the piece
    board[pos.first][pos.second] = activePlayer;

    // For each direction
    for(int i = 0; i < 8; ++i) {
        flipDisc(pos, directions[i][0], directions[i][1]);
    }

    // Switch player
    activePlayer = (activePlayer == BLACK) ? WHITE : BLACK;

    activePlayer = getActivePlayer();
    
    return true;
}

bool Othello::isValidMove(POSITION pos){
    // Vérification des limites du tableau
    if(pos.first < 0 || pos.first >= GRID_SIZE || pos.second < 0 || pos.second >= GRID_SIZE){
        return false;
    }

    if(board[pos.first][pos.second] != EMPTY) {
        return false;
    }

    for(int i = 0; i < 8; ++i) {
        int row = pos.first + directions[i][0];
        int col = pos.second + directions[i][1];

        bool foundOpponent = false;

        // While we are on the limit of the board
        while(row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE){
            if(board[row][col] == EMPTY) break;
            
            if(board[row][col] == activePlayer){
                if(foundOpponent) {
                    return true;
                }
                break;
            }
            
            foundOpponent = true;

            row += directions[i][0];
            col += directions[i][1];
        }
    }
    
    return false;
}

Othello::Colour Othello::value(POSITION pos){
    if(pos.first < 0 || pos.first >= GRID_SIZE || pos.second < 0 || pos.second >= GRID_SIZE){
        throw out_of_range("Position out of bounds");
    }

    return board[pos.first][pos.second];
}




