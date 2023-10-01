#include "MS43Can.h"

MS43_DME3_Frame::MS43_DME3_Frame(uint8_t (&f)[8]) : frame(f) {};

MS43_DME3_SPORT_BUTTON_STATUS MS43_DME3_Frame::sportButtonStatus() const {
  return static_cast<MS43_DME3_SPORT_BUTTON_STATUS>(frame[2]);
}
