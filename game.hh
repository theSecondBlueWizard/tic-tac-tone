#pragma once
#include "main.hh"
#include "Arduino.h"

enum Player {
    None,
    Red,
    Black
};

class TicTacToe {
    public:
        Player currentMove = Red;

        void printBoard();
        void move(int x, int y, Player player);

        Player winner = None;
        bool finished = false;
        
        Player board[3][3];
    private:
        void detectVictories();
};