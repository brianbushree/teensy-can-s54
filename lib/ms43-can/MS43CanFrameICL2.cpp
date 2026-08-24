#include "MS43Can.h"

// Fuel level fields in this frame are sent as whole liters, not as a
// percentage or an abstract scale.
//
// Byte 2 (bits 0-6) counts down in liters from a full tank toward 0x00, with
// the low fuel light coming on around 8L remaining. 0x39 is 57 decimal, which
// lines up with the E46's ~63L tank measured to the sender's full mark.
const uint8_t FUEL_TANK_FULL_LITERS = 0x39;

// Liters held by each side of the saddle tank. The driver side drains first,
// emptying over the top half of the tank, then reads zero while the passenger
// side drains over the bottom half.
const uint8_t FUEL_TANK_SIDE_LITERS = FUEL_TANK_FULL_LITERS / 2;

// Largest value the 6 bit driver side field can carry, in liters
const uint8_t FUEL_TANK_SIDE_MAX_LITERS = 63;

MS43_ICL2_Frame::MS43_ICL2_Frame() : MS43_Frame_Send_Base() {
  setOdometerKm(150000); // plausible reading for a car of this age
  setFuelTankReserveEnabled(false);
  setRunningClockMinutes(0);
  setFuelTankLevelPercent(65); // also seeds the driver side level in liters
}

void MS43_ICL2_Frame::setOdometerKm(uint32_t km) {
  if (km > 655350) {
    return;
  }

  uint16_t raw = km / 10;

  frame[0] = raw & 0x00FF;
  frame[1] = (raw & 0xFF00) >> 8;
}

void MS43_ICL2_Frame::setFuelTankLevelPercent(uint8_t percent) {
  if (percent > 100) {
    return;
  }

  // the frame carries liters, so convert the percentage before writing
  uint8_t totalLiters = (percent * FUEL_TANK_FULL_LITERS) / 100;

  setFuelTankLevelLiters(totalLiters);
}

void MS43_ICL2_Frame::setFuelTankLevelLiters(uint8_t liters) {
  if (liters > FUEL_TANK_FULL_LITERS) {
    return;
  }

  frame[2] = (frame[2] & 0b10000000) | (liters & 0b01111111);

  // The saddle tank drains sequentially rather than evenly, so the driver side
  // holds only the liters sitting above the halfway mark. Deriving it here
  // keeps the two fields from contradicting each other, which is what the
  // siphon pump failure and fuel level plausibility diagnostics look for.
  uint8_t driverSideLiters = (liters > FUEL_TANK_SIDE_LITERS)
    ? liters - FUEL_TANK_SIDE_LITERS
    : 0;

  setFuelTankLevelDriverSideLiters(driverSideLiters);
}

void MS43_ICL2_Frame::setFuelTankReserveEnabled(bool enabled) {
  frame[2] = (frame[2] & 0b01111111) | (enabled << 7);
}

void MS43_ICL2_Frame::setRunningClockMinutes(uint16_t minutes) {
  frame[3] = minutes & 0x00FF;
  frame[4] = (minutes & 0xFF00) >> 8;
}

void MS43_ICL2_Frame::setFuelTankLevelDriverSideLiters(uint8_t liters) {
  if (liters > FUEL_TANK_SIDE_MAX_LITERS) {
    return;
  }
  frame[5] = (frame[5] & 0b11000000) | (liters & 0b00111111);
}
