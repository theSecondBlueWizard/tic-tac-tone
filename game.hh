#pragma once
#include "main.hh"
#include "Arduino.h"

enum Player {
    None,
    Red,
    Yellow
};

class TicTacToe {
    public:
        Player currentMove = Red;

        void printBoard();
        void updateBoard();
        void move(int x, int y, Player player);
        Player winner = None;
        bool finished = false;
        
    private:
        Player board[3][3] = { 0 };
        void detectVictories();
        void printLedArray(
            bool yellow[LED_MATRIX_Y][LED_MATRIX_X],
            bool red[LED_MATRIX_Y][LED_MATRIX_X]
        );
};