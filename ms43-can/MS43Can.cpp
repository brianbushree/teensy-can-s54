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

double MS43_DME2_Frame::engineTempC() {
    return (frame[1] * .75) - 48.373;
}

double MS43_DME2_Frame::engineTempF() {
    return (engineTempC() * 1.8) + 32;
}

int MS43_DME2_Frame::ambientPressure() {
    if (frame[2] == 0x00) {
        return 0;
    } else if (frame[2] == 0xFF) {
        return -1;
    } else {
        return frame[2] != 0xFF ? (frame[2] * 2) + 598 : -1;
    }
}

bool MS43_DME2_Frame::clutchIsPressed() {
    return frame[3] & 0b00000001;
}

bool MS43_DME2_Frame::idleRegulatorOn() {
    return frame[3] & 0b00000010;
}

bool MS43_DME2_Frame::ackFromACC1() {
    return frame[3] & 0b00000100;
}

bool MS43_DME2_Frame::engineIsRunning() {
    return frame[3] & 0b00001000;
}

MS43_DME2_STEERING_WHEEL_CRUISE_STATE MS43_DME2_Frame::steeringWheelCruiseState() {
    return static_cast<MS43_DME2_STEERING_WHEEL_CRUISE_STATE>((frame[3] & 0b11100000) >> 5);
}

// Helper function for pedal values
double pedalValueToPercent(uint8_t val) {
    // init and zero values
    if (val == 0x00 || val == 0x01) {
        return 0;
    } else if (val == 0xFF) {
        return -1;
    } else {
        return val * 0.390625;
    }
}

double MS43_DME2_Frame::virtualCruisePedalPercent() {
    return pedalValueToPercent(frame[4]);
}

double MS43_DME2_Frame::acceleratorPedalPercent() {
    return pedalValueToPercent(frame[5]);
}

bool MS43_DME2_Frame::brakeSwitchActive() {
    return frame[6] & 0b00000001;
}

bool MS43_DME2_Frame::brakeSwitchHasError() {
    return frame[6] & 0b00000010;
}

bool MS43_DME2_Frame::kickdownIsActive() {
    return frame[6] & 0b00000100;
}

MS43_DME2_CRUISE_STATE MS43_DME2_Frame::cruiseControlState() {
    return static_cast<MS43_DME2_CRUISE_STATE>((frame[6] & 0b00111000) >> 3);

}

bool MS43_DME2_Frame::shiftLockRequested() {
    return frame[6] & 0b11000000;
}

MS43_DME3_Frame::MS43_DME3_Frame(uint8_t (&f)[8]) : frame(f) {};

MS43_DME4_Frame::MS43_DME4_Frame(uint8_t (&f)[8]) : frame(f) {};

