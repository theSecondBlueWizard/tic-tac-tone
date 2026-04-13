#include "main.hh"

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 9; i++) {
    pinMode(i, OUTPUT);
    delay(10);
    digitalWrite(i, LOW);
  }
  pinMode(buttonPin, INPUT);
  Serial.println("[setup] Arduino is ready");
}

bool intValid = false;
int intData;
void takeInt() {
   if (Serial.available() > 0) {
      char in = Serial.read();
      if (in == '0' || in == '1' || in == '2') {
        intValid = true;
        intData = in - '0';
      }
  }
}

int serialIn() {
  while (intValid == false) {
    takeInt();
  }
  intValid = false;
  return intData;
}

bool ax[LED_MATRIX_Y][LED_MATRIX_X] = {
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0}
};
bool bx[LED_MATRIX_Y][LED_MATRIX_X] = {
  {0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 1, 0}
};
bool cx[LED_MATRIX_Y][LED_MATRIX_X] = {
  {0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 1}
};

bool ay[LED_MATRIX_Y][LED_MATRIX_X] = {
  {0, 0, 0, 1, 1, 1},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};
bool by[LED_MATRIX_Y][LED_MATRIX_X] = {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1},
  {0, 0, 0, 0, 0, 0}
};
bool cy[LED_MATRIX_Y][LED_MATRIX_X] = {
  {0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1}
};
bool nullArray[LED_MATRIX_Y][LED_MATRIX_X] = {
  {0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};

void printLedArray(
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

int micInX(Player player) {
  while (!digitalRead(buttonPin)) {
    continue;
  }
  float freq;
  while (digitalRead(buttonPin)) {
    float amplitude = measureAmplitude();
    // Serial.println(amplitude);
    // if (amplitude < 510 || amplitude > 511.5) {
    if (true) {
      freq = measureFrequency();
      // Serial.println(freq);
      switch(player) {
        case Yellow:
          if(freq < 400) printLedArray(ax, nullArray);
          else if (freq < 600) printLedArray(bx, nullArray);
          else printLedArray(cx, nullArray);
          break;
        case Red:
          if(freq < 400) printLedArray(nullArray, ax);
          else if (freq < 600) printLedArray(nullArray, bx);
          else printLedArray(nullArray, cx);
          break;

      }
    }
  }
  if (freq < 400) return 0;
  if (freq < 600) return 1;
  return 2;
}

int micInY(Player player) {
  while (!digitalRead(buttonPin)) {
    continue;
  }
  float freq;
  while (digitalRead(buttonPin)) {
    float amplitude = measureAmplitude();
    // Serial.println(amplitude);
    // Serial.println(freq);
    // if (amplitude < 510 || amplitude > 511.5) {
    if (true) {
      freq = measureFrequency();
      switch(player) {
        case Yellow:
          if(freq < 400) printLedArray(ay, nullArray);
          else if (freq < 600) printLedArray(by, nullArray);
          else printLedArray(cy, nullArray);
          break;
        case Red:
          if(freq < 400) printLedArray(nullArray, ay);
          else if (freq < 600) printLedArray(nullArray, by);
          else printLedArray(nullArray, cy);
          break;
        }
      }
  }
  if (freq < 400) return 0;
  if (freq < 600) return 1;
  return 2;
}

void loop() {
    TicTacToe game;
    Serial.println("Welcome to TicTacToe!");

    while (!game.finished) {
        game.printBoard();
        digitalWrite(redTurnPin, LOW);
        digitalWrite(yellowTurnPin, LOW);
        game.updateBoard();

        Serial.print("Current move: ");
        switch (game.currentMove) {
            case Red:
                digitalWrite(redTurnPin, HIGH);
                Serial.println("Red!");
                break;
            case Yellow:
                digitalWrite(yellowTurnPin, HIGH);
                Serial.println("Yellow!");
                break;
            default:
                break;
                Serial.println("[loop] Error: No current move!");
        }

        Serial.print("x > ");
        int x = micInX(game.currentMove);
        Serial.println(x);
        delay(500);
        game.updateBoard();

        
        Serial.print("y > ");
        int y = micInY(game.currentMove);
        Serial.println(y);
        delay(500);
        game.move(x, y, game.currentMove);
    }
  game.updateBoard();
}
