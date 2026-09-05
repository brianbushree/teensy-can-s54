/*
 * Teensy 4.0 CAN S54 data to Nextion display
 */

#include "FlexCAN_T4.h"
#include "MS43Can.h"
#include "Nextion.h"

#define nextionSerial Serial2
#define celDigtlPin   PIND3

//
// FEATURE FLAGS
// --------------------------------------------------------------------------
// Use these to disable/enable units when unplugged
const bool CAN_ENABLE          = true;
const bool CAN_TRANSMIT_ENABLE = true;
const bool NEXTION_ENABLE      = true;
const bool CEL_DIGTL_ENABLE    = true;
const bool USB_DEBUG_ENABLE    = false;

//
// CAN CONFIG
// --------------------------------------------------------------------------
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can; // CAN2 port/pins on a Teensy 4.0

const uint32_t CAN_BIT_RATE      = 500000; // E46 bitrate is 500kb/s
const uint8_t  CAN_MAX_MAILBOXES = 16;     // Teensy 4.0 max is 64

const uint32_t CAN_ID_DME1 = 0x316;
const uint32_t CAN_ID_DME2 = 0x329;
const uint32_t CAN_ID_DME3 = 0x338;
const uint32_t CAN_ID_DME4 = 0x545;
const uint32_t CAN_ID_ICL2 = 0x613; // sent, not received
const uint32_t CAN_ID_ICL3 = 0x615; // sent, not received

