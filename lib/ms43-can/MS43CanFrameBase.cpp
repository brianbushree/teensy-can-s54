#include "MS43Can.h"

MS43_Frame_Receive_Base::MS43_Frame_Receive_Base(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]) {
  update(f);
};

void MS43_Frame_Receive_Base::update(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]) {
  for (int i = 0; i < CAN_FRAME_SIZE_BYTES; i++) {
    frame[i] = f[i];
  }
};

// Init the send frames with zero values
MS43_Frame_Send_Base::MS43_Frame_Send_Base() {}

// Serialize the frame
uint8_t* MS43_Frame_Send_Base::serialize() {
  return frame;
}
