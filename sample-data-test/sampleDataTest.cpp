#include <stdint.h>
#include <iostream>
#include "../ms43-can/MS43Can.h"

int main(int argc, char const *argv[])
{

    uint8_t buffer[8] = { 0x05, 0x0B, 0xB8, 0x0C, 0x0B, 0x0A, 0x00, 0x77 };
    // uint8_t buffer[8] = { 0x05, 0x0C, 0xB8, 0x27, 0x0C, 0x0B, 0x46, 0x80 };
    // uint8_t buffer[8] = { 0x0D, 0x00, 0x84, 0x27, 0x00, 0x0B, 0x31, 0x82 };

    MS43_DME1_Frame f = MS43_DME1_Frame(buffer);
    std::cout << "ignition: " << f.ignitionKeyVoltageIsOn() << "\n";
    std::cout << "crankshaftSensorHasError: " << f.crankshaftSensorHasError() << "\n";
    std::cout << "tractionControlHasError: " << f.tractionControlHasError() << "\n";
    std::cout << "gearChangeIsPossible: " << f.gearChangeIsPossible() << "\n";
    std::cout << "chargeIntrvState: " << f.chargeIntrvState() << "\n";
    std::cout << "mafHasError: " << f.mafHasError() << "\n\n";

    std::cout << "engineSpeedRPM: " << f.engineSpeedRPM() << "\n";
    std::cout << "indexedTorquePercent: " << f.indexedTorquePercent() << "\n";
    std::cout << "indicatedTorquePercent: " << f.indicatedTorquePercent() << "\n";
    std::cout << "lossTorquePercent: " << f.lossTorquePercent() << "\n";
    std::cout << "theorethicalTorquePercent: " << f.theorethicalTorquePercent() << "\n";
    std::cout << "amtHasError: " << f.amtHasError() << "\n\n";


}
