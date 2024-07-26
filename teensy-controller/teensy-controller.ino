/*
 * Teensy 4.0 CAN S54 data to Nextion display
 */

#include <FlexCAN_T4.h>
#include <MS43Can.h>
#include <Nextion.h>

#define nextionSerial Serial2

// Use these to disable/enable units when unplugged
const bool CAN_ENABLE       = false;
const bool NEXTION_ENABLE   = true;
const bool USB_DEBUG_ENABLE = true;

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can; // For CAN communications between devices, use the "CAN2" port/pins on a Teensy 4.0

const uint32_t CAN_BIT_RATE       = 500000; // E46 bitrate is 500kb/s
const uint8_t  CAN_MAX_MAILBOXES  = 16;     // A mailbox represents an input or output data stream, for Teensy 4.0 max is 64

// For testing frame values
const uint8_t DEFAULT_VAL_DME_1[8] = { 0x05, 0x0B, 0xB8, 0x0C, 0x0B, 0x0A, 0x00, 0x77 };
const uint8_t DEFAULT_VAL_DME_2[8] = { 0x4F, 0xB1, 0x84, 0x10, 0x00, 0x51, 0xFF, 0x00 };
const uint8_t DEFAULT_VAL_DME_3[8] = { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t DEFAULT_VAL_DME_4[8] = { 0xEE, 0x8B, 0x02, 0x88, 0x00, 0x00, 0x00, 0x00 };

struct MS43CanData {
  //   MB0 == 0x316 DME1 Engine Control Unit
  MS43_DME1_Frame valDME1 = MS43_DME1_Frame(DEFAULT_VAL_DME_1);
  //   MB1 == 0x329 DME2 Engine Control Unit
  MS43_DME2_Frame valDME2 = MS43_DME2_Frame(DEFAULT_VAL_DME_2);
  //   MB2 == 0x338 DME3 Engine Control Unit
  MS43_DME3_Frame valDME3 = MS43_DME3_Frame(DEFAULT_VAL_DME_3);
  //   MB3 == 0x545 DME4 Engine Control Unit
  MS43_DME4_Frame valDME4 = MS43_DME4_Frame(DEFAULT_VAL_DME_4);
} ms43CanData;

// Nextion Page State
// stateful page variable (0 indexed, default to zero on startup)
// indicates what page user is on and what values to send
uint8_t valCurrentNextionPage = 0;

// Check Engine Light (CEL) State
// val: 't' for ON, 'f' for OFF
const char NEXTION_KEY_CEL_IS_ON[18] = "obc.cel_is_on.val"; // declare one extra char
const uint8_t VAL_CEL_IS_ON_TRUE     = (uint8_t) '1';
const uint8_t VAL_CEL_IS_ON_FALSE    = (uint8_t) '0';
const uint8_t DEFAULT_VAL_CEL_IS_ON  = VAL_CEL_IS_ON_FALSE;
uint8_t valCelIsOn                   = DEFAULT_VAL_CEL_IS_ON;

// Engine Temperature in F
const char NEXTION_KEY_ENG_TEMP_F[19] = "obc.eng_temp_f.val";

// Nextion State struct
// contains all Nextion pages, keys, & pointers to values
NextionVariable pageOneVars[] = {
  {
    (char*)&NEXTION_KEY_CEL_IS_ON, [] { return (uint8_t) ms43CanData.valDME4.checkEngineLightOn(); },
  },
  {
    (char*)&NEXTION_KEY_ENG_TEMP_F, [] { return (uint8_t) ms43CanData.valDME2.engineTempF(); },
  }
};
NextionPage pages[] = {
  { // Page 1 == ???? TODO
    pageOneVars, (sizeof(pageOneVars)/sizeof(*pageOneVars)),
  }
};
NextionState nextionState = NextionState{
  pages, (sizeof(pages)/sizeof(*pages))
};

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

void setup(void) {
  if (CAN_ENABLE) {
    initCanBus();
  }
  if (NEXTION_ENABLE) {
    nextionSerial.begin(115200);
  }
  if (USB_DEBUG_ENABLE) {
    Serial.begin(9600);
  }
  delay(1000);
}

void setupReceiveMailbox(FLEXCAN_MAILBOX mb) {
  Can.setMB(mb, RX, EXT);
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
  ms43CanData.valDME1.update(msg.buf);
}

void dme2Receive(const CAN_message_t &msg) {
  ms43CanData.valDME2.update(msg.buf);
}

void dme3Receive(const CAN_message_t &msg) {
  ms43CanData.valDME3.update(msg.buf);
}

void dme4Receive(const CAN_message_t &msg) {
  ms43CanData.valDME4.update(msg.buf);
}

// Send all Nextion values for a given page
void sendValuesToNextion(const uint8_t page) {
  for (int var= 0; var < nextionState.pages[page].variablesLength; var++) {
    // send variable as "KEY=V"
    NextionVariable variable = nextionState.pages[page].variables[var];
    nextionSerial.print(variable.key);
    nextionSerial.print('='); // printf?
    nextionSerial.write(variable.value());

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
  if (CAN_ENABLE) {
    Can.events();
  }

  // receive values from Nextion & update local state
  // TODO

  //
  // EMIT EVENTS
  // --------------------------------------------------------------------------

  // emit all values necessary for CAN bus
  // TODO

  // emit all values for current page to Nextion
  if (NEXTION_ENABLE) {
    sendValuesToNextion(valCurrentNextionPage);
  }

  if (USB_DEBUG_ENABLE) {
    Serial.print(".");
    delay(500);
  }
}
