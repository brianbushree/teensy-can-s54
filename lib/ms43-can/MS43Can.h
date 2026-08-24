#ifndef MS43Can_H
#define MS43Can_H
#include <stdint.h>

#define CAN_FRAME_SIZE_BYTES 8

float celciusToFahrenheit(float tempC);
float fahrenheitToCelcius(float tempF);
int kmhToMph(int kmh);
int mphToKmh(int mph);
float pedalValueToPercent(uint8_t val);

// Received frame base class
class MS43_Frame_Receive_Base {
  protected:
    uint8_t frame[CAN_FRAME_SIZE_BYTES] = { };
  public:
    // Create a ReceiveFrame
    MS43_Frame_Receive_Base(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]);
    // Update the frame
    void update(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]);
};

// Send frame base class
class MS43_Frame_Send_Base {
  protected:
    uint8_t frame[CAN_FRAME_SIZE_BYTES] = { };
  public:
    // Create a SendFrame
    MS43_Frame_Send_Base();
    // Serialize the frame
    uint8_t* serialize();
};

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
class MS43_DME1_Frame : public MS43_Frame_Receive_Base {

  public:

    // Create a MS43_DME1_Frame view from frame
    MS43_DME1_Frame(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]);

    // LV_SWI_IGK:
    // false - terminal 15 off detected
    // true  - terminal 15 on detected
    bool ignitionKeyVoltageIsOn() const;

    // LV_F_N_ENG:
    // false - no CRK error present
    // true  - CRK error present
    bool crankshaftSensorHasError() const;

    // LV_ACK_TCS:
    // false - either ASC1 was not received within the last 500 ms or it
    //         contains a plausibility error
    // true  - the ASC message ASC1 was received within the last 500 ms and
    //         contains no plausibility errors of the requirements TQI_ASR_CAN / TQI_MSR_CA
    bool tractionControlHasError() const;

    // LV_ERR_GC:
    // false - gear change not or partly possible
    // true  - gear change possible
    bool gearChangeIsPossible() const;

    // SF_TQD: Charge Intervention State
    // See enum values for reference
    MS43_DME1_CHARGE_INTRV_STATE chargeIntrvState() const;

    // LV_F_SUB_TQI:
    // false - MAF ok
    // true  - MAF error present
    bool mafHasError() const;

    // TQI_TQR_CAN:
    // Indexed Engine Torque in % of C_TQ_STND (including ASR/MSR/ETCU/LIM/AMT/GEAR intervention)
    float indexedTorquePercent() const;

    // N_ENG:
    // Engine Speed in rpm
    int engineSpeedRPM() const;

    // TQI_CAN:
    // Indicated Engine Torque in % of C_TQ_STND (based on PVS, N, AMP, TIA ,TCO, IGA, PUC so ip_tqi_pvs__n__pvs)
    float indicatedTorquePercent() const;

    // TQ_LOSS_CAN:
    // Engine Torque Loss (due to engine friction, AC compressor and electrical power consumption) in % of C_TQ_STND
    float lossTorquePercent() const;

    // ERR_AMT_CAN:
    // Automatic Manual Transmission has error
    bool amtHasError() const;

    // TQI_MAF_CAN:
    // Theorethical Engine Torque in % of C_TQ_STND after charge intervention (based on MAF & IGA so ip_tqi_maf__n__maf)
    float theorethicalTorquePercent() const;
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
class MS43_DME2_Frame : public MS43_Frame_Receive_Base {

  public:

