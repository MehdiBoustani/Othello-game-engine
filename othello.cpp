#include "othello.h"
#include <stdexcept>
#include <sstream>

using namespace std;

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
        throw length_error("Othello board must have " + to_string(GRID_SIZE) + "rows")
    }

    for (size_t i = 0; i < board.size; ++i){
        if(board[i].size() != GRID_SIZE){
            throw length_error("Othello board must have" + to_string(GRID_SIZE) + "columns")
        }
    }

    if(player != WHITE && player != BLACK){
        throw invalid_argument("Player must be WHITE or BLACK")
    }

    this->board = board;

    this->activePlayer = player;
}

Othello::getActivePlayer(){
    return isGameOver() ? EMPTY : activePlayer;
}

Othello::isGameOver(){
    // Check if board is full
    bool isFull = true;

    for(size_t i = 0; i < GRID_SIZE; ++i){
        for(size_t j = 0; j < GRID_SIZE; ++j){
            if(board[i][j] == EMPTY){
                isFull = false;
                break;
            }
        }
        if(!isFull) break;
    }

    if(isFull) return true;

    // Check if current player can play
    if(!getMoves().empty()) return false;

    // If current player can't play, check if opponent can play
    Colour opponent = (activePlayer == BLACK) ? WHITE : BLACK;

    Colour temp = activePlayer;

    activePlayer = opponent;

    bool opponentCanPlay = !getMoves().empty();
    
    activePlayer = temp;

    return !opponentCanPlay;
}

Othello::getWinner(){
    if(!isGameOver) return EMPTY;

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

    if(blackCount > whiteCount){
        return BLACK;
    } 
    
    else if(whiteCount > blackCount){
        return WHITE;
    } 
    
    else{
        return EMPTY;
    }
}

Othello::getMoves(){
    set<POSITION> moves = {};

    if(isGameOver){
        return moves;
    }

    for(size_t i = 0; i < GRID_SIZE; ++i){
        for(size_t j = 0; j < GRID_SIZE; ++j){
            if(board[i][j] == EMPTY){

                POSITION pos = make_pair(i, j);

                if(isValidMove(pos)){
                    moves.insert(pos);
                }
            }
        }
    }

    return moves;
}

Othello::play(POSITION pos){
    set<POSITION> moves = getMoves();
    
    // The find method return the end if it is not in the set
    if(moves.find(pos) == moves.end()){
        return false;
    }

    // Place the piece
    board[pos.first][pos.second] = activePlayer;

    // The 8 possible directions
    const int directions = this->directions;

    // For each direction
    for(const auto& dir : directions){
        int row = pos.first + dir[0];
        int col = pos.second + dir[1];
        vector<POSITION> piecesToFlip;

        // While we are on the board
        while(row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE){
            if(board[row][col] == EMPTY) break;
            
            if(board[row][col] == activePlayer){
                // Flip all pieces in this direction
                for(const auto& piece : piecesToFlip){
                    board[piece.first][piece.second] = activePlayer;
                }
                break;
            }
            
            piecesToFlip.push_back(make_pair(row, col));

            row += dir[0];
            col += dir[1];
        }
    }

    // Switch player
    activePlayer = (activePlayer == BLACK) ? WHITE : BLACK;
    
    return true;
}

bool Othello::isValidMove(POSITION pos){
    if(board[pos.first][pos.second] != EMPTY) return false;

    const int directions = this->directions;

    for(const auto& dir : directions){
        int row = pos.first + dir[0];
        int col = pos.second + dir[1];

        bool foundOpponent = false;

        // While we are on the limit of the board
        while(row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE){
            if(board[row][col] == EMPTY) break;
            
            if(board[row][col] == activePlayer){
                if(foundOpponent) return true;
                break;
            }
            
            foundOpponent = true;

            row += dir[0];
            col += dir[1];
        }
    }
    
    return false;
}

Othello::value(POSITION pos){
    if(pos.first < 0 || pos.first >= GRID_SIZE || pos.second < 0 || pos.second >= GRID_SIZE){
        throw out_of_range("Position out of bounds");
    }

    return board[pos.first][pos.second];
}




