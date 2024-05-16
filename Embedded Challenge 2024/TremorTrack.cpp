#include <Adafruit_CircuitPlayground.h>
#include "arduinoFFT.h"

#define MIN_TREMOR_DURATION 300000 // originally 3000 milliseconds -> actual tremor has 300000 milliseconds

/*
DISCLAIMER:
IF THE TESTER DOES NOT CHANGE THE CODE TO THE ORIGINAL VALUES
THE CODE WILL NOT RUN AS IT TAKES UP TOO MUCH MEMORY

MAKE THE FOLLOWING CHANGE TO ALLOW THE CODE TO WORK:
 -> change the sample size to 64

-> turn the switch on to begin data collection (should make a beep noise)
-> turn the switch off to stop data collection (should make a different noise)
-> if there is a potential tremor it will show white NeoPixels blinking
-> if there is no tremor being detected it shouldn't display anything
-> if there is a mild tremor it should light up green for NeoPixels 1-5
-> if there is a moderate tremor it should light up blue for NeoPixels 1-8
-> if there is a severe tremor it should light up red for NeoPixels 1-10
   (should beep three times before lighting up)
*/

const uint16_t SAMPLE_SIZE = 256; // // originally 64 samples -> actual application will have 256 samples
const float SAMPLING_FREQUENCY = 100.0;

float realData[SAMPLE_SIZE];
float imagData[SAMPLE_SIZE] = {0};
float magnitudes[SAMPLE_SIZE / 2];
int magnitudeIndex = 0;
bool collectingData = false;
bool dataCollected = false;

float threshold = 0;
float peakMagnitude = 0;
float peakFrequency = 0;

float mildThreshold = 1.50;
float moderateThreshold = 3.0;
float severeThreshold = 5.0;

unsigned long tremorStartTime = 0;
bool tremorOngoing = false;
int lastSeverity = 0;

void collectAccelerometerData();
void processFFT();
void determineTremorPresence(float frequency, float magnitude);
void adjustThreshold();
void checkSwitchAndToggleCollection();
void displaySeverity(int severity);
void manageTremorConfirmation();


ArduinoFFT<float> fft(realData, imagData, SAMPLE_SIZE, SAMPLING_FREQUENCY);

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.playTone(200, 500);
  delay(100);
  DDRF &= ~(1 << 4); // configure PF4 as input with pull-up
  PORTF |= (1 << 4);
}

void loop() {
  checkSwitchAndToggleCollection();
  if (collectingData) {
    collectAccelerometerData();
    processFFT();
    adjustThreshold();
    CircuitPlayground.playTone(200, 500);
    delay(100);
  } else if (dataCollected) {
    collectAccelerometerData();
    processFFT();
    determineTremorPresence(peakFrequency, peakMagnitude);
    manageTremorConfirmation();
    delay(500); // delay to throttle the output and processing
  }
}

void collectAccelerometerData() {
  unsigned long period = 1000000 / SAMPLING_FREQUENCY;
  unsigned long lastMicros = micros();
  for (int i = 0; i < SAMPLE_SIZE; i++) {
    while (micros() - lastMicros < period); // wait for the next sample period
    lastMicros += period;
    realData[i] = sqrt(pow(CircuitPlayground.motionX(), 2) + pow(CircuitPlayground.motionY(), 2) + pow(CircuitPlayground.motionZ(), 2));
    imagData[i] = 0;
  }
}

void processFFT() {
  fft.windowing(FFT_WIN_TYP_HANN, FFT_FORWARD, true);
  fft.compute(FFT_FORWARD);
  fft.complexToMagnitude();

  peakMagnitude = 0;
  peakFrequency = 0;
  for (int i = 1; i < SAMPLE_SIZE / 2; i++) {
    float frequency = (i * SAMPLING_FREQUENCY) / SAMPLE_SIZE;
    float magnitude = (realData[i] * 2) / SAMPLE_SIZE;
    if (frequency >= 3.0 && frequency <= 6.0 && magnitude > peakMagnitude) {
      peakMagnitude = magnitude;
      peakFrequency = frequency;
    }
  }
}

