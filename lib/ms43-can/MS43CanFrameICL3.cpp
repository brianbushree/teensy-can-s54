#include "MS43Can.h"

MS43_ICL3_Frame::MS43_ICL3_Frame() : MS43_Frame_Send_Base() {
  setTorqueOffsetForACCompressor(0);
  setRequestForLoweringCoolingTempEnabled(false);
  setACCompressorStatusEnabled(false);
  setAirConditioningRequestEnabled(false);
  setIncreasedHeatRequestEnabled(false);
  setTrailerOperationModeEnabled(false);
  setDayNightLightingEnabled(false);
  setHoodSwitchEnabled(false);
  setElectricCoolingFanLevel(0);
  setRequestRaisedIdleEnabled(false);
  setAmbientAirTempF(75); // 75°F seems like a good default
  setDoorSwitchEnabled(false);
  setHandBrakeSwitchEnabled(false);
  setSuspensionLevel(0);
  setDisplayedVehicleSpeedKph(0); // this may be a bad idea???
}

void MS43_ICL3_Frame::setTorqueOffsetForACCompressor(int torqueOffset) {
  if (torqueOffset < 0 || torqueOffset > 31) {
    return;
  }
  frame[0] = (frame[0] & 0b11100000) | (torqueOffset & 0b00011111);
}

void MS43_ICL3_Frame::setRequestForLoweringCoolingTempEnabled(bool enabled) {
  frame[0] = (frame[0] & 0b11011111) | (enabled << 5);
}

void MS43_ICL3_Frame::setACCompressorStatusEnabled(bool enabled) {
  frame[0] = (frame[0] & 0b10111111) | (enabled << 6);
  frame[4] = (frame[4] & 0b10111111) | (enabled << 6);
}

void MS43_ICL3_Frame::setAirConditioningRequestEnabled(bool enabled) {
  frame[0] = (frame[0] & 0b01111111) | (enabled << 7);
  frame[4] = (frame[4] & 0b01111111) | (enabled << 7);
}

void MS43_ICL3_Frame::setIncreasedHeatRequestEnabled(bool enabled) {
  frame[1] = (frame[1] & 0b11111110) | (enabled);
}

void MS43_ICL3_Frame::setTrailerOperationModeEnabled(bool enabled) {
  frame[1] = (frame[1] & 0b11111101) | (enabled << 1);
}

void MS43_ICL3_Frame::setDayNightLightingEnabled(bool nightEnabled) {
  frame[1] = (frame[1] & 0b11111011) | (nightEnabled << 2);
}

void MS43_ICL3_Frame::setHoodSwitchEnabled(bool enabled) {
  frame[1] = (frame[1] & 0b11110111) | (enabled << 3);
}

void MS43_ICL3_Frame::setElectricCoolingFanLevel(int level) {
  if (level < 0 || level > 15) {
    return;
  }
  frame[1] = (frame[1] & 0b00001111) | ((level & 0b00001111) << 4);
}

void MS43_ICL3_Frame::setRequestRaisedIdleEnabled(bool enabled) {
  frame[2] = (frame[2] & 0b10111111) | (enabled << 6);
}

void MS43_ICL3_Frame::setAmbientAirTempC(int tempC) {
  if (tempC < -40 || tempC > 49) {
    return;
  }
  frame[3] = tempC;
}

void MS43_ICL3_Frame::setAmbientAirTempF(int tempF) {
  setAmbientAirTempC(fahrenheitToCelcius(tempF));
}

void MS43_ICL3_Frame::setDoorSwitchEnabled(bool enabled) {
  frame[4] = (frame[4] & 0b11111110) | (enabled);
}

void MS43_ICL3_Frame::setHandBrakeSwitchEnabled(bool enabled) {
  frame[4] = (frame[4] & 0b11111101) | (enabled << 1);
}

void MS43_ICL3_Frame::setSuspensionLevel(int level) {
  if (level < 0 || level > 3) {
    return;
  }
  frame[4] = (frame[4] & 0b11110011) | ((level & 0b00000011) << 2);
}

void MS43_ICL3_Frame::setDisplayedVehicleSpeedKph(uint16_t speedKph) {
  if (speedKph < 0 || speedKph > 1023) {
    return;
  }

  uint8_t partOne = speedKph & 0b0000000000000011;
  uint8_t partTwo = (speedKph & 0b0000001111111100) >> 2;

  frame[5] = (frame[5] & 0b00111111) | (partOne << 6);
  frame[6] = partTwo;
}

void MS43_ICL3_Frame::setDisplayedVehicleSpeedMph(uint16_t speedMph) {
  setDisplayedVehicleSpeedKph(mphToKmh(speedMph));
}
