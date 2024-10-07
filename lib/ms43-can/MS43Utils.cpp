#include "MS43Can.h"

// Helper function for temp conversion
float celciusToFahrenheit(float celcius) {
  return (celcius * 1.8) + 32;
}

// Helper function for temp conversion  (reverse)
float fahrenheitToCelcius(float fahrenheit) {
  return (fahrenheit - 32) / 1.8;
}

// Helper function for pedal values
float pedalValueToPercent(uint8_t val) {
  // init and zero values
  if (val == 0x00 || val == 0x01) {
    return 0;
  } else if (val == 0xFF) {
    return -1;
  } else {
    return val * 0.390625;
  }
}

int kmhToMph(int kmh) {
  return kmh * 0.621371;
}

int mphToKmh(int mph) {
  return mph * 1.60934;
}
