#include <stdint.h>
#include <iostream>
#include "../ms43-can/MS43Can.h"

void printDME1(std::ostream &os, const MS43_DME1_Frame &dme1) {
  os << "MS43_DME1_Frame{" << "\n";
  os << " ignition: " << dme1.ignitionKeyVoltageIsOn() << "\n";
  os << " crankshaftSensorHasError: " << dme1.crankshaftSensorHasError() << "\n";
  os << " tractionControlHasError: " << dme1.tractionControlHasError() << "\n";
  os << " gearChangeIsPossible: " << dme1.gearChangeIsPossible() << "\n";
  os << " chargeIntrvState: " << dme1.chargeIntrvState() << "\n";
  os << " mafHasError: " << dme1.mafHasError() << "\n";
  os << " engineSpeedRPM: " << dme1.engineSpeedRPM() << "\n";
  os << " indexedTorquePercent: " << dme1.indexedTorquePercent() << "\n";
  os << " indicatedTorquePercent: " << dme1.indicatedTorquePercent() << "\n";
  os << " lossTorquePercent: " << dme1.lossTorquePercent() << "\n";
  os << " theorethicalTorquePercent: " << dme1.theorethicalTorquePercent() << "\n";
  os << " amtHasError: " << dme1.amtHasError() << "\n";
  os << "}" << "\n";
}

void printDME2(std::ostream &os, const MS43_DME2_Frame &dme2) {
  os << "MS43_DME2_Frame{" << "\n";
  os << " engineTempC: " << dme2.engineTempC() << "\n";
  os << " engineTempF: " << dme2.engineTempF() << "\n";
  os << " ambientPressure: " << dme2.ambientPressure() << "\n";
  os << " clutchIsPressed: " << dme2.clutchIsPressed() << "\n";
  os << " idleRegulatorOn: " << dme2.idleRegulatorOn() << "\n";
  os << " ackFromACC1: " << dme2.ackFromACC1() << "\n";
  os << " engineIsRunning: " << dme2.engineIsRunning() << "\n";
  os << " steeringWheelCruiseState: " << dme2.steeringWheelCruiseState() << "\n";
  os << " virtualCruisePedalPercent: " << dme2.virtualCruisePedalPercent() << "\n";
  os << " acceleratorPedalPercent: " << dme2.acceleratorPedalPercent() << "\n";
  os << " brakeSwitchActive: " << dme2.brakeSwitchActive() << "\n";
  os << " brakeSwitchHasError: " << dme2.brakeSwitchHasError() << "\n";
  os << " kickdownIsActive: " << dme2.kickdownIsActive() << "\n";
  os << " cruiseControlState: " << dme2.cruiseControlState() << "\n";
  os << " shiftLockRequested: " << dme2.shiftLockRequested() << "\n";
  os << "}" << "\n";
}

void printDME3(std::ostream &os, const MS43_DME3_Frame &dme3) {
  os << "MS43_DME3_Frame{" << "\n";
  os << " sportButtonStatus: " << dme3.sportButtonStatus() << "\n";
  os << "}" << "\n";
}

void printDME4(std::ostream &os, const MS43_DME4_Frame &dme4) {
  os << "MS43_DME4_Frame{" << "\n";
  os << " checkEngineLightOn: " << dme4.checkEngineLightOn() << "\n";
  os << " cruiseControlLightOn: " << dme4.cruiseControlLightOn() << "\n";
  os << " emlLightOn: " << dme4.emlLightOn() << "\n";
  os << " fuelTankCapLight: " << dme4.fuelTankCapLight() << "\n";
  os << " fuelConsumptionCounter: " << dme4.fuelConsumptionCounter() << "\n";
  os << " oilLevelErrorLightOnM5One: " << dme4.oilLevelErrorLightOnM5One() << "\n";
  os << " oilLevelErrorLightOn: " << dme4.oilLevelErrorLightOn() << "\n";
  os << " oilLevelErrorLightOnM5Two: " << dme4.oilLevelErrorLightOnM5Two() << "\n";
  os << " coolantOverheatingLightOn: " << dme4.coolantOverheatingLightOn() << "\n";
  os << " warmUpHighLightOn: " << dme4.warmUpHighLightOn() << "\n";
  os << " warmUpMediumLightOn: " << dme4.warmUpMediumLightOn() << "\n";
  os << " warmUpLowLightOn: " << dme4.warmUpLowLightOn() << "\n";
  os << " oilTempC: " << dme4.oilTempC() << "\n";
  os << " oilTempF: " << dme4.oilTempF() << "\n";
  os << " batteryChargeLightOn: " << dme4.batteryChargeLightOn() << "\n";
  os << " oilLevelLiters: " << dme4.oilLevelLiters() << "\n";
  os << " tirePressureState: " << dme4.tirePressureState() << "\n";
  os << " engineOilPressureLow: " << dme4.engineOilPressureLow() << "\n";
  os << "}" << "\n";
}

int main(int argc, char const *argv[])
{
  uint8_t dme1[8] = { 0x05, 0x0B, 0xB8, 0x0C, 0x0B, 0x0A, 0x00, 0x77 };
  // uint8_t dme1[8] = { 0x05, 0x0C, 0xB8, 0x27, 0x0C, 0x0B, 0x46, 0x80 };
  // uint8_t dme1[8] = { 0x0D, 0x00, 0x84, 0x27, 0x00, 0x0B, 0x31, 0x82 };
  MS43_DME1_Frame d1 = MS43_DME1_Frame(dme1);
  printDME1(std::cout, d1);

  //uint8_t dme2[8] = { 0x4F, 0xB2, 0x82, 0x10, 0x00, 0x20, 0xFF, 0x00 };
  //uint8_t dme2[8] = { 0x4F, 0xB0, 0x82, 0x10, 0x00, 0x32, 0xFF, 0x00 };
  uint8_t dme2[8] = { 0x4F, 0xB1, 0x84, 0x10, 0x00, 0x51, 0xFF, 0x00 };
  MS43_DME2_Frame d2 = MS43_DME2_Frame(dme2);
  printDME2(std::cout, d2);

  uint8_t dme3[8] = { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
  // uint8_t dme3[8] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 };
  MS43_DME3_Frame d3 = MS43_DME3_Frame(dme3);
  printDME3(std::cout, d3);

  //uint8_t dme4[8] = { 0xF6, 0x61, 0x01, 0x89, 0x00, 0x00, 0x00, 0x00 };
  //uint8_t dme4[8] = { 0xEE, 0xFA, 0x02, 0x89, 0x00, 0x00, 0x00, 0x00 };
  uint8_t dme4[8] = { 0xEE, 0x8B, 0x02, 0x88, 0x00, 0x00, 0x00, 0x00 };
  MS43_DME4_Frame d4 = MS43_DME4_Frame(dme4);
  printDME4(std::cout, d4);
}
