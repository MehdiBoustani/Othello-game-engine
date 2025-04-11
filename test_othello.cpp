
#include "othello.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>

// Helper function to clear the screen (platform-independent)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Helper function to print the board with more visual appeal
void printBoard(Othello& game) {
    std::cout << "\n    ";
    for (int col = 0; col < 8; col++) {
        std::cout << col << "   ";
    }
    std::cout << "\n  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n";
    
    for (int row = 0; row < 8; row++) {
        std::cout << row << " │";
        for (int col = 0; col < 8; col++) {
            Othello::Colour cell = game.value({row, col});
            char symbol = ' ';
            if (cell == Othello::BLACK) symbol = 'B';
            else if (cell == Othello::WHITE) symbol = 'W';
            std::cout << " " << symbol << " │";
        }
        
        if (row < 7) {
            std::cout << "\n  ├───┼───┼───┼───┼───┼───┼───┼───┤\n";
        } else {
            std::cout << "\n  └───┴───┴───┴───┴───┴───┴───┴───┘\n";
        }
    }
}

// Helper function to print available moves with numbers for selection
std::vector<Othello::POSITION> printMoves(const std::set<Othello::POSITION>& moves) {
    std::vector<Othello::POSITION> moveVector(moves.begin(), moves.end());
    
    std::cout << "Available moves:\n";
    for (size_t i = 0; i < moveVector.size(); i++) {
        std::cout << "  " << i + 1 << ". (" << moveVector[i].first << "," 
                  << moveVector[i].second << ")\n";
    }
    
    return moveVector;
}

// Function to show game statistics
void showStats(Othello& game) {
    // Count pieces
    int blackCount = 0, whiteCount = 0, emptyCount = 0;
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Othello::Colour cell = game.value({row, col});
            if (cell == Othello::BLACK) blackCount++;
            else if (cell == Othello::WHITE) whiteCount++;
            else emptyCount++;
        }
    }
    
    std::cout << "\nGame statistics:\n";
    std::cout << "  Black (B): " << blackCount << " pieces\n";
    std::cout << "  White (W): " << whiteCount << " pieces\n";
    std::cout << "  Empty spaces: " << emptyCount << "\n";
}

// Main game loop
void playGame() {
    Othello game;
    bool quit = false;
    int moveCount = 0;
    
    while (!game.isGameOver() && !quit) {
        clearScreen();
        
        // Display game information
        std::cout << "=== OTHELLO INTERACTIVE GAME ===\n";
        std::cout << "Move #" << ++moveCount << "\n";
        
        auto activePlayer = game.getActivePlayer();
        std::cout << "Current player: " 
                  << (activePlayer == Othello::BLACK ? "BLACK (B)" : "WHITE (W)") << "\n";
        
        // Display board
        printBoard(game);
        
        // Show statistics
        showStats(game);
        
        // Get available moves
        auto moves = game.getMoves();
        if (moves.empty()) {
            std::cout << "\nNo moves available for current player. Press Enter to continue...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        // Display available moves
        auto moveVector = printMoves(moves);
        
        // Get player's choice
        int choice = -1;
        bool validChoice = false;
        
        std::cout << "\nEnter move number (1-" << moveVector.size() 
                  << "), 'c' to see coordinates, or 'q' to quit: ";
        
        std::string input;
        std::getline(std::cin, input);
        
        // Check for quit
        if (input == "q" || input == "Q") {
            quit = true;
            continue;
        }
        
        // Check for coordinates input mode
        if (input == "c" || input == "C") {
            int row = -1, col = -1;
            
            std::cout << "Enter row and column (e.g., '2 3'): ";
            std::string coordInput;
            std::getline(std::cin, coordInput);
            
            std::istringstream iss(coordInput);
            if (iss >> row >> col) {
                Othello::POSITION pos(row, col);
                
                // Check if this is a valid move
                bool validMove = false;
                for (const auto& move : moves) {
                    if (move.first == row && move.second == col) {
                        validMove = true;
                        break;
                    }
                }
                
                if (validMove) {
                    if (game.play(pos)) {
                        std::cout << "Move successful!\n";
                    } else {
                        std::cout << "Invalid move. Press Enter to try again...\n";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } else {
                    std::cout << "That position is not a valid move. Press Enter to try again...\n";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            } else {
                std::cout << "Invalid input format. Press Enter to try again...\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            
            continue;
        }
        
        // Try to parse as a number
        try {
            choice = std::stoi(input);
            if (choice >= 1 && choice <= static_cast<int>(moveVector.size())) {
                validChoice = true;
            }
        } catch (...) {
            validChoice = false;
        }
        
        if (validChoice) {
            Othello::POSITION pos = moveVector[choice - 1];
            if (game.play(pos)) {
                std::cout << "Played move at (" << pos.first << "," << pos.second << ")!\n";
            } else {
                std::cout << "Invalid move. Press Enter to try again...\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } else {
            std::cout << "Invalid choice. Press Enter to try again...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    // Game over
    clearScreen();
    std::cout << "=== GAME OVER ===\n";
    printBoard(game);
    showStats(game);
    
    if (quit) {
        std::cout << "\nGame ended by player.\n";
    } else {
        std::cout << "\nWinner: ";
        auto winner = game.getWinner();
        if (winner == Othello::EMPTY) {
            std::cout << "It's a draw!\n";
        } else if (winner == Othello::BLACK) {
            std::cout << "BLACK (B) wins!\n";
        } else {
            std::cout << "WHITE (W) wins!\n";
        }
    }
}

int main() {
    // Welcome screen
    std::cout << "\n==================================\n";
    std::cout << "         OTHELLO GAME             \n";
    std::cout << "==================================\n\n";
    std::cout << "Welcome to Othello!\n";
    std::cout << "Black (B) always goes first.\n";
    std::cout << "Press Enter to start the game...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    playGame();
    
    std::cout << "\nThanks for playing!\n";
    return 0;
}