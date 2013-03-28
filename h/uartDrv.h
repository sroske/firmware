//function prototypes
extern void uartInit (void);
extern void transmitResults(uint8_t sensor, _Q15 *phaseAngle, _Q15 *amplitude, bool bridgeADCClipFlag, bool coilADCClipFlag, bool bridgeDigitalClipFlag);
extern void transmitError(uint8_t errorCode);

//global variables
extern bool GUIRequestingRun;
extern uint16_t phaseMeasurementTime;//units are samples//bigger than uint16_t could cause overflows
extern uint16_t amplitudeMeasurementTime;//units are samples//bigger than uint16_t could cause overflows
extern _Q15 f1;//units are half-cycles per sample-period
extern _Q15 f2;//units are half-cycles per sample-period
extern _Q15 fdiff;//units are half-cycles per sample-period
extern _Q15 fsum;//units are half-cycles per sample-period
extern _Q15 a1;
extern _Q15 a2;

/****************************************************************
The gain factors can be 0 to 8;
Gain = 2^xxxxADCGainFactor;
All internal processing is 16 bit.
Lowest 8 bits of incoming 24bit ADC samples are truncated
These parameters left shift the 24bit samples prior to truncation
****************************************************************/
extern uint8_t bridgeADCGainFactor;
