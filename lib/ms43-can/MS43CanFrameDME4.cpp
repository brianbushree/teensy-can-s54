#include "MS43Can.h"

MS43_DME4_Frame::MS43_DME4_Frame(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]) : MS43_Frame_Receive_Base(f) {};

bool MS43_DME4_Frame::checkEngineLightOn() const {
  return frame[0] & 0b00000010;
}

bool MS43_DME4_Frame::cruiseControlLightOn() const {
  return frame[0] & 0b00001000;
}

bool MS43_DME4_Frame::emlLightOn() const {
  return frame[0] & 0b00010000;
}

bool MS43_DME4_Frame::fuelTankCapLight() const {
  return frame[0] & 0b01000000;
}

uint16_t MS43_DME4_Frame::fuelConsumptionCounter() const {
  return ((uint16_t)frame[2] << 8) | frame[1];
}

bool MS43_DME4_Frame::oilLevelErrorLightOnM5One() const {
  return frame[3] & 0b00000001;
}

bool MS43_DME4_Frame::oilLevelErrorLightOn() const {
  return frame[3] & 0b00000010;
}

bool MS43_DME4_Frame::oilLevelErrorLightOnM5Two() const {
  return frame[3] & 0b00000100;
}

bool MS43_DME4_Frame::coolantOverheatingLightOn() const {
  return frame[3] & 0b00001000;
}

bool MS43_DME4_Frame::warmUpHighLightOn() const {
  return frame[3] & 0b00010000;
}

bool MS43_DME4_Frame::warmUpMediumLightOn() const {
  return frame[3] & 0b00100000;
}

bool MS43_DME4_Frame::warmUpLowLightOn() const {
  return frame[3] & 0b01000000;
}

int MS43_DME4_Frame::oilTempC() const {
  return frame[4] - 48;
}

int MS43_DME4_Frame::oilTempF() const {
  return celciusToFahrenheit(oilTempC());
}

bool MS43_DME4_Frame::batteryChargeLightOn() const {
  return frame[5] & 0b00000001;
}

float MS43_DME4_Frame::oilLevelLiters() const {
  return ((double)frame[6] - 158) / 10;
}

bool MS43_DME4_Frame::tirePressureState() const {
  return frame[7] & 0b00000001;
}

bool MS43_DME4_Frame::engineOilPressureLow() const {
  return frame[7] & 0b10000000;
}
