#include "MS43Can.h"

MS43_Frame_Receive_Base::MS43_Frame_Receive_Base(const uint8_t (&f)[8]) : frame(f) {};

// Helper function for temp conversion
float MS43_Frame_Receive_Base::celciusToFahrenheit(float celcius) const {
  return (celcius * 1.8) + 32;
}

// Helper function for pedal values
float MS43_Frame_Receive_Base::pedalValueToPercent(uint8_t val) const {
  // init and zero values
  if (val == 0x00 || val == 0x01) {
    return 0;
  } else if (val == 0xFF) {
    return -1;
  } else {
    return val * 0.390625;
  }
}

// Init the send frames with zero values
MS43_Frame_Send_Base::MS43_Frame_Send_Base() :
 frame{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} {}
