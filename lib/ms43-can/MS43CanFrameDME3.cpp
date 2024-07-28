#include "MS43Can.h"

MS43_DME3_Frame::MS43_DME3_Frame(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]) : MS43_Frame_Receive_Base(f) {};

MS43_DME3_SPORT_BUTTON_STATUS MS43_DME3_Frame::sportButtonStatus() const {
  return static_cast<MS43_DME3_SPORT_BUTTON_STATUS>(frame[2]);
}
