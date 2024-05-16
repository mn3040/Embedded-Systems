# Tremor Detection and PWM Control using Adafruit Circuit Playground Classic

This project consists of two parts: a tremor detection system using FFT (Fast Fourier Transform) to analyze accelerometer data, and a PWM (Pulse Width Modulation) control system based on potentiometer input.

## Youtube Presentation Link
Check out our project on Youtube: [PetPal on Devpost](https://youtu.be/1L3QSWorLYs)


## Features

### Tremor Detection
- **Switch-Controlled Data Collection:** The system starts data collection when the switch is turned on and stops when the switch is turned off.
- **Real-Time FFT Processing:** The system processes accelerometer data in real-time using FFT to detect potential tremors.
- **Visual and Audio Feedback:** Different tremor severities are indicated using NeoPixels and tones.
  - **No Tremor:** No NeoPixels are lit.
  - **Mild Tremor:** NeoPixels 1-5 light up green.
  - **Moderate Tremor:** NeoPixels 1-8 light up blue.
  - **Severe Tremor:** NeoPixels 1-10 light up red and a beeping sound is emitted.
- **Adjustable Thresholds:** Thresholds for tremor detection are adjusted based on collected data.

### PWM Control
- **LED Indicators:** LEDs on pins 6, 9, and TX (D3) indicate different ranges of the potentiometer value.
- **PWM Output:** PWM signal is generated on pin 10, with the frequency controlled by the potentiometer input.

## Hardware Requirements
- Adafruit Circuit Playground Classic
- Potentiometer connected to pin 12 (ADC input)
- LEDs connected to pins 6, 9, and TX (D3)

## Software Requirements
- Arduino IDE with Adafruit Circuit Playground library installed
- PlatformIO (for tremor detection part)

## Installation
1. **Install Dependencies:**
   - Install the Adafruit Circuit Playground library from the Arduino Library Manager.
   - Install the arduinoFFT library from the Arduino Library Manager.

2. **Load Code:**
   - Open the provided sketch in the Arduino IDE.
   - Upload the sketch to the Adafruit Circuit Playground Classic.

## Usage

### Tremor Detection

1. **Start Data Collection:**
   - Turn the switch on to start data collection. You should hear a beep sound.
   - Turn the switch off to stop data collection. A different beep sound will indicate the end of data collection.

2. **Interpret Results:**
   - If a potential tremor is detected, white NeoPixels will blink.
   - Depending on the severity of the tremor, NeoPixels will light up in different colors:
     - Mild Tremor: Green (NeoPixels 1-5)
     - Moderate Tremor: Blue (NeoPixels 1-8)
     - Severe Tremor: Red (NeoPixels 1-10) with three beeps.

### PWM Control

1. **Connect Potentiometer:**
   - Connect a potentiometer to pin 12 (ADC input).

2. **Observe LED Indicators:**
   - LEDs on pins 6, 9, and TX (D3) will light up based on the potentiometer value:
     - Less than 25%: LED on pin 6.
     - 25-50%: LED on pin 9.
     - 50-75%: LED on TX (D3).
     - More than 75%: All LEDs on.

3. **PWM Output:**
   - The frequency of the PWM signal on pin 10 will vary from 1 kHz to 4 kHz based on the potentiometer input.

## Code Overview

### Tremor Detection Code

The tremor detection code uses the Adafruit Circuit Playground's accelerometer to collect data. The data is processed using FFT to detect the presence and severity of tremors.

- **Collect Data:** The `collectAccelerometerData()` function collects accelerometer data.
- **Process FFT:** The `processFFT()` function processes the collected data using FFT.
- **Determine Tremor Presence:** The `determineTremorPresence()` function determines the presence and severity of tremors based on the FFT results.
- **Display Severity:** The `displaySeverity()` function displays the severity of detected tremors using NeoPixels.

### PWM Control Code

The PWM control code uses a potentiometer to control the frequency of a PWM signal and LEDs to indicate the potentiometer's value range.

- **Setup:** The `SetupLEDs()`, `SetupTimerPWM()`, and `setupADC()` functions initialize the LEDs, PWM, and ADC respectively.
- **Update LEDs:** The `updateLEDs()` function updates the LEDs based on the potentiometer value.
- **Set PWM Frequency:** The `SetPWMFreq()` function sets the frequency of the PWM signal based on the potentiometer input.

## Troubleshooting

- Ensure all connections are secure.
- Verify the correct pin assignments.
- If the code takes up too much memory, reduce the sample size as indicated in the comments.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments
- Adafruit for the Circuit Playground and supporting libraries.
- ArduinoFFT library for FFT processing.
