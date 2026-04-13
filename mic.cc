#include "mic.hh"

float measureFrequency(){
  const int nSamples = N_SAMPLES;
  uint16_t timeStamps[nSamples] {0};
  uint8_t samples[nSamples] {0};

  // take time & sample readings
  unsigned long t0 = micros();

  for (int i = 0; i < nSamples; i++) {
    timeStamps[i] = (micros() - t0);
    samples[i] = analogRead(0);
    delayMicroseconds(120);
  }
  
  int mean = 0;
  // find average
  for (int i = 0; i < nSamples; i++) {
    mean += samples[i];
  }
  mean /= nSamples;

  int edgesDetected = 0;
  uint16_t edgeTimes[nSamples] {0};

  // Find edges in sample
  for (int i = 0; i < nSamples - 1; i++) {
    if (samples[i] < mean && samples[i + 1] > mean) {
      edgeTimes[edgesDetected] = timeStamps[i];
      edgesDetected ++;
    }
  }

  // Find periods in samples
  uint16_t periods[edgesDetected - 1] {0};

  for (int i = 0; i < edgesDetected - 1; i++) {
    periods[i] = edgeTimes[i + 1] - edgeTimes[i];
  }

  // Calculate average period
  double meanPeriod = 0;
  for (int i = 0; i < edgesDetected - 1; i++) {
    meanPeriod += periods[i];
  }
  meanPeriod /= (edgesDetected - 1);
  return 1000000 / meanPeriod;
}




float measureAmplitude() {

  float sum = 0;

  float nSamples = 512;
  
  for (int i = 0; i < nSamples; i++) {
    sum += abs(analogRead(0));
    delayMicroseconds(100);
  }
  float amplitude = sum / (float) nSamples;

  return amplitude;
}

// bool buttonPressed;