// Test/fallback frame values: engine just fired, cold idle (~950 RPM,
// coolant/oil at ~68°F), no warning lights or fault bits set. This is what
// the Nextion shows before any real CAN traffic arrives.
const uint8_t DEFAULT_VAL_DME_1[CAN_FRAME_SIZE_BYTES] = { 0x0D, 0x0B, 0xC0, 0x17, 0x0B, 0x0A, 0x00, 0x77 };
const uint8_t DEFAULT_VAL_DME_2[CAN_FRAME_SIZE_BYTES] = { 0x4F, 0x5C, 0x84, 0x08, 0x00, 0x00, 0x00, 0x00 };
const uint8_t DEFAULT_VAL_DME_3[CAN_FRAME_SIZE_BYTES] = { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t DEFAULT_VAL_DME_4[CAN_FRAME_SIZE_BYTES] = { 0x00, 0x8B, 0x02, 0x00, 0x44, 0x00, 0x9E, 0x00 };

//
// NEXTION PROTOCOL
// --------------------------------------------------------------------------
// Receive buffer: 0xfe start delim, 3-byte command, 4-byte value, 0xff end delim
const int NEXTION_BUF_SIZE_BYTES = 10;
uint8_t valNextionRXBuffer[NEXTION_BUF_SIZE_BYTES];
int valNextionRXBufferPos = 0;
bool valNextionRXInProgress = false;
struct NextionCommand {
  char command[4]; // extra byte for terminator
  uint8_t value[4];
};

// True/False values for Nextion booleans
const uint8_t VAL_BOOLEAN_TRUE  = 1;
const uint8_t VAL_BOOLEAN_FALSE = 0;

//
// DATA MODEL
// --------------------------------------------------------------------------
// Frames decoded from CAN. See CAN_RX_MAILBOXES for the mailbox/ID feeding
// each one; valICL2/valICL3 below are sent, not received (CAN_ID_ICL2/ICL3).
struct MS43CanData {
  MS43_DME1_Frame valDME1 = MS43_DME1_Frame(DEFAULT_VAL_DME_1);
  MS43_DME2_Frame valDME2 = MS43_DME2_Frame(DEFAULT_VAL_DME_2);
  MS43_DME3_Frame valDME3 = MS43_DME3_Frame(DEFAULT_VAL_DME_3);
  MS43_DME4_Frame valDME4 = MS43_DME4_Frame(DEFAULT_VAL_DME_4);

  MS43_ICL3_Frame valICL3 = MS43_ICL3_Frame();
  MS43_ICL2_Frame valICL2 = MS43_ICL2_Frame();
} ms43CanData;

struct SendTimers {
  uint32_t Nextion_lastSend = millis();
  uint16_t Nextion_sendRateMs = 50;

  uint32_t MS43_ICL3_Frame_lastSend = millis();
  uint16_t MS43_ICL3_sendRateMs     = 200;

  uint32_t MS43_ICL2_Frame_lastSend = millis();
  uint16_t MS43_ICL2_sendRateMs     = 200;

  uint32_t Debug_lastSend = millis();
  uint16_t Debug_sendRateMs = 500;
} sendTimers;

//
// DASHBOARD STATE
// --------------------------------------------------------------------------
// Nextion page state (0 indexed, default to zero on startup)
uint8_t valCurrentNextionPage = 0;

// Check Engine Light (CEL) State
const char NEXTION_KEY_CEL_IS_ON[]   = "obc.cel_is_on.val";
const uint8_t DEFAULT_VAL_CEL_IS_ON  = VAL_BOOLEAN_FALSE;
uint8_t valCelIsOn                   = DEFAULT_VAL_CEL_IS_ON;

// Is AC ON?
const char NEXTION_IS_AC_ON[]       = "obc.ac_is_on.val";
const uint8_t DEFAULT_VAL_AC_IS_ON  = VAL_BOOLEAN_FALSE;
uint8_t valAcIsOn                   = DEFAULT_VAL_AC_IS_ON;

// Torque (Nm) the AC compressor draws at idle; maxed at 31 (the field's 5-bit
// ceiling) to avoid bogging down when power steering load is also present.
const uint8_t AC_COMPRESSOR_TORQUE_OFFSET_NM = 31;

// Oil low warning light boolean
const char NEXTION_KEY_IS_OIL_ON[] = "obc.oil_is_on.val";

// Coolant warning light boolean
const char NEXTION_KEY_IS_COOLANT_ON[] = "obc.coolant_is_on.val";

// Engine Temperature in F
const char NEXTION_KEY_ENG_TEMP_F[] = "obc.eng_temp_f.val";

// Engine Speed in RPM
const char NEXTION_KEY_ENG_SPEED_RPM[] = "obc.eng_speed_rpm.val";

// Oil Temperature in F
const char NEXTION_KEY_OIL_TEMP_F[] = "obc.oil_temp_f.val";

// Indicated Torque in %
const char NEXTION_KEY_TORQUE[] = "obc.torque.val";

bool isCELOn() {
   return (ms43CanData.valDME4.checkEngineLightOn() || ms43CanData.valDME4.emlLightOn());
}

bool isACOn() {
   return valAcIsOn == VAL_BOOLEAN_TRUE;
}

// Nextion State struct: contains all Nextion pages, keys, & pointers to values
NextionVariable pageOneVars[] = {
  {
    (char*)&NEXTION_KEY_CEL_IS_ON, []() -> int16_t { return (isCELOn()) ? VAL_BOOLEAN_TRUE : VAL_BOOLEAN_FALSE; },
  },
  {
    (char*)&NEXTION_IS_AC_ON, []() -> int16_t { return (isACOn()) ? VAL_BOOLEAN_TRUE : VAL_BOOLEAN_FALSE; },
  },
  {
    (char*)&NEXTION_KEY_IS_OIL_ON, []() -> int16_t { return (ms43CanData.valDME4.oilLevelErrorLightOn() || ms43CanData.valDME4.engineOilPressureLow()) ? VAL_BOOLEAN_TRUE : VAL_BOOLEAN_FALSE; },
  },
  {
    (char*)&NEXTION_KEY_IS_COOLANT_ON, []() -> int16_t { return (ms43CanData.valDME4.coolantOverheatingLightOn()) ? VAL_BOOLEAN_TRUE : VAL_BOOLEAN_FALSE; },
  },
  {
    (char*)&NEXTION_KEY_ENG_TEMP_F, []() -> int16_t { return ms43CanData.valDME2.engineTempF(); },
  },
  {
    (char*)&NEXTION_KEY_ENG_SPEED_RPM, []() -> int16_t { return ms43CanData.valDME1.engineSpeedRPM(); },
  },
  {
    (char*)&NEXTION_KEY_OIL_TEMP_F, []() -> int16_t { return ms43CanData.valDME4.oilTempF(); },
  },
  {
    (char*)&NEXTION_KEY_TORQUE, []() -> int16_t { return ms43CanData.valDME1.indicatedTorquePercent(); },
  },
};
NextionPage pages[] = {
  { // Page 1 == ???? TODO
    pageOneVars, (sizeof(pageOneVars)/sizeof(*pageOneVars)),
  }
};
NextionState nextionState = NextionState{
  pages, (sizeof(pages)/sizeof(*pages))
};

//
// CAN SETUP
// --------------------------------------------------------------------------
void setupTransmitMailbox(FLEXCAN_MAILBOX mb) {
  Can.setMB(mb, TX, STD);
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

struct CanRxMailbox {
  FLEXCAN_MAILBOX mb;
  uint32_t id;
  _MB_ptr handler;
};

const CanRxMailbox CAN_RX_MAILBOXES[] = {
  { MB0, CAN_ID_DME1, dme1Receive },
  { MB1, CAN_ID_DME2, dme2Receive },
  { MB2, CAN_ID_DME3, dme3Receive },
  { MB3, CAN_ID_DME4, dme4Receive },
};
const uint8_t CAN_RX_MAILBOX_COUNT = sizeof(CAN_RX_MAILBOXES) / sizeof(*CAN_RX_MAILBOXES);

void initCanBus(void) {
  Can.begin();
  Can.setBaudRate(CAN_BIT_RATE);
  Can.setMaxMB(CAN_MAX_MAILBOXES);

  // E46 IDs are standard 11-bit, not extended
  for (uint8_t i = 0; i < CAN_RX_MAILBOX_COUNT; i++) {
    Can.setMB(CAN_RX_MAILBOXES[i].mb, RX, STD);
  }

  if (CAN_TRANSMIT_ENABLE) {
    setupTransmitMailbox(MB4);
    setupTransmitMailbox(MB5);
  }

  // apply default reject filter & interrupts
  Can.setMBFilter(REJECT_ALL);
  Can.enableMBInterrupts();

  for (uint8_t i = 0; i < CAN_RX_MAILBOX_COUNT; i++) {
    Can.onReceive(CAN_RX_MAILBOXES[i].mb, CAN_RX_MAILBOXES[i].handler);
    Can.setMBFilter(CAN_RX_MAILBOXES[i].mb, CAN_RX_MAILBOXES[i].id); // exact match on a single ID
  }

  Can.mailboxStatus();
}

//
// NEXTION I/O
// --------------------------------------------------------------------------
void nextionReceiveCommand(NextionCommand *cmd) {
  if (strcmp(cmd->command, "CEL") == 0) {
    if (cmd->value[0] == '1') {
      valCelIsOn =  VAL_BOOLEAN_TRUE;
    } else if (cmd->value[0] == '0') {
      valCelIsOn = VAL_BOOLEAN_FALSE;
    }
  } else if (strcmp(cmd->command, "ACC") == 0) {
    bool isACOn = false;
    if (cmd->value[0] == '1') {
      valAcIsOn = VAL_BOOLEAN_TRUE;
      isACOn = true;
    } else if (cmd->value[0] == '0') {
      valAcIsOn = VAL_BOOLEAN_FALSE;
    }
    // AC request, compressor status, and torque offset all move together —
    // the DME sizes idle compensation off the offset, so requesting AC while
    // leaving it at 0 gets ignored.
    ms43CanData.valICL3.setAirConditioningRequestEnabled(isACOn);
    ms43CanData.valICL3.setRequestRaisedIdleEnabled(isACOn);
    ms43CanData.valICL3.setACCompressorStatusEnabled(isACOn);
    ms43CanData.valICL3.setTorqueOffsetForACCompressor(
      isACOn ? AC_COMPRESSOR_TORQUE_OFFSET_NM : 0
    );
  }
}

// Receive values from Nextion
void nextionReceive() {
  while (nextionSerial.available()) {
    uint8_t c = nextionSerial.read();
    if (!valNextionRXInProgress) {
      // start message
      if (c == 0xfe) {
        valNextionRXBufferPos = 0;
        valNextionRXInProgress = true;
      }
    } else {
      // end of command
      if (c == 0xff) {
        valNextionRXInProgress = false;
        NextionCommand cmd = {
          .command = { (char) valNextionRXBuffer[0], (char) valNextionRXBuffer[1], (char) valNextionRXBuffer[2], '\0' },
          .value   = { valNextionRXBuffer[3], valNextionRXBuffer[4], valNextionRXBuffer[5], valNextionRXBuffer[6] }
        };
        nextionReceiveCommand(&cmd);
      } else if (valNextionRXBufferPos < NEXTION_BUF_SIZE_BYTES) {
        valNextionRXBuffer[valNextionRXBufferPos] = c;
        valNextionRXBufferPos += 1;
      }
    }
  }
}

//
// ARDUINO ENTRY POINTS
// --------------------------------------------------------------------------
void setup(void) {
  if (CAN_ENABLE) {
    initCanBus();
  }
  if (NEXTION_ENABLE) {
    nextionSerial.begin(115200);
  }
  if (CEL_DIGTL_ENABLE) {
    pinMode(celDigtlPin, OUTPUT);
  }
  if (USB_DEBUG_ENABLE) {
    Serial.begin(9600);
  }
  delay(1000);
}

//
// SEND HELPERS
// --------------------------------------------------------------------------
// Send all Nextion values for a given page
void sendValuesToNextion(u_int32_t currentTimeMs, const uint8_t page) {
  if (currentTimeMs - sendTimers.Nextion_lastSend > sendTimers.Nextion_sendRateMs) {
    sendTimers.Nextion_lastSend = currentTimeMs;

    for (int var= 0; var < nextionState.pages[page].variablesLength; var++) {
      NextionVariable variable = nextionState.pages[page].variables[var];
      nextionSerial.printf("%s=%d", variable.key, variable.value());
      // use three 0xff to delimit input (Nextion format)
      nextionSerial.write(0xff);
      nextionSerial.write(0xff);
      nextionSerial.write(0xff);
    }
  }
}

void sendCELToDashboard() {
  if (isCELOn()) {
    digitalWrite(celDigtlPin, HIGH);
  } else {
    digitalWrite(celDigtlPin, LOW);
  }
}

// Construct & send a single frame out of the given mailbox
void sendFrameToDME(FLEXCAN_MAILBOX mb, uint32_t id, u_int8_t* data) {
  CAN_message_t msg;
  msg.id = id;
  msg.len = CAN_FRAME_SIZE_BYTES;
  for (int i = 0; i < CAN_FRAME_SIZE_BYTES; i++) {
    msg.buf[i] = data[i];
  }
  Can.write(mb, msg);
}

void sendICL3ToDME(u_int32_t currentTimeMs) {
  if (currentTimeMs - sendTimers.MS43_ICL3_Frame_lastSend > sendTimers.MS43_ICL3_sendRateMs) {
    sendTimers.MS43_ICL3_Frame_lastSend = currentTimeMs;
    sendFrameToDME(MB4, CAN_ID_ICL3, ms43CanData.valICL3.serialize());
  }
}

void sendICL2ToDME(u_int32_t currentTimeMs) {
  if (currentTimeMs - sendTimers.MS43_ICL2_Frame_lastSend > sendTimers.MS43_ICL2_sendRateMs) {
    sendTimers.MS43_ICL2_Frame_lastSend = currentTimeMs;
    // running clock counts minutes since power was last lost
    ms43CanData.valICL2.setRunningClockMinutes(currentTimeMs / 60000);
    sendFrameToDME(MB5, CAN_ID_ICL2, ms43CanData.valICL2.serialize());
  }
}

void sendValuesToDME(u_int32_t currentTimeMs) {
  sendICL3ToDME(currentTimeMs);
  sendICL2ToDME(currentTimeMs);
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
  if (NEXTION_ENABLE) {
    nextionReceive();
  }

  //
  // EMIT EVENTS
  // --------------------------------------------------------------------------

  // shared timestamp for this cycle's send checks, to avoid a system call per sender
  u_int32_t currentTimeMs = millis();

  // emit all values necessary for CAN bus
  if (CAN_ENABLE && CAN_TRANSMIT_ENABLE) {
    sendValuesToDME(currentTimeMs);
  }

  // emit all values for current page to Nextion
  if (NEXTION_ENABLE) {
    sendValuesToNextion(currentTimeMs, valCurrentNextionPage);
  }

  if (CEL_DIGTL_ENABLE) {
    sendCELToDashboard();
  }

  if (USB_DEBUG_ENABLE && currentTimeMs - sendTimers.Debug_lastSend > sendTimers.Debug_sendRateMs) {
    sendTimers.Debug_lastSend = currentTimeMs;
    Serial.print(".");
  }
}
