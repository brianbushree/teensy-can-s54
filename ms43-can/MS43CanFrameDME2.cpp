#include "MS43Can.h"

MS43_DME2_Frame::MS43_DME2_Frame(uint8_t (&f)[8]) : MS43_Frame_Base(f) {};

int MS43_DME2_Frame::engineTempC() const {
  return (frame[1] * .75) - 48.373;
}

int MS43_DME2_Frame::engineTempF() const {
  return celciusToFahrenheit(engineTempC());
}

int MS43_DME2_Frame::ambientPressure() const {
  if (frame[2] == 0x00) {
    return 0;
  } else if (frame[2] == 0xFF) {
    return -1;
  } else {
    return frame[2] != 0xFF ? (frame[2] * 2) + 598 : -1;
  }
}

bool MS43_DME2_Frame::clutchIsPressed() const {
  return frame[3] & 0b00000001;
}

bool MS43_DME2_Frame::idleRegulatorOn() const {
  return frame[3] & 0b00000010;
}

bool MS43_DME2_Frame::ackFromACC1() const {
  return frame[3] & 0b00000100;
}

bool MS43_DME2_Frame::engineIsRunning() const {
  return frame[3] & 0b00001000;
}

MS43_DME2_STEERING_WHEEL_CRUISE_STATE MS43_DME2_Frame::steeringWheelCruiseState() const {
  return static_cast<MS43_DME2_STEERING_WHEEL_CRUISE_STATE>((frame[3] & 0b11100000) >> 5);
}

float MS43_DME2_Frame::virtualCruisePedalPercent() const {
  return pedalValueToPercent(frame[4]);
}

float MS43_DME2_Frame::acceleratorPedalPercent() const {
  return pedalValueToPercent(frame[5]);
}

bool MS43_DME2_Frame::brakeSwitchActive() const {
  return frame[6] & 0b00000001;
}

bool MS43_DME2_Frame::brakeSwitchHasError() const {
  return frame[6] & 0b00000010;
}

bool MS43_DME2_Frame::kickdownIsActive() const {
  return frame[6] & 0b00000100;
}

MS43_DME2_CRUISE_STATE MS43_DME2_Frame::cruiseControlState() const {
  return static_cast<MS43_DME2_CRUISE_STATE>((frame[6] & 0b00111000) >> 3);
}

bool MS43_DME2_Frame::shiftLockRequested() const {
  return frame[6] & 0b11000000;
}
