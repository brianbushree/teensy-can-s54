#include "MS43Can.h"

MS43_DME1_Frame::MS43_DME1_Frame(uint8_t (&f)[8]) : frame(f) {};

bool MS43_DME1_Frame::ignitionKeyVoltageIsOn() const {
  return frame[0] & 0b00000001;
}

bool MS43_DME1_Frame::crankshaftSensorHasError() const {
  return frame[0] & 0b00000010;
}

bool MS43_DME1_Frame::tractionControlHasError() const {
  return frame[0] & 0b00000100;
}

bool MS43_DME1_Frame::gearChangeIsPossible() const {
  return frame[0] & 0b00001000;
}

MS43_DME1_CHARGE_INTRV_STATE MS43_DME1_Frame::chargeIntrvState() const {
  return static_cast<MS43_DME1_CHARGE_INTRV_STATE>((frame[0] & 0b00110000) >> 4);
}

bool MS43_DME1_Frame::mafHasError() const {
  return frame[0] & 0b10000000;
}

float MS43_DME1_Frame::indexedTorquePercent() const {
  return frame[1] * 0.39;
}

int MS43_DME1_Frame::engineSpeedRPM() const {
  return ((frame[3] * 256) + frame[2]) * 0.15625;
}

float MS43_DME1_Frame::indicatedTorquePercent() const {
  return frame[4] * 0.39;
}

float MS43_DME1_Frame::lossTorquePercent() const {
  return frame[5] * 0.39;
}

bool MS43_DME1_Frame::amtHasError() const {
  return frame[6] & 0b00000011;
}

float MS43_DME1_Frame::theorethicalTorquePercent() const {
  return frame[7] * 0.39;
}

float celciusToFahrenheit(float celcius) {
    return (celcius * 1.8) + 32;
}


MS43_DME2_Frame::MS43_DME2_Frame(uint8_t (&f)[8]) : frame(f) {};

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

MS43_DME3_Frame::MS43_DME3_Frame(uint8_t (&f)[8]) : frame(f) {};

MS43_DME3_SPORT_BUTTON_STATUS MS43_DME3_Frame::sportButtonStatus() const {
  return static_cast<MS43_DME3_SPORT_BUTTON_STATUS>(frame[2]);
}

MS43_DME4_Frame::MS43_DME4_Frame(uint8_t (&f)[8]) : frame(f) {};

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

u_int16_t MS43_DME4_Frame::fuelConsumptionCounter() const {
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