void checkSwitchAndToggleCollection() {
  static bool lastSwitchState = LOW;
  bool currentSwitchState = !(PINF & (1 << 4));

  if (lastSwitchState != currentSwitchState) {
    lastSwitchState = currentSwitchState;
    collectingData = currentSwitchState; // toggle data collection based on switch
    if (collectingData) {
      CircuitPlayground.playTone(500, 100);
      delay(100);
      dataCollected = false;
      magnitudeIndex = 0;
    } else {
      CircuitPlayground.playTone(1000, 200);
      delay(100);
      dataCollected = true;
    }
  }
}

void adjustThreshold() {
  if (magnitudeIndex < (SAMPLE_SIZE * (8/10))) {
    magnitudes[magnitudeIndex++] = peakMagnitude;
  } else {
    float sum = 0, sumSq = 0;
  for (int i = 0; i < magnitudeIndex; i++) {
    sum += magnitudes[i];
    sumSq += magnitudes[i] * magnitudes[i];
  }
  float mean = sum / magnitudeIndex;
  float stddev = sqrt((sumSq / magnitudeIndex) - (mean * mean));

  mildThreshold = mean + stddev;
  moderateThreshold = mean + 2 * stddev;
  severeThreshold = mean + 3 * stddev;
  threshold = mean + 2 * stddev;

  collectingData = false;
  dataCollected = true;
  }
}


int calculateSeverity(float magnitude) {
    if (magnitude > severeThreshold) {
        return 3;
    } else if (magnitude > moderateThreshold) {
        return 2;
    } else if (magnitude > mildThreshold) {
        return 1;
    } else {
        return 0;
    }
}

void determineTremorPresence(float frequency, float magnitude) {
    unsigned long currentMillis = millis();

    if (frequency >= 2 && frequency <= 8) {
        int currentSeverity = calculateSeverity(magnitude);
        if (currentSeverity > 0) {
            if (!tremorOngoing) {
                tremorOngoing = true;
                tremorStartTime = currentMillis;
                lastSeverity = currentSeverity;
            }
            uint32_t color = 0xFFFFFF;
            for (int i = 0; i < 10; i++) {
              CircuitPlayground.setPixelColor(i, color);
              delay(100);
              CircuitPlayground.clearPixels();
            }
            delay(100);         
        } else {
            tremorOngoing = false;
        }
    } else {
        tremorOngoing = false;
    }
}

void manageTremorConfirmation() {
    unsigned long currentMillis = millis();
    if (tremorOngoing && (currentMillis - tremorStartTime >= MIN_TREMOR_DURATION)) {
        displaySeverity(lastSeverity);
        tremorOngoing = false;  // reset after confirming tremor
    }
}

void displaySeverity(int severity) {
  CircuitPlayground.clearPixels();
  uint32_t color;
  int pixelsToLight = 0;

  switch (severity) {
    case 1:  // mild tremor
      color = 0x00FF00;
      pixelsToLight = 5;
      break;
    case 2:  // moderate tremor
      color = 0x0000FF;
      pixelsToLight = 8;
      break;
    case 3:  // severe tremor
      color = 0xFF0000;
      pixelsToLight = 10;
      CircuitPlayground.playTone(1000, 200);
      delay(100);
      CircuitPlayground.playTone(1000, 200);
      delay(100);
      CircuitPlayground.playTone(1000, 200);
      delay(100);
      break;
    default:
      color = 0;  // no tremor detected
      pixelsToLight = 0;
      break;
  }

  for (int i = 0; i < pixelsToLight; i++) {
    CircuitPlayground.setPixelColor(i, color);
    delay(100);
  }

  delay(500);
  CircuitPlayground.clearPixels();
}