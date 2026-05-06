#include "game.hh"

void TicTacToe::printLedArray(
  bool yellow[LED_MATRIX_Y][LED_MATRIX_X],
  bool red[LED_MATRIX_Y][LED_MATRIX_X]) {

  bool flatYellow[SHIFT_REG_SIZE + 2];
  bool flatRed[SHIFT_REG_SIZE + 2];

  int i = 0;
  for (int y = LED_MATRIX_Y - 1; y >= 0; y--) {
    for (int x = 0; x < LED_MATRIX_X; x++) {
      flatYellow[i] = yellow[y][x];
      flatRed[i] = red[y][x];
      i++;
    }
  }
  
  digitalWrite(latchPin, LOW);
  delay(10);

  for(int i = 2; i < SHIFT_REG_SIZE + 2; i++) {
    digitalWrite(serialPin, LOW);

    if (flatYellow[i] == true) {
      digitalWrite(yellowLED, HIGH);
    } else {
      digitalWrite(yellowLED, LOW);
    }
    if (flatRed[i] == true) {
      digitalWrite(redLED, HIGH);
    } else {
      digitalWrite(redLED, LOW);
    }
    digitalWrite(serialPin, HIGH);
  }
  
  delay(10);
  digitalWrite(latchPin, HIGH);
  delay(10);
  return;
}

bool emptyLedArray[3][6] = { 0 };
bool crossLedArray[3][6] = {
    { 0, 0, 0, 1, 0, 1 },
    { 0, 0, 0, 0, 1, 0 },
    { 0, 0, 0, 1, 0, 1 },
};

void TicTacToe::crashAndBurn() {
    updateBoard();
    switch (currentMove) {
        case Yellow:
            for (int i = 0; i < 3; i++) {
                delay(200);
                printLedArray(crossLedArray, emptyLedArray);
                delay(200);
                printLedArray(emptyLedArray, emptyLedArray);
            }
            break;
        case Red:
            for (int i = 0; i < 3; i++) {
                delay(200);
                printLedArray(emptyLedArray, crossLedArray);
                delay(200);
                printLedArray(emptyLedArray, emptyLedArray);
            }
            break;
        default:
            for (int i = 0; i < 3; i++) {
                delay(500);
                printLedArray(crossLedArray, emptyLedArray);
                delay(500);
                printLedArray(emptyLedArray, crossLedArray);
            }
    }
    updateBoard();
}

void TicTacToe::updateBoard() {
    bool yellow[LED_MATRIX_Y][LED_MATRIX_X] = {0};
    bool red[LED_MATRIX_Y][LED_MATRIX_X] = {0};

    for(int y = 0; y < 3; y++) {
        for(int x = 0; x < 3; x++) {
            Player cellOwner = board[y][x];

            switch (cellOwner) {
                case Player::Yellow:
                    yellow[y][x + 3] = true;
                    break;
                case Player::Red:
                    red[y][x + 3] = true;
                    break;
                case Player::None:
                    break;
                default:
                    Serial.println("[Game] Error: non-player object on board");
                    crashAndBurn();
                    break;
            }
        }
    }
    if (winner == Player::Yellow) {
        for (int i = 0; i < 3; i++) {
            printLedArray(yellow, red);
            delay(500);
            printLedArray(emptyLedArray, red);
            delay(500);
        }
    }
    else if (winner == Player::Red) {
        for (int i = 0; i < 3; i++) {
            printLedArray(yellow, red);
            delay(500);
            printLedArray(yellow, emptyLedArray);
            delay(500);
        }
    }
    printLedArray(yellow, red);
}

void TicTacToe::printBoard() {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            Serial.print(board[y][x]);
        }
        Serial.println();
    }
}

void TicTacToe::move(int x, int y, Player player) {
    if (finished == true) {
        Serial.println("[TicTacToe] Error: game already finished!");
        crashAndBurn();
        return;
    }
    
    if (player != currentMove) {
        Serial.println("[TicTacToe] Error: Wrong player move attempted");
        crashAndBurn();
        return;
    }

    if (board[y][x] != None) {
        Serial.println("[TicTacToe] Error: Illegal move");
        crashAndBurn();
        return;
    }

    if (winner != None) {
        Serial.println("[TicTacToe] Error:Player already won!");
        crashAndBurn();
        return;
    }

    Player newCellOwner;

    switch (player) {
        case Red:
            newCellOwner = Player::Red;
            break;
        case Yellow:
            newCellOwner = Player::Yellow;
            break;
        default:
            Serial.println("[TicTacToe] Error: Illegal player");
    };

    board[y][x] = newCellOwner;
    
    switch (currentMove) {
        case Red:
            currentMove = Yellow;
            break;
        case Yellow:
            currentMove = Red;
            break;
        default:
            Serial.println("[TicTacToe] Error: Illegal state of currentMove");
    }

    detectVictories();
    if (this->finished == true && winner == Player::None) {
        Serial.println("[TicTacToe] Error: game is finished!");
        crashAndBurn();
        return;
    }
}

void TicTacToe::detectVictories() {
    if (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][0] != None){ this-> winner = board[0][0]; }
    if (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][1] != None){ this-> winner = board[1][1]; }
    if (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][2] != None){ this-> winner = board[2][2]; }
    
    if (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[0][0] != None){ this-> winner = board[0][0]; }
    if (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[1][1] != None){ this-> winner = board[1][1]; }
    if (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[2][2] != None){ this-> winner = board[2][2]; }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] != None){ this-> winner = board[1][1]; }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != None){ this-> winner = board[1][1]; }
    
    this->finished = true;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == Player::None) {this->finished = false; Serial.println("noone's won yet!");}
        }
    }
    
    if (this->finished == true) currentMove = None;

    if (winner != None) this->finished = true;
}
