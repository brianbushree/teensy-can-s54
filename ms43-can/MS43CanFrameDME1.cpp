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
