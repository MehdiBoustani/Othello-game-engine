#include "othello.h"
#include <iostream>

using namespace std;

int main() {
    try {
        cout << "Création du jeu..." << endl;
        Othello game;
        
        cout << "Test des mouvements valides..." << endl;
        auto moves = game.getMoves();
        cout << "Nombre de mouvements valides au début: " << moves.size() << endl;
        
        // Test d'un mouvement valide
        cout << "\nTest d'un mouvement valide..." << endl;
        Othello::POSITION pos = make_pair(2, 3);
        if(game.play(pos)) {
            cout << "Mouvement réussi" << endl;
        } else {
            cout << "Mouvement échoué" << endl;
        }
        
        // Test d'un mouvement invalide
        cout << "\nTest d'un mouvement invalide..." << endl;
        pos = make_pair(0, 0);
        if(game.play(pos)) {
            cout << "Mouvement réussi" << endl;
        } else {
            cout << "Mouvement échoué" << endl;
        }
        
        return 0;
    } catch (const exception& e) {
        cerr << "Erreur: " << e.what() << endl;
        return 1;
    }
} 