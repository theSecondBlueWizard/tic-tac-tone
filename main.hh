#pragma once

#define latchPin 2  // RCLK
#define serialPin 3 // SRCLK
#define redLED 4    //
#define yellowLED 5 //
#define buttonPin 6 //
#define yellowTurnPin 7 //
#define redTurnPin 8 //


#define boardLen 6
#define N_SAMPLES 192
#define LED_MATRIX_X 6
#define LED_MATRIX_Y 3
#define SHIFT_REG_SIZE 16

#include <time.h>
#include "game.hh"
#include "mic.hh"
#include "Arduino.h"