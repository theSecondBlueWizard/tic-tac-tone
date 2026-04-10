#include "game.hh"
#include "main.hh"

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 6; i++) {
    pinMode(i, OUTPUT);
    delay(10);
    digitalWrite(i, LOW);
  }
  pinMode(buttonPin, INPUT);
  Serial.println("[setup] Arduino is ready");
}

// float measureFrequency(){
//   const int nSamples = N_SAMPLES;
//   uint16_t timeStamps[nSamples] {0};
//   uint8_t samples[nSamples] {0};

//   // take time & sample readings
//   unsigned long t0 = micros();

//   for (int i = 0; i < nSamples; i++) {
//     timeStamps[i] = (micros() - t0);
//     samples[i] = analogRead(0);
//     delayMicroseconds(120);
//   }
  
//   int mean = 0;
//   // find average
//   for (int i = 0; i < nSamples; i++) {
//     mean += samples[i];
//   }
//   mean /= nSamples;

//   int edgesDetected = 0;
//   uint16_t edgeTimes[nSamples] {0};

//   // Find edges in sample
//   for (int i = 0; i < nSamples - 1; i++) {
//     if (samples[i] < mean && samples[i + 1] > mean) {
//       edgeTimes[edgesDetected] = timeStamps[i];
//       edgesDetected ++;
//     }
//   }

//   // Find periods in samples
//   uint16_t periods[edgesDetected - 1] {0};

//   for (int i = 0; i < edgesDetected - 1; i++) {
//     periods[i] = edgeTimes[i + 1] - edgeTimes[i];
//   }

//   // Calculate average period
//   double meanPeriod = 0;
//   for (int i = 0; i < edgesDetected - 1; i++) {
//     meanPeriod += periods[i];
//   }
//   meanPeriod /= (edgesDetected - 1);
//   return 1000000 / meanPeriod;
// }

// bool buttonPressed;

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

void loop() {
    TicTacToe game;
    Serial.println("Welcome to TicTacToe!");

    while (!game.finished) {
        game.printBoard();
        game.updateBoard();

        Serial.print("Current move: ");
        switch (game.currentMove) {
            case Red:
                Serial.println("Red!");
                break;
            case Yellow:
                Serial.println("Yellow!");
                break;
            default:
                break;
                Serial.println("[loop] Error: No current move!");
        }

        int x;
        Serial.print("x > ");

        while (intValid == false) {
          takeInt();
        }
        intValid = false;
        x = intData;
        
        int y;
        Serial.print("y > ");
        while (intValid == false) {
          takeInt();
        }
        intValid = false;
        y = intData;

        game.move(x, y, game.currentMove);
        
        // std::cout << "\n";
    }
    // std::cout << game.winner << "\n";
  delay(500);
}