    // Create a MS43_DME2_Frame view from frame
    MS43_DME2_Frame(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]);

    // TEMP_ENG:
    // Engine (Coolant) Temperature in °C
    // min:      -47°C
    // init/max: 142°C
    int engineTempC() const;

    // TEMP_ENG:
    // Engine (Coolant) Temperature in °F
    // min:      -53°F
    // init/max: 289°F
    int engineTempF() const;

    // AMP_CAN:
    // Ambient Pressure in hPa
    // init:   0
    // min:    600hPa
    // max:    1106hPa
    // Error:  -1
    int ambientPressure() const;

    // LV_SWI_CLU:
    // Clutch Switch State
    // false - released
    // true  - depressed
    bool clutchIsPressed() const;

    // LV_LEVEL_IS:
    // Idle Regulator State
    // false - idle above threshold
    // true  - idle below threshold
    bool idleRegulatorOn() const;

    // LV_ACK_CRU_AD_ECU:
    // Acknowledge of ACC1 CAN Message
    // false - no ACK
    // true  - ACK
    bool ackFromACC1() const;

    // LV_ERU_CAN:
    // Engine Running State
    // false - engine stopped
    // true  - engine running
    bool engineIsRunning() const;

    // STATE_MSW_CAN: Steering wheel cruise control buttons
    // See enum values for reference
    MS43_DME2_STEERING_WHEEL_CRUISE_STATE steeringWheelCruiseState() const;

    // TPS_VIRT_CRU_CAN:
    // Virtual cruise control pedal percentage
    float virtualCruisePedalPercent() const;

    // TPS_CAN:
    // Accelerator Pedal Position in % of PVS_MAX
    // init/min: 0%
    // max:    : 99.2%
    // error   : -1
    float acceleratorPedalPercent() const;

    // LV_BS:
    // Brake Switch State
    // false - brake not actuated
    // true  - brake actuated
    bool brakeSwitchActive() const;

    // LV_ERR_BS:
    // Brake Switch System State
    // false - brake switch system OK
    // true  - brake switch system faulty
    bool brakeSwitchHasError() const;

    // LV_KD_CAN:
    // Kick Down State
    // 0 = kick down not active
    // 1 = kick down active
    bool kickdownIsActive() const;

    // STATE_CRU_CAN: Cruise Control state
    // See enum values for reference
    MS43_DME2_CRUISE_STATE cruiseControlState() const;

    // REQ_SHIFTLOCK:
    // Request for shiftlock to engage
    // false - no actuation is active
    // true  - actuation ISA, MTC or N_SP_IS is active
    bool shiftLockRequested() const;

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
class MS43_DME3_Frame : public MS43_Frame_Receive_Base {

  public:

    // Create a MS43_DME2_Frame view from frame
    MS43_DME3_Frame(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]);

    // STATE_SOF_CAN:
    // Sport Button Status
    // init : off
    MS43_DME3_SPORT_BUTTON_STATUS sportButtonStatus() const;
};

// DME4 : 0x545
// Refresh Rate: 10ms
class MS43_DME4_Frame : public MS43_Frame_Receive_Base {

  public:

    // Create a MS43_DME2_Frame view from frame
    MS43_DME4_Frame(const uint8_t (&f)[CAN_FRAME_SIZE_BYTES]);

    // LV_MIL:
    // Check Engine Light
    // false - OFF
    // true  - ON
    //
    // SES/CEL/MIL are the same name of what is "officially" the
    // Malfunction Indicator Light that can light when there is an OBDII
    // fault that impacts the engine fuel management system and can cause
    // increase exhaust emissions.
    bool checkEngineLightOn() const;

    // LV_MAIN_SWI_CRU - CRU_MAIN_SWI:
    // Cruise Control Light
    // false - OFF
    // true  - ON
    bool cruiseControlLightOn() const;

    // LV_ETC_DIAG:
    // EML Light
    // false - OFF
    // true  - ON
    //
    // The EML is for the electronic drive by wire throttle control.
    // It may or may not come on with the addition of ABS and/or DSC
    // and/or SES/CEL/MIL lights. The Electronic Throttle control system
    // is a pretty complex system and often can and will work with other
    // systems on the car.
    bool emlLightOn() const;

    // LV_FUC_CAN:
    // Fuel Tank Cap Light
    // false - OFF
    // true  - ON
    bool fuelTankCapLight() const;

