# Tremor Detector Project

# Project Description
This project uses an Adafruit Circuit Playground to detect Parkinsonian tremors using its built-in accelerometer. The code captures accelerometer data, processes it using an FFT, and determines the presence and severity of tremors based on predefined thresholds. Visual feedback is provided using NeoPixels on the Circuit Playground, and audio feedback is given using the built-in speaker.

# Prerequisites
- Hardware: Adafruit Circuit Playground Classic, Micro-USB cable, Powerbank.
- Software: Arduino IDE or PlatformIO, Adafruit Circuit Playground library, arduinoFFT library.

# Instructions
1. Setup the development environment:
   - Install the required libraries in the Arduino IDE.
2. Configure the code:
   - Change `SAMPLE_SIZE` to `64` to ensure the code is fully functional.
3. Load and upload the code to the Circuit Playground.
4. Operating Instructions:
   - Turn the switch on to start data collection (beep sound).
   - Turn the switch off to stop data collection (different beep sound).
   - Visual feedback for tremor detection using NeoPixels and audio feedback.

# Data Collection Guidelines
1. Initial Setup:
   - Place the Circuit Playground on a stable surface or wear it securely on your wrist.
   - Ensure the device is not moving before starting data collection to establish a baseline.

2. Starting Data Collection:
   - Turn the switch on to start data collection. You will hear a beep indicating data collection has begun.
   - **Remain Still**: Stay as still as possible for the first few seconds to establish a baseline for non-tremor data.

3. Simulating Tremors:
   - **Mild Tremor**: Gently shake your hand or wrist to simulate a mild tremor. The NeoPixels should light up green if detected.
   - **Moderate Tremor**: Increase the intensity of the shaking to simulate a moderate tremor. The NeoPixels should light up blue if detected.
   - **Severe Tremor**: Shake more vigorously to simulate a severe tremor. The NeoPixels should light up red, and you should hear three beeps if detected.

4. Stopping Data Collection:
   - Turn the switch off to stop data collection. A different beep will sound, indicating that data collection has stopped.

5. Repeat for Calibration:
   - Repeat the process to allow the device to adjust its threshold values and improve accuracy.

# Submission Details
- Author: Matthew Nunez (mn3040)
