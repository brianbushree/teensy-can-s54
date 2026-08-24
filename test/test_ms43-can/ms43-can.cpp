#include <stdint.h>
#include <iostream>
#include "MS43Can.h"
#include "unity.h"

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

void printICL2(std::ostream &os, uint8_t* data) {
  os << "MS43_ICL2_Frame{" << "\n";
  for (int i = 0; i < CAN_FRAME_SIZE_BYTES; i++) {
    os << " byte" << i << ": " << static_cast<int>(data[i]) << "\n";
  }
  os << "}" << "\n";
}

// The fuel level encoding is the whole point of this frame, so assert the
// serialized bytes rather than just printing them. Both level fields are sent
// as whole liters against a 57L (0x39) full tank.
void test_icl2FuelTankLevel() {
  MS43_ICL2_Frame icl2 = MS43_ICL2_Frame();
  uint8_t* data = icl2.serialize();
  printICL2(std::cout, data);

  // defaults: 65% of a 57L tank is 37L (0x25)
  TEST_ASSERT_EQUAL_UINT8(37, data[2] & 0b01111111);
  // not in reserve, so the top bit stays clear
  TEST_ASSERT_EQUAL_UINT8(0, (data[2] & 0b10000000) >> 7);
  // comfortably above the 8L low fuel light threshold
  TEST_ASSERT_TRUE((data[2] & 0b01111111) > 8);

  // 65% is still in the first drain phase, so the driver side holds the liters
  // sitting above the halfway mark: 37L - 28L = 9L
  TEST_ASSERT_EQUAL_UINT8(9, data[5] & 0b00111111);

  // odometer: 150000km -> raw 15000 (0x3A98), LSB first
  TEST_ASSERT_EQUAL_UINT8(0x98, data[0]);
  TEST_ASSERT_EQUAL_UINT8(0x3A, data[1]);

  // a full tank lands exactly on 57L, split 29L driver / 28L passenger
  icl2.setFuelTankLevelPercent(100);
  TEST_ASSERT_EQUAL_UINT8(57, data[2] & 0b01111111);
  TEST_ASSERT_EQUAL_UINT8(29, data[5] & 0b00111111);

  // at half a tank (28L) the driver side has just run dry
  icl2.setFuelTankLevelPercent(50);
  TEST_ASSERT_EQUAL_UINT8(0, data[5] & 0b00111111);

  // below half the driver side stays empty while the passenger side drains,
  // so a zero driver side is only correct once the total is under halfway
  icl2.setFuelTankLevelPercent(25);
  TEST_ASSERT_EQUAL_UINT8(0, data[5] & 0b00111111);
  TEST_ASSERT_EQUAL_UINT8(14, data[2] & 0b01111111);

  // liters can be set directly, bypassing the percentage conversion
  icl2.setFuelTankLevelLiters(40);
  TEST_ASSERT_EQUAL_UINT8(40, data[2] & 0b01111111);
  TEST_ASSERT_EQUAL_UINT8(12, data[5] & 0b00111111); // 40L - 28L

  // more liters than the tank holds is rejected, leaving the frame untouched
  icl2.setFuelTankLevelLiters(58);
  TEST_ASSERT_EQUAL_UINT8(40, data[2] & 0b01111111);

  // out of range percentages are rejected, leaving the frame untouched
  icl2.setFuelTankLevelPercent(101);
  TEST_ASSERT_EQUAL_UINT8(40, data[2] & 0b01111111);

  // the reserve bit sets without disturbing the level bits
  icl2.setFuelTankLevelPercent(5);
  icl2.setFuelTankReserveEnabled(true);
  TEST_ASSERT_EQUAL_UINT8(1, (data[2] & 0b10000000) >> 7);
  TEST_ASSERT_EQUAL_UINT8(0x02, data[2] & 0b01111111);

  // running clock is stored LSB first across bytes 3-4
  icl2.setRunningClockMinutes(300);
  TEST_ASSERT_EQUAL_UINT8(300 & 0xFF, data[3]);
  TEST_ASSERT_EQUAL_UINT8(300 >> 8, data[4]);
}

void test_ms43PrintValues() {
  uint8_t dme1[CAN_FRAME_SIZE_BYTES] = { 0x05, 0x0B, 0xB8, 0x0C, 0x0B, 0x0A, 0x00, 0x77 };
  // uint8_t dme1[CAN_FRAME_SIZE_BYTES] = { 0x05, 0x0C, 0xB8, 0x27, 0x0C, 0x0B, 0x46, 0x80 };
  // uint8_t dme1[CAN_FRAME_SIZE_BYTES] = { 0x0D, 0x00, 0x84, 0x27, 0x00, 0x0B, 0x31, 0x82 };
  MS43_DME1_Frame d1 = MS43_DME1_Frame(dme1);
  printDME1(std::cout, d1);

  //uint8_t dme2[CAN_FRAME_SIZE_BYTES] = { 0x4F, 0xB2, 0x82, 0x10, 0x00, 0x20, 0xFF, 0x00 };
  //uint8_t dme2[CAN_FRAME_SIZE_BYTES] = { 0x4F, 0xB0, 0x82, 0x10, 0x00, 0x32, 0xFF, 0x00 };
  uint8_t dme2[CAN_FRAME_SIZE_BYTES] = { 0x4F, 0xB1, 0x84, 0x10, 0x00, 0x51, 0xFF, 0x00 };
  MS43_DME2_Frame d2 = MS43_DME2_Frame(dme2);
  printDME2(std::cout, d2);

  uint8_t dme3[CAN_FRAME_SIZE_BYTES] = { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
  // uint8_t dme3[CAN_FRAME_SIZE_BYTES] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 };
  MS43_DME3_Frame d3 = MS43_DME3_Frame(dme3);
  printDME3(std::cout, d3);

  //uint8_t dme4[CAN_FRAME_SIZE_BYTES] = { 0xF6, 0x61, 0x01, 0x89, 0x00, 0x00, 0x00, 0x00 };
  //uint8_t dme4[CAN_FRAME_SIZE_BYTES] = { 0xEE, 0xFA, 0x02, 0x89, 0x00, 0x00, 0x00, 0x00 };
  uint8_t dme4[CAN_FRAME_SIZE_BYTES] = { 0xEE, 0x8B, 0x02, 0x88, 0x00, 0x00, 0x00, 0x00 };
  MS43_DME4_Frame d4 = MS43_DME4_Frame(dme4);
  printDME4(std::cout, d4);

  TEST_ASSERT_TRUE(true);
}

void setUp(void) {
}

void tearDown(void) {
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_ms43PrintValues);
  RUN_TEST(test_icl2FuelTankLevel);
  return UNITY_END();
}

int main(void) {
  return runUnityTests();
}
