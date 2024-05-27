/*
 * Teensy 4.0 CAN S54 data to Nextion display
 */

#include <FlexCAN_T4.h>
#include "../ms43-can/MS43Can.h"
#include "../nextion/Nextion.h"

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can; // For CAN communications between devices, use the "CAN2" port/pins on a Teensy 4.0

const uint32_t CAN_BIT_RATE       = 500000; // E46 bitrate is 500kb/s
const uint8_t  CAN_MAX_MAILBOXES  = 16;     // A mailbox represents an input or output data stream, for Teensy 4.0 max is 64

// CAN mailboxes:
//   MB0 == 0x316 DME1 Engine Control Unit
//   MB1 == 0x329 DME2 Engine Control Unit
//   MB2 == 0x338 DME3 Engine Control Unit
//   MB3 == 0x545 DME4 Engine Control Unit

// Nextion Page State
// stateful page variable (0 indexed, default to zero on startup)
// indicates what page user is on and what values to send
uint8_t VAL_CURR_NEXTION_PAGE = 0;

// Nextion State struct
// contains all Nextion pages, keys, & pointers to values
NextionState NEXTION_STATE;

// Check Engine Light (CEL) State
// val: 't' for ON, 'f' for OFF
const char NEXTION_KEY_CEL_IS_ON[8] = "CELIsOn"; // declare one extra char
const uint8_t DEFAULT_VAL_CEL_IS_ON = (uint8_t) 'f';
uint8_t valCelIsOn = DEFAULT_VAL_CEL_IS_ON;

void initCanBus(void) {
  // init CAN
  Can.begin();
  Can.setBaudRate(CAN_BIT_RATE);
  Can.setMaxMB(CAN_MAX_MAILBOXES);

  // init mailboxes
  setupReceiveMailbox(MB0);
  setupReceiveMailbox(MB1);
  setupReceiveMailbox(MB2);
  setupReceiveMailbox(MB3);

  // TODO: setup transmit
  // setupTransmitMailbox(MB4);
  // setupTransmitMailbox(MB5);

  // apply default reject filter & interrupts
  Can.setMBFilter(REJECT_ALL);
  Can.enableMBInterrupts();

  // route & filter receive mailboxes
  filterReceiveMailbox(MB0, dme1Receive, 0x316);
  filterReceiveMailbox(MB1, dme2Receive, 0x329);
  filterReceiveMailbox(MB2, dme3Receive, 0x338);
  filterReceiveMailbox(MB3, dme4Receive, 0x545);

  Can.mailboxStatus();
}

void initNextionState(void) {
  NextionVariable pageOneVars[] = {
    {
      (char*)&NEXTION_KEY_CEL_IS_ON, &valCelIsOn
    }
  };
  NextionPage pages[] = {
    { // Page 1 == ???? TODO
      pageOneVars, (sizeof(pageOneVars)/sizeof(*pageOneVars)),
    }
  };

  NEXTION_STATE = NextionState{
    pages, (sizeof(pages)/sizeof(*pages))
  };
}

void setup(void) {
  initCanBus();
  initNextionState()
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

// Examples:
//   05 0B B8 0C 0B 0A 00 77
//   05 0C B8 27 0C 0B 46 80
//   0D 00 84 27 00 0B 31 82
void dme1Receive(const CAN_message_t &msg) {
  MS43_DME1_Frame value = MS43_DME1_Frame(msg.buf);
  // TODO: do something with this
}

void dme2Receive(const CAN_message_t &msg) {
  MS43_DME2_Frame value = MS43_DME2_Frame(msg.buf);
  // TODO: do something with this
}

void dme3Receive(const CAN_message_t &msg) {
  MS43_DME3_Frame value = MS43_DME3_Frame(msg.buf);
  // TODO: do something with this
}

void dme4Receive(const CAN_message_t &msg) {
  MS43_DME4_Frame value = MS43_DME4_Frame(msg.buf);
  // TODO: do something with this
}

// Send all Nextion values for a given page
void sendValuesToNextion(const uint8_t page) {
  for (int var= 0; var < state.pages[page].variablesLength; var++) {
    // send variable as "KEY=V"
    NextionVariable variable = state.pages[page].variables[var];
    nextionSerial.print(variable.key);
    nextionSerial.print('='); // printf?
    nextionSerial.print(*variable.value); // TODO: should this be write()?

    // use three 0xff to deliminate input
    nextionSerial.write(0xff);
    nextionSerial.write(0xff);
    nextionSerial.write(0xff);
  }
}

void loop() {
  //
  // RECEIVE EVENTS
  // --------------------------------------------------------------------------

  // receive values coming in from CAN bus & update local state
  Can.events()

  // receive values from Nextion & update local state
  // TODO

  //
  // EMIT EVENTS
  // --------------------------------------------------------------------------

  // emit all values necessary for CAN bus
  // TODO

  // emit all values for current page to Nextion
  sendValuesToNextion(VAL_CURR_NEXTION_PAGE);

}
