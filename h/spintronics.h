/************
spintronics.h
************/

//#define CS4272_CONTROL_PORT_MODE //un-comment this if the CS4272 uses SPI control
//#define SIMULATION_MODE //un-comment this for a simulation specific build
//#define CODEC_USES_I2S //un-comment this if tx/rx samples from the CODEC are in I2S format; otherwise samples are left-justified
#define TRIG_USES_LUT//un-comment this to enable the LUT-based sin / cos functions; otherwise, the C30 _Q15cosPI(), _Q15sinPI() functions are used

//un-comment one of these only; this should be the same as the sample rate determined by the CODEC's xtal
//#define FORTY_EIGHT_KHZ
#define THIRTY_TWO_KHZ
//#define TWENTY_FOUR_KHZ

#ifdef FORTY_EIGHT_KHZ
#define SAMPLE_RATE 48000 //units are samples per second
#define HALF_SAMPLE_RATE 24000 //units are samples per second
#define TWICE_SAMPLE_PERIOD 41.6666666e-6 //units are seconds
#endif

#ifdef THIRTY_TWO_KHZ
#define SAMPLE_RATE 32000 //units are samples per second
#define HALF_SAMPLE_RATE 16000 //units are samples per second
#define TWICE_SAMPLE_PERIOD 62.5e-6 //units are seconds
#endif

#ifdef TWENTY_FOUR_KHZ
#define SAMPLE_RATE 24000 //units are samples per second
#define HALF_SAMPLE_RATE 12000 //units are samples per second
#define TWICE_SAMPLE_PERIOD 83.3333333e-6 //units are seconds
#endif

#define MAX_OUTPUT_FREQUENCY 0.9 * HALF_SAMPLE_RATE //units are samples per second
#define PROCESSOR_CYCLES_PER_SECOND 70000000
#define PROCESSOR_CYCLES_PER_MS 70000
#define PROCESSOR_CYCLES_PER_US 70

#ifdef SIMULATION_MODE
#define ADC_CALIBRATION_TIME 1 //units are samples
#define SETUP_TIME 10 //units are samples
#define CS4272_SPI_TX_LATENCY 1
#else
#define ADC_CALIBRATION_TIME 120000 //units are samples//must be >= specified in the CS4272 datasheet
#define SETUP_TIME 12000 //units are samples//the amount of time to let the transients settle after switching sensors
#define CS4272_SPI_TX_LATENCY 1000
#endif

#define NUMBER_OF_SENSORS 29
#define ADCDAC_GROUP_DELAY 24//units are samples//Note that pointer and pointerPlusOne in generateAndProcessSamples.c/ShiftRegister are uint8_t, so max ADCDAC_GROUP_DELAY is 254
#define V_BRIDGE_CORRESPONDING_TO_ADC_FULLSCALE_DIVIDED_BY_256 3.90625e-10//units are volts//the Wheatstone bridge voltage that corresponds to a full-scale input divided by 256
#define A_COIL_CORRESPONDING_TO_ADC_FULLSCALE 3//units are amperes//the coil current that corresponds to a full-scale input
#define FULLSCALE_BRIDGE_DAC_VOLTAGE 3.0//units are volts//TBD//the Wheatstone bridge voltage that corresponds to a full-scale output
#define FULLSCALE_COIL_DAC_VOLTAGE 3.0//untis are volts//TBD//the output voltage for the coil channel that corresponds to a full-scale output
#define NUM_FS_SAMPLES_FOR_CLIP 3//the number of consecutive samples at full-scale output by the ADC that will cause xxxx_ADC_CLIP error codes to be transmitted
#define MAX_MEASUREMENT_SAMPLES 65535//this is a hard limit due to many uint16_t variables that would otherwise overflow
#define MIN_MEASUREMENT_SAMPLES 1000//appropriate value TBD

//state encoding
#define IDLE 0
#define CALIBRATE_ADC 1
#define WAIT_FOR_HPF_DISABLE_MESSAGE_TX 2
#define START_SIGNAL_GEN 3
#define	MEASURE_PHASE 4
#define	CALCULATE_PHASE 5
#define MEASURE_AMPLITUDE 6
#define	CALCULATE_AMPLITUDE 7

//signal generator state encoding
#define RESET 0
#define RUN 1

//error codes
#define A1_OUT_OF_RANGE 0x00
#define F1_OUT_OF_RANGE 0x01
#define A2_OUT_OF_RANGE 0x02
#define F2_OUT_OF_RANGE 0x03
#define F1_PLUS_F2_OUT_OF_RANGE 0x04
#define T_OUT_OF_RANGE 0x05
#define INVALID_DIGITAL_GAIN_VALUE 0x06
#define BRIDGE_ADC_CLIP 0x07
#define COIL_ADC_CLIP 0x08
#define BRIDGE_DIGITAL_CLIP 0x09

#include <libq.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
int32_t asm16X16Mult(int16_t, int16_t);
