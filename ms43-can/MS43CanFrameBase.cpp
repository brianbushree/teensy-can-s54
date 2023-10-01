#include "MS43Can.h"

MS43_Frame_Base::MS43_Frame_Base(uint8_t (&f)[8]) : frame(f) {};

// Helper function for temp conversion
float MS43_Frame_Base::celciusToFahrenheit(float celcius) const {
  return (celcius * 1.8) + 32;
}

// Helper function for pedal values
float MS43_Frame_Base::pedalValueToPercent(uint8_t val) const {
  // init and zero values
  if (val == 0x00 || val == 0x01) {
    return 0;
  } else if (val == 0xFF) {
    return -1;
  } else {
    return val * 0.390625;
  }
}
