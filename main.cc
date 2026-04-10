#include "game.hh"

int mainNOT() {
    TicTacToe game;

    // std::cout << "Welcome to TicTacToe!" << "\n";

    while (!game.finished) {
        game.printBoard();

        // std::cout << "Current move: ";
        switch (game.currentMove) {
            case Red:
                // std::cout << "r";
                break;
            case Yellow:
                // std::cout << "b";
                break;
            default:
                break;
                // throw std::runtime_error("[Main] No current move!");
        }

        int x;
        // std::cout << "\nx > ";
        // std::cin >> x;
        
        int y;
        // std::cout << "y > ";
        // std::cin >> y;
        game.move(x, y, game.currentMove);
        
        // std::cout << "\n";
    }
    
    
    
    // std::cout << game.winner << "\n";
    return 0;
}