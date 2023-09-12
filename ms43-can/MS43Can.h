#ifndef MS43Can_H
#define MS43Can_H
#include <stdint.h>

typedef enum MS43_DME1_CHARGE_INTRV_STATE {
  // the required intervention can be performed completely
  // or if there is no ASC requirement
  INTRV_POSSIBLE,

  // the required intervention cannot be performed completely
  // because the IGA retard limit is reached
  INTRV_NOT_POSSIBLE_IGA_LIMIT,

  // the required intervention cannot be performed completely
  // because the charge actuators are fully closed (this case is
  // not possible with MTC without break-away mechanism),
  INTRV_NOT_POSSIBLE_ACT_CLOSED,

  // "limited vehicle dynamics" is active (MTC/ISA error)
  LIMITED_VEH_DYNAMICS_ACTIVE
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

// DME2 : 0x329
// Refresh Rate: 10ms
class MS43_DME2_Frame {

  private:
    uint8_t (&frame)[8];

  public:

    // Create a MS43_DME2_Frame view from frame
    MS43_DME2_Frame(uint8_t (&f)[8]);

};

// DME3 : 0x338
// Refresh Rate: 1000ms and at signal change
class MS43_DME3_Frame {

  private:
    uint8_t (&frame)[8];

  public:

    // Create a MS43_DME2_Frame view from frame
    MS43_DME3_Frame(uint8_t (&f)[8]);

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
