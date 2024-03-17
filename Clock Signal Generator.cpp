#include <Arduino.h>

// setup LEDs on pins 6, 9, and TX (D3)
void SetupLEDs() {
  DDRD |= (1 << 7);
  DDRB |= (1 << 5);
  DDRD |= (1 << 3);
}

// update LEDs based on the percentage 
void updateLEDs(uint8_t Percent) {
  if (Percent < 25) {
    // turning on pin 6 and turn off pins 9 and TX (D3)
    PORTD |= (1 << 7);
    PORTB &= ~(1 << 5);
    PORTD &= ~(1 << 3);
  } else if (25 < Percent && Percent < 50) {
    // turning on pin 9 and turn off pins 6 and TX (D3)
    PORTB |= (1 << 5);
    PORTD &= ~(1 << 7);
    PORTD &= ~(1 << 3);
  } else if (50 < Percent && Percent < 75) {
    // turning on pin TX (D3) and turn off pins 6 and 9
    PORTD |= (1 << 3);
    PORTB &= ~(1 << 5);
    PORTD &= ~(1 << 7);
  } else if (75 < Percent) {
    // turning on all pins
    PORTD |= (1 << 3);
    PORTB |= (1 << 5);
    PORTD |= (1 << 7);
  }
}

// why Timer 1:
// I setup Timer 1 for PWM so that I could output PWM signals on OC1B
// Timer 1 is a 16-bit timer which allows me to have that higher resolution we want
// by having Timer 1 in Fast PWM mode, I can have PWM signals with different frequencies
// 
// PWM Output Pin: Pad #10 / DB6
//
// Why Pad #10: 
// I chose pin 10 because I know it provides a versatile PWM output capability
// as it is associted with Timer 1 which meets the requirements listed in the exam manual
void SetupTimerPWM() {
  // setting Pad #10 as output for PWM
  DDRB |= (1 << 6);

  // set up Timer 1 for Fast PWM mode
  TCCR1A |= (1 << WGM11) | (1 << WGM10) | (1 << COM1B1);
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);

  // set frequency to 1kHz
  OCR1A = 8000; // set top value for PWM frequency
  OCR1B = 4000; // set compare value for 50% duty cycle
}

// setup ADC for reading potentiometer value
void setupADC() {
  // set Pad #12 as input for ADC because it is associated with ADC channel 1
  // and it is easily accessible
  DDRD &= ~(1 << 6);

  // make ADC use AVcc as reference, chose channel 1 (pin 12)
  ADMUX = (1 << REFS0) | 1;

  // turn on ADC, start conversion, enable auto trigger, set prescaler to 128
  ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // turn off digital input buffers on ADC pins
  DIDR0 = 0;
  DIDR1 = 0;

  // turn on high speed mode
  ADCSRB = (1 << ADHSM) | (1 << MUX5);

  // start the first conversion
  ADCSRA |= (1 << ADSC);
}

uint8_t translateADCtoFreq(uint16_t ADCval) {
  // converting ADC into frequency
  uint8_t freqInHz = ((ADCval * 3000) / 1023) + 1000;

  // makes sure that the frequency remains within 1kHz-4kHz range
  if (freqInHz < 1000) {
    freqInHz = 1000;
  } else if (freqInHz > 4000) {
    freqInHz = 4000;
  }

  return freqInHz;
}

// set PWM frequency while maintaining 50% duty cycle
void SetPWMFreq(uint16_t freqInHz) {
  // calculate OCR value for desired frequency
  uint16_t ocrValue = (F_CPU / (2UL * 1 * freqInHz)) - 1;

  // makes sure that the ocrValue remains within range
  if (ocrValue > 0xFFFF) {
    ocrValue = 0xFFFF;
  } else if (ocrValue == 0) {
    ocrValue = 1;
  }

  // updating OCR1A to change the frequency and OCR1A to maintain 50% duty cycle
  OCR1A = ocrValue;
  OCR1B = ocrValue / 2;
}

void setup() {
  SetupLEDs();
  SetupTimerPWM();
  setupADC();
}

void loop() {
  SetPWMFreq(translateADCtoFreq(ADCW));
  updateLEDs(ADCW / 10.23); // ADCW ranges from 0 to 1023 so convert from 0-100%
  // printing the ADC value
  Serial.println(ADCW / 10.23);
  delay(100); // Update 10 times a second
}