    // FCO:
    // Fuel consumiption counter
    // Values just cycle from 0 to FFFF then start over at zero.
    // Fuel consumiption is the rate of change.
    uint16_t fuelConsumptionCounter() const;

    // Oil Level Error LED M5 Cluster
    // Not sure why there's two...
    // false - OFF
    // true  - ON
    bool oilLevelErrorLightOnM5One() const;

    // Oil Level Warning LED Every Other Cluster
    // false - OFF
    // true  - ON
    bool oilLevelErrorLightOn() const;

    // Oil Level Error LED M5 Cluster
    // Not sure why there's two...
    // false - OFF
    // true  - ON
    bool oilLevelErrorLightOnM5Two() const;

    // LV_TEMP_ENG - Coolant Overheating Light
    // false - OFF
    // true  - ON
    bool coolantOverheatingLightOn() const;

    // M3 warm up light, temp high
    // 7K and up RPM lights
    // false - OFF
    // true  - ON
    bool warmUpHighLightOn() const;

    // M3 warm up light, temp medium
    // 6.5K and up RPM lights
    // false - OFF
    // true  - ON
    bool warmUpMediumLightOn() const;

    // M3 warm up light, temp low
    // 5.5K and up RPM lights
    // false - OFF
    // true  - ON
    bool warmUpLowLightOn() const;

    // TOIL_CAN:
    // Oil Temperature in °C
    // min: -48°C
    // max: 206°C
    int oilTempC() const;

    // TOIL_CAN:
    // Oil Temperature in °F
    // min: -54°F
    // max: 402°F
    int oilTempF() const;

    // Battery Chargelight (Alpina Roadster Only)
    bool batteryChargeLightOn() const;

    // Oil Level (MSS54HP only)
    // min: -3.0L
    // max: +3.2L
    float oilLevelLiters() const;

    // Tire pressure (MSS54HP only)
    // false - normal (confirm this?)
    // true  - low pressure (confirm this?)
    bool tirePressureState() const;

    // Engine Oil Pressure Low
    // false - normal
    // true  - low pressure
    bool engineOilPressureLow() const;
};

// ICL3 : 0x615
// Refresh Rate: 200ms
class MS43_ICL3_Frame : public MS43_Frame_Send_Base {

  public:

    // Create a MS43_DME1_Frame view from frame
    MS43_ICL3_Frame();

    // Set torque offset for air conditioning compressor
    // Must be a value between 0-31 (inclusive) in NM
    void setTorqueOffsetForACCompressor(int torqueOffset);

    // Set request for lowering cooling temp (c_tco_bol_ect)
    // false - OFF
    // true  - ON
    void setRequestForLoweringCoolingTempEnabled(bool enabled);

    // Set AC compressor status
    // false - OFF
    // true  - ON
    void setACCompressorStatusEnabled(bool enabled);

    // Set air conditioning request
    // false - OFF
    // true  - ON
    void setAirConditioningRequestEnabled(bool enabled);

    // Set increased heat request
    // false - OFF
    // true  - ON
    void setIncreasedHeatRequestEnabled(bool enabled);

    // Set trailer operation mode
    // false - OFF
    // true  - ON
    void setTrailerOperationModeEnabled(bool enabled);

    // Set day/night lighting
    // false - day lighting
    // true  - night lighting
    // TODO: need to confirm this
    void setDayNightLightingEnabled(bool nightEnabled);

    // Set Hood Switch
    // false - hood closed
    // true  - hood open
    // TODO: need to confirm this
    void setHoodSwitchEnabled(bool enabled);

    // Set electric cooling fan level
    // step value 0-15 (inclusive)
    void setElectricCoolingFanLevel(int level);

    // Set request raised idle
    // false - no raised idle
    // true  - raised idle
    void setRequestRaisedIdleEnabled(bool enabled);

