#ifndef MS43Can_H
#define MS43Can_H
#include <stdint.h>

typedef enum MS43_DME1_CHARGE_INTRV_STATE {
  // the required intervention can be performed completely
  // or if there is no ASC requirement
  CIS_INTRV_POSSIBLE,

  // the required intervention cannot be performed completely
  // because the IGA retard limit is reached
  CIS_INTRV_NOT_POSSIBLE_IGA_LIMIT,

  // the required intervention cannot be performed completely
  // because the charge actuators are fully closed (this case is
  // not possible with MTC without break-away mechanism),
  CIS_INTRV_NOT_POSSIBLE_ACT_CLOSED,

  // "limited vehicle dynamics" is active (MTC/ISA error)
  CIS_LIMITED_VEH_DYNAMICS_ACTIVE
} MS43_DME1_CHARGE_INTRV_STATE;

// DME1 : 0x316
// Refresh Rate: 10ms
class MS43_DME1_Frame {

  private:
    uint8_t (&frame)[8];

  public:

    // Create a MS43_DME1_Frame view from frame
    MS43_DME1_Frame(uint8_t (&f)[8]);

    // LV_SWI_IGK:
    // false - terminal 15 off detected
    // true  - terminal 15 on detected
    bool ignitionKeyVoltageIsOn();

    // LV_F_N_ENG:
    // false - no CRK error present
    // true  - CRK error present
    bool crankshaftSensorHasError();

    // LV_ACK_TCS:
    // false - either ASC1 was not received within the last 500 ms or it
    //         contains a plausibility error
    // true  - the ASC message ASC1 was received within the last 500 ms and
    //         contains no plausibility errors of the requirements TQI_ASR_CAN / TQI_MSR_CA
    bool tractionControlHasError();

    // LV_ERR_GC:
    // false - gear change not or partly possible
    // true  - gear change possible
    bool gearChangeIsPossible();

    // SF_TQD: Charge Intervention State
    // See enum values for reference
    MS43_DME1_CHARGE_INTRV_STATE chargeIntrvState();

    // LV_F_SUB_TQI:
    // false - MAF ok
    // true  - MAF error present
    bool mafHasError();

    // TQI_TQR_CAN:
    // Indexed Engine Torque in % of C_TQ_STND (including ASR/MSR/ETCU/LIM/AMT/GEAR intervention)
    double indexedTorquePercent();

    // N_ENG:
    // Engine Speed in rpm
    int engineSpeedRPM();

    // TQI_CAN:
    // Indicated Engine Torque in % of C_TQ_STND (based on PVS, N, AMP, TIA ,TCO, IGA, PUC so ip_tqi_pvs__n__pvs)
    double indicatedTorquePercent();

    // TQ_LOSS_CAN:
    // Engine Torque Loss (due to engine friction, AC compressor and electrical power consumption) in % of C_TQ_STND
    double lossTorquePercent();

    // ERR_AMT_CAN:
    // Automatic Manual Transmission has error
    bool amtHasError();

    // TQI_MAF_CAN:
    // Theorethical Engine Torque in % of C_TQ_STND after charge intervention (based on MAF & IGA so ip_tqi_maf__n__maf)
    double theorethicalTorquePercent();
};

typedef enum MS43_DME2_STEERING_WHEEL_CRUISE_STATE {
  // No Button Pressed (Init Value)
  SW_CRUISE_STATE_NO_BUTTON_PRESSED,

  // Set / Acceleration (tip-up)
  SW_CRUISE_STATE_INCREASE_OR_SET,

  // Deceleration (tip-down)
  SW_CRUISE_STATE_DECREASE,

  // Resume
  SW_CRUISE_STATE_RESUME,

  // Deactivate (I/O)
  SW_CRUISE_STATE_DEACTIVATE,

  SW_CRUISE_STATE_NOT_USED_5,
  SW_CRUISE_STATE_NOT_USED_6,

  // Error Condition
  SW_CRUISE_STATE_ERROR
} MS43_DME2_STEERING_WHEEL_CRUISE_STATE;

