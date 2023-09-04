#include <FlexCAN_T4.h>
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can; // For CAN communications between devices, use the "CAN2" port/pins on a Teensy 4.0

const uint32_t CAN_BIT_RATE       = 500000; // E46 bitrate is 500kb/s
const uint8_t  CAN_MAX_MAILBOXES  = 16;     // A mailbox represents an input or output data stream, for Teensy 4.0 max is 64

// MB0 == 0x316 DME1 Engine Control Unit
// MB1 == 0x329 DME2 Engine Control Unit
// MB2 == 0x545 DME4 Engine Control Unit

void setup(void) {
  // init CAN
  Can.begin();
  Can.setBaudRate(CAN_BIT_RATE);
  Can.setMaxMB(CAN_MAX_MAILBOXES);

  // init mailboxes
  setupReceiveMailbox(MB0);
  setupReceiveMailbox(MB1);
  setupReceiveMailbox(MB2);

  // TODO: setup transmit
  // setupTransmitMailbox(MB3);
  // setupTransmitMailbox(MB4);
  // setupTransmitMailbox(MB5);

  // apply default reject filter & interrupts
  Can.setMBFilter(REJECT_ALL);
  Can.enableMBInterrupts();

  // route & filter receive mailboxes
  filterReceiveMailbox(MB0, dme1Receive, 0x316);
  filterReceiveMailbox(MB1, dme2Receive, 0x329);
  filterReceiveMailbox(MB2, dme2Receive, 0x545);

  Can.mailboxStatus();
}

void setupReceiveMailbox(FLEXCAN_MAILBOX mb) {
  Can.setMB(mb, RX, EXT)
}

void setupTransmitMailbox(FLEXCAN_MAILBOX mb) {
  Can.setMB(mb, TX, EXT);
}

void filterReceiveMailbox(FLEXCAN_MAILBOX mb, _MB_ptr handler, uint32_t filter) {
  Can.onReceive(mb, handler);
  Can.setMBUserFilter(mb, filter, 0xFF); // final param is a bit mask
}

void dme1Receive(const CAN_message_t &msg) {
  // TODO: use msg.timestamp, msg.flags.overrun, msg.flags.extended
  // for ( uint8_t i = 0; i < msg.len; i++ ) {
  //   Serial.print(msg.buf[i], HEX); Serial.print(" ");
  // } Serial.println();
}

void dme2Receive(const CAN_message_t &msg) {

}

void dme4Receive(const CAN_message_t &msg) {

}

void loop() {
  Can.events()
}
