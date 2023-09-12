#include "MS43Can.h"

MS43_DME1_Frame::MS43_DME1_Frame(uint8_t (&f)[8]) : frame(f) {};

bool MS43_DME1_Frame::ignitionKeyVoltageIsOn() {
    return frame[0] & 0b00000001;
}

bool MS43_DME1_Frame::crankshaftSensorHasError() {
    return frame[0] & 0b00000010;
}

bool MS43_DME1_Frame::tractionControlHasError() {
    return frame[0] & 0b00000100;
}

bool MS43_DME1_Frame::gearChangeIsPossible() {
    return frame[0] & 0b00001000;
}

MS43_DME1_CHARGE_INTRV_STATE MS43_DME1_Frame::chargeIntrvState() {
    return static_cast<MS43_DME1_CHARGE_INTRV_STATE>((frame[0] & 0b00110000) >> 4);
}

bool MS43_DME1_Frame::mafHasError() {
    return frame[0] & 0b10000000;
}

double MS43_DME1_Frame::indexedTorquePercent() {
    return frame[1] * 0.39;
}

int MS43_DME1_Frame::engineSpeedRPM() {
    return ((frame[3] * 256) + frame[2]) * 0.15625;
}

double MS43_DME1_Frame::indicatedTorquePercent() {
    return frame[4] * 0.39;
}

double MS43_DME1_Frame::lossTorquePercent() {
    return frame[5] * 0.39;
}

bool MS43_DME1_Frame::amtHasError() {
    return frame[6] & 0b00000011;
}

double MS43_DME1_Frame::theorethicalTorquePercent() {
    return frame[7] * 0.39;
}


MS43_DME2_Frame::MS43_DME2_Frame(uint8_t (&f)[8]) : frame(f) {};

MS43_DME3_Frame::MS43_DME3_Frame(uint8_t (&f)[8]) : frame(f) {};

MS43_DME4_Frame::MS43_DME4_Frame(uint8_t (&f)[8]) : frame(f) {};