typedef enum MS43_DME2_CRUISE_STATE {

  // LV_CRU_ACT = 0
  CRUISE_STATE_NOT_ACTIVE,

  // LV_CRU_ACT = 1, constant drive (or tip-up / tip-down)
  CRUISE_STATE_CONSTANT_DRIVE,

  CRUISE_STATE_NOT_USED_2,

  // LV_CRU_ACT = 1, resume
  CRUISE_STATE_RESUME,

  CRUISE_STATE_NOT_USED_4,

  // LV_CRU_ACT = 1, set / acceleration
  CRUISE_STATE_SET_ACCELERATION,

  CRUISE_STATE_NOT_USED_6,

  // LV_CRU_ACT = 1, deceleration
  CRUISE_STATE_DECELERATION

} MS43_DME2_CRUISE_STATE;

// DME2 : 0x329
// Refresh Rate: 10ms
class MS43_DME2_Frame {

  private:
    uint8_t (&frame)[8];

  public:

    // Create a MS43_DME2_Frame view from frame
    MS43_DME2_Frame(uint8_t (&f)[8]);

    // TEMP_ENG:
    // Engine (Coolant) Temperature in °C
    // min:      -47.623°C
    // init/max: 142.877°C
    double engineTempC();

    // TEMP_ENG:
    // Engine (Coolant) Temperature in °F
    // min:      -53.7214°F
    // init/max: 289.179°F
    double engineTempF();

    // AMP_CAN:
    // Ambient Pressure in hPa
    // init:   0
    // min:    600hPa
    // max:    1106hPa
    // Error:  -1
    int ambientPressure();

    // LV_SWI_CLU:
    // Clutch Switch State
    // false - released
    // true  - depressed
    bool clutchIsPressed();

    // LV_LEVEL_IS:
    // Idle Regulator State
    // false - idle above threshold
    // true  - idle below threshold
    bool idleRegulatorOn();

    // LV_ACK_CRU_AD_ECU:
    // Acknowledge of ACC1 CAN Message
    // false - no ACK
    // true  - ACK
    bool ackFromACC1();

    // LV_ERU_CAN:
    // Engine Running State
    // false - engine stopped
    // true  - engine running
    bool engineIsRunning();

    // STATE_MSW_CAN: Steering wheel cruise control buttons
    // See enum values for reference
    MS43_DME2_STEERING_WHEEL_CRUISE_STATE steeringWheelCruiseState();

    // TPS_VIRT_CRU_CAN:
    // Virtual cruise control pedal percentage
    double virtualCruisePedalPercent();

    // TPS_CAN:
    // Accelerator Pedal Position in % of PVS_MAX
    // init/min: 0%
    // max:    : 99.2%
    // error   : -1
    double acceleratorPedalPercent();

    // LV_BS:
    // Brake Switch State
    // false - brake not actuated
    // true  - brake actuated
    bool brakeSwitchActive();

    // LV_ERR_BS:
    // Brake Switch System State
    // false - brake switch system OK
    // true  - brake switch system faulty
    bool brakeSwitchHasError();

    // LV_KD_CAN:
    // Kick Down State
    // 0 = kick down not active
    // 1 = kick down active
    bool kickdownIsActive();

    // STATE_CRU_CAN: Cruise Control state
    // See enum values for reference
    MS43_DME2_CRUISE_STATE cruiseControlState();

    // REQ_SHIFTLOCK:
    // Request for shiftlock to engage
    // false - no actuation is active
    // true  - actuation ISA, MTC or N_SP_IS is active
    bool shiftLockRequested();

};

typedef enum MS43_DME3_SPORT_BUTTON_STATUS {

  // Sport Button On (requested by SMG)
  SPORT_BUTTON_SPORT_ON_SMG_TRANS,

  // Sport Button Off
  SPORT_BUTTON_OFF,

  // Sport Button On
  SPORT_BUTTON_ON,

  // Sport Button Error
  SPORT_BUTTON_ERROR

} MS43_DME3_SPORT_BUTTON_STATUS;

// DME3 : 0x338
// Refresh Rate: 1000ms and at signal change
class MS43_DME3_Frame {

  private:
    uint8_t (&frame)[8];

  public:

    // Create a MS43_DME2_Frame view from frame
    MS43_DME3_Frame(uint8_t (&f)[8]);

    // STATE_SOF_CAN:
    // Sport Button Status
    // init : off
    MS43_DME3_SPORT_BUTTON_STATUS sportButtonStatus();
};

// DME4 : 0x545
// Refresh Rate: 10ms
class MS43_DME4_Frame {

  private:
    uint8_t (&frame)[8];

  public:

    // Create a MS43_DME2_Frame view from frame
    MS43_DME4_Frame(uint8_t (&f)[8]);

};

#endif