    // Set amblient air temperature in °C
    // min: -40°C
    // max:  49°C
    // Note: this comes from a sensor directly wired to the instrument cluster
    void setAmbientAirTempC(int tempC);

    // Set amblient air temperature in °F
    // min: -40°F
    // max: 120°F
    // Note: this comes from a sensor directly wired to the instrument cluster
    void setAmbientAirTempF(int tempF);

    // Set door switch
    // false - door closed
    // true  - door open
    void setDoorSwitchEnabled(bool enabled);

    // Set hand brake switch
    // false - hand brake off
    // true  - hand brake on
    void setHandBrakeSwitchEnabled(bool enabled);

    // Set suspension level
    // Used for the auto-leveling xenons ???
    // value 0-15 (inclusive)
    void setSuspensionLevel(int level);

    // Set displayed vehicle speed in KPH
    // 10 bits, 0-1023 (inclusive) ???
    // Seems to be calculated from the wheel speed sensors that come in on "0x1F0"
    void setDisplayedVehicleSpeedKph(uint16_t speed);

    // Set displayed vehicle speed in MPH
    // 10 bits, 0-1023 (inclusive) ???
    // Seems to be calculated from the wheel speed sensors that come in on "0x1F0"
    void setDisplayedVehicleSpeedMph(uint16_t speed);
};

// ICL2 : 0x613
// Refresh Rate: 200ms
// Sent by the instrument cluster once ICL3 (0x615) is acknowledged.
//
// Note: both fuel level fields are carried as whole liters, not percentages.
class MS43_ICL2_Frame : public MS43_Frame_Send_Base {

  public:

    // Create a MS43_ICL2_Frame
    MS43_ICL2_Frame();

    // KM_CTR_CAN:
    // Set the odometer reading in kilometers
    // Stored across two bytes (LSB first) as km / 10
    // max: 655350km
    void setOdometerKm(uint32_t km);

    // FTL_CAN:
    // Set the fuel tank level as a percentage of a full tank
    // min: 0%
    // max: 100%
    //
    // Convenience wrapper: the frame itself carries liters, so this converts
    // against a full tank before writing.
    void setFuelTankLevelPercent(uint8_t percent);

    // FTL_CAN:
    // Set the fuel tank level in liters
    // min: 0L
    // max: 57L (0x39, a full tank)
    //
    // The 7-bit field counts down in liters from 0x39 toward 0x00. The low
    // fuel light comes on around 8L remaining, below which the cluster sets
    // the reserve bit and the byte reads 0x87 (nearly empty) down to 0x80.
    //
    // Also derives the driver side level, so the two stay consistent.
    void setFuelTankLevelLiters(uint8_t liters);

    // FTL_RES_CAN:
    // Set the fuel tank reserve switch
    // false - not in reserve
    // true  - in reserve
    //
    // The cluster sets this below the low fuel threshold, where the byte
    // reads 0x87 (nearly empty) down to 0x80 (empty).
    void setFuelTankReserveEnabled(bool enabled);

    // T_REL_CAN:
    // Set the running clock in minutes since power was last lost
    // Stored across two bytes (LSB first)
    // max: 65535 minutes (~45 days) before wrapping
    void setRunningClockMinutes(uint16_t minutes);

    // FTL_CAN_L:
    // Set the driver side fuel tank level in liters
    // min: 0L
    // max: 63L (the 6-bit field ceiling)
    //
    // The saddle tank has a sender per side, and the cluster reports the
    // driver side in isolation so the car can spot a failed siphon pump by
    // comparing it against the total.
    //
    // The driver side always empties first: it drains from full to empty over
    // the top half of the tank, then reads zero while the passenger side
    // drains over the bottom half. setFuelTankLevelLiters() derives this
    // automatically, so call this afterwards only to model a deliberate
    // imbalance.
    void setFuelTankLevelDriverSideLiters(uint8_t liters);
};

#endif
