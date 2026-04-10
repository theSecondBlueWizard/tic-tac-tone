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
                    break;
            }
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
        return;
    }
    
    if (player != currentMove) {
        Serial.println("[TicTacToe] Error: Wrong player move attempted");
        return;
    }

    if (board[y][x] != None) {
        Serial.println("[TicTacToe] Error: Illegal move");
        return;
    }

    if (winner != None) {
        Serial.println("[TicTacToe] Error:Player already won!");
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
    
    finished = true;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] != None) finished = false;
        }
    }

    if (winner != None) finished = true;
}