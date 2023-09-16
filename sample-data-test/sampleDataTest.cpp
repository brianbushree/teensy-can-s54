#include <stdint.h>
#include <iostream>
#include "../ms43-can/MS43Can.h"

int main(int argc, char const *argv[])
{

    uint8_t dme1[8] = { 0x05, 0x0B, 0xB8, 0x0C, 0x0B, 0x0A, 0x00, 0x77 };
    // uint8_t dme1[8] = { 0x05, 0x0C, 0xB8, 0x27, 0x0C, 0x0B, 0x46, 0x80 };
    // uint8_t dme1[8] = { 0x0D, 0x00, 0x84, 0x27, 0x00, 0x0B, 0x31, 0x82 };

    MS43_DME1_Frame d1 = MS43_DME1_Frame(dme1);
    std::cout << "ignition: " << d1.ignitionKeyVoltageIsOn() << "\n";
    std::cout << "crankshaftSensorHasError: " << d1.crankshaftSensorHasError() << "\n";
    std::cout << "tractionControlHasError: " << d1.tractionControlHasError() << "\n";
    std::cout << "gearChangeIsPossible: " << d1.gearChangeIsPossible() << "\n";
    std::cout << "chargeIntrvState: " << d1.chargeIntrvState() << "\n";
    std::cout << "mafHasError: " << d1.mafHasError() << "\n\n";

    std::cout << "engineSpeedRPM: " << d1.engineSpeedRPM() << "\n";
    std::cout << "indexedTorquePercent: " << d1.indexedTorquePercent() << "\n";
    std::cout << "indicatedTorquePercent: " << d1.indicatedTorquePercent() << "\n";
    std::cout << "lossTorquePercent: " << d1.lossTorquePercent() << "\n";
    std::cout << "theorethicalTorquePercent: " << d1.theorethicalTorquePercent() << "\n";
    std::cout << "amtHasError: " << d1.amtHasError() << "\n\n";

    //uint8_t dme2[8] = { 0x4F, 0xB2, 0x82, 0x10, 0x00, 0x20, 0xFF, 0x00 };
    //uint8_t dme2[8] = { 0x4F, 0xB0, 0x82, 0x10, 0x00, 0x32, 0xFF, 0x00 };
    uint8_t dme2[8] = { 0x4F, 0xB1, 0x84, 0x10, 0x00, 0x51, 0xFF, 0x00 };
    MS43_DME2_Frame d2 = MS43_DME2_Frame(dme2);

    std::cout << "engineTempC: " << d2.engineTempC() << "\n";
    std::cout << "engineTempF: " << d2.engineTempF() << "\n";
    std::cout << "ambientPressure: " << d2.ambientPressure() << "\n";
    std::cout << "clutchIsPressed: " << d2.clutchIsPressed() << "\n";
    std::cout << "idleRegulatorOn: " << d2.idleRegulatorOn() << "\n";
    std::cout << "ackFromACC1: " << d2.ackFromACC1() << "\n";
    std::cout << "engineIsRunning: " << d2.engineIsRunning() << "\n";
    std::cout << "steeringWheelCruiseState: " << d2.steeringWheelCruiseState() << "\n";
    std::cout << "virtualCruisePedalPercent: " << d2.virtualCruisePedalPercent() << "\n";
    std::cout << "acceleratorPedalPercent: " << d2.acceleratorPedalPercent() << "\n";
    std::cout << "brakeSwitchActive: " << d2.brakeSwitchActive() << "\n";
    std::cout << "brakeSwitchHasError: " << d2.brakeSwitchHasError() << "\n";
    std::cout << "kickdownIsActive: " << d2.kickdownIsActive() << "\n";
    std::cout << "cruiseControlState: " << d2.cruiseControlState() << "\n";
    std::cout << "shiftLockRequested: " << d2.shiftLockRequested() << "\n";
}
