#include "p33exxxx.h"
#include "spintronics.h"
#include "uartDrv.h"
//
//global variables
bool GUIRequestingRun;
uint16_t phaseMeasurementTime;//units are samples//bigger than uint16_t could cause overflows
uint16_t amplitudeMeasurementTime;//units are samples//bigger than uint16_t could cause overflows
_Q15 f1;//units are half-cycles per sample-period
_Q15 f2;//units are half-cycles per sample-period
_Q15 fdiff;//units are half-cycles per sample-period
_Q15 fsum;//units are half-cycles per sample-period
uint8_t bridgeADCGainFactor;//this can be 0, 1, 2, 3, 4, 5, 6, 7, 8; gain = 2^bridgeADCGainFactor;//in practice, only 0, 1, 2, 3, 4 offer any advantage
bool f1PlusF2OutOfRangeFlag;
_Q15 a1;
_Q15 a2;

float setVolume(uint8_t channel, float voltage);

void processStartCommand(float GUISpecifiedA1, float GUISpecifiedF1, float GUISpecifiedA2, float GUISpecifiedF2, float GUISpecifiedT, uint8_t GUISpeciedBridgeGainFactor)
{
    float implementedA1, implementedF1, implementedA2, implementedF2, implementedT, maxOutputFrequency, maxMeasurementSamples, minMeasurementSamples;
    uint8_t implementedBridgeGainFactor;
    float tempTime;

    implementedA1 = setVolume(0, GUISpecifiedA1);
    implementedA2 = setVolume(1, GUISpecifiedA2);

    maxOutputFrequency = MAX_OUTPUT_FREQUENCY;
    maxMeasurementSamples = MAX_MEASUREMENT_SAMPLES;
    minMeasurementSamples = MIN_MEASUREMENT_SAMPLES;

    if (GUISpecifiedF1 > maxOutputFrequency)
    {
        transmitError(F1_OUT_OF_RANGE);
        f1 = _Q15ftoi(maxOutputFrequency * TWICE_SAMPLE_PERIOD);
        implementedF1 = maxOutputFrequency;
    }
    else if (GUISpecifiedF1 < 0)
    {
        transmitError(F1_OUT_OF_RANGE);
        f1 = 0;
        implementedF1 = 0.0;
    }
    else
    {
        f1 = _Q15ftoi(GUISpecifiedF1 * TWICE_SAMPLE_PERIOD);
        implementedF1 = _itofQ15(f1) * HALF_SAMPLE_RATE;
    }

    if (GUISpecifiedF2 > maxOutputFrequency)
    {
        transmitError(F2_OUT_OF_RANGE);
        f2 = _Q15ftoi(maxOutputFrequency * TWICE_SAMPLE_PERIOD);
        implementedF2 = maxOutputFrequency;
    }
    else if (GUISpecifiedF2 < 0)
    {
        transmitError(F2_OUT_OF_RANGE);
        f2 = 0;
        implementedF2 = 0.0;
    }
    else
    {
        f2 = _Q15ftoi(GUISpecifiedF2 * TWICE_SAMPLE_PERIOD);
        implementedF2 = _itofQ15(f2) * HALF_SAMPLE_RATE;
    }

    f1PlusF2OutOfRangeFlag = false;
    if (GUISpecifiedF1 + GUISpecifiedF2 > maxOutputFrequency)
    {
        f1PlusF2OutOfRangeFlag = true;
        transmitError(F1_PLUS_F2_OUT_OF_RANGE);
        fsum = 0;
    }
    else
    {
        fsum = f1 + f2;
    }
    fdiff = _Q15abs(f1 - f2);

    tempTime = GUISpecifiedT * HALF_SAMPLE_RATE;
    if (tempTime > maxMeasurementSamples)
    {
        transmitError(T_OUT_OF_RANGE);
        phaseMeasurementTime = MAX_MEASUREMENT_SAMPLES;
        amplitudeMeasurementTime = MAX_MEASUREMENT_SAMPLES;
        implementedT = maxMeasurementSamples / HALF_SAMPLE_RATE;
    }
    else if (tempTime < minMeasurementSamples)
    {
        transmitError(T_OUT_OF_RANGE);
        phaseMeasurementTime = MIN_MEASUREMENT_SAMPLES;
        amplitudeMeasurementTime = MIN_MEASUREMENT_SAMPLES;
        implementedT = minMeasurementSamples / HALF_SAMPLE_RATE;
    }
    else
    {
        phaseMeasurementTime = (uint16_t)tempTime;
        amplitudeMeasurementTime = phaseMeasurementTime;
        implementedT = (float)phaseMeasurementTime / HALF_SAMPLE_RATE;
    }

    switch(GUISpeciedBridgeGainFactor)
    {
        case 1:
        {
            bridgeADCGainFactor = 0;
            implementedBridgeGainFactor = 1;
            break;
        }
        case 2:
        {
            bridgeADCGainFactor = 1;
            implementedBridgeGainFactor = 2;
            break;
        }
        case 4:
        {
            bridgeADCGainFactor = 2;
            implementedBridgeGainFactor = 4;
            break;
        }
        case 8:
        {
            bridgeADCGainFactor = 3;
            implementedBridgeGainFactor = 8;
            break;
        }
        case 16:
        {
            bridgeADCGainFactor = 4;
            implementedBridgeGainFactor = 16;
            break;
        }
        default:
        {
            transmitError(INVALID_DIGITAL_GAIN_VALUE);
            bridgeADCGainFactor = 0;
            implementedBridgeGainFactor = 1;
            break;
        }
    }

    GUIRequestingRun = true;
}

float setVolume(uint8_t channel, float voltage)
{
    if (channel == 0x00)
    {
        if (voltage > FULLSCALE_BRIDGE_DAC_VOLTAGE)
        {
            transmitError(A1_OUT_OF_RANGE);
            a1 = 0x7FFF;
            voltage = FULLSCALE_BRIDGE_DAC_VOLTAGE;
        }
        else if (voltage < 0.0)
        {
            transmitError(A1_OUT_OF_RANGE);
            a1 = 0x0000;
            voltage = 0.0;
        }
        else
        {
            a1 = _Q15ftoi(voltage / FULLSCALE_BRIDGE_DAC_VOLTAGE);
            voltage = _itofQ15(a1) * FULLSCALE_BRIDGE_DAC_VOLTAGE;
        }
    }
    else
    {
        if (voltage > FULLSCALE_COIL_DAC_VOLTAGE)
        {
            transmitError(A2_OUT_OF_RANGE);
            a2 = 0x7FFF;
            voltage = FULLSCALE_COIL_DAC_VOLTAGE;
        }
        else if (voltage < 0.0)
        {
            transmitError(A2_OUT_OF_RANGE);
            a2 = 0x0000;
            voltage = 0.0;
        }
        else
        {
            a2 = _Q15ftoi(voltage / FULLSCALE_COIL_DAC_VOLTAGE);
            voltage = _itofQ15(a2) * FULLSCALE_COIL_DAC_VOLTAGE;
        }
    }
    return voltage;
}

void uart_Init (void)
{
        IPC3bits.U1TXIP= 2;		//TX Priority 2
	IEC0bits.U1TXIE = 1;            // Enable UART TX Interrupt
        IFS0bits.U1TXIF= 0;
	IPC2bits.U1RXIP= 2;		//RX priority
	IEC0bits.U1RXIE= 1;
	IFS0bits.U1RXIF= 0;

	U1MODEbits.USIDL= 0;
	U1MODEbits.IREN= 0;
	U1MODEbits.RTSMD= 1;			//U1RTS in Simplex mode
	U1MODEbits.UEN= 0;
	U1MODEbits.WAKE= 0;
	U1MODEbits.ABAUD= 0;
	U1MODEbits.URXINV= 0;
	U1MODEbits.BRGH= 1;			//Set for High-Speed mode
	U1MODEbits.PDSEL= 0;			//8-bit mode, no parity
	U1MODEbits.STSEL= 0;			//1-stop bit
	U1MODEbits.UARTEN= 0;			//UART1 is Disabled
	U1MODEbits.LPBACK = 0;			//LPBACK Disabled

        U1STA= 0x0400;  //enable UART.TXEN

//****************************** Baud rate Calculations*********************************************************//
//																												
//	U1BRG= [(Fcy/(Desired_Baud_rate*16)] - 1........... Provided BRGH= 0 (Slow mode)							
//	U1BRG= [Fcy/(4*Baud_rate)] - 1................... Provided BRGH= 1 (Fast mode)								
//	Fosc= 80Mhz																									
//	Fcy= Fosc/2= 40MHz																		
//																	
//																												
//**************************************************************************************************************//

//  U1BRG= 86;		//Set for baudrate of 115200 Baud
//  U1BRG= 42;		//Set for baudrate of 230400 Baud
    U1BRG= 10;		//Set for baudrate of 921600 Baud

    GUIRequestingRun = false;
    phaseMeasurementTime = MAX_MEASUREMENT_SAMPLES;
    amplitudeMeasurementTime = MAX_MEASUREMENT_SAMPLES;
    f1 = 0;
    f2 = 0;
    fdiff = 0;
    fsum = 0;
    bridgeADCGainFactor = 1;
    f1PlusF2OutOfRangeFlag = false;
    a1 = 0x7FFF;
    a2 = 0x7FFF;

}


void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF= 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF= 0;
}

void transmitResults(uint8_t sensor, _Q15 *phaseAngle, _Q15 *amplitude, bool bridgeADCClipFlag, bool coilADCClipFlag, bool bridgeDigitalClipFlag)
{
	/* NOTE: bridge voltages are calculated with a floating-point multiply
     * because the FULLSCALE_BRIDGE_ADC_VOLTAGE is too small to be accurately
     * represented by a _Q16
     */
    
	/* NOTE: calculating the bridge voltages proceeds as follows:
     * amplitude[] contains _Q15 fractions of full-scale observed at the ADC
     * multiplied by 2^bridgeADCGainFactor; casting to a _Q16 has the effect
     * of dividing by 2; shifting left by (9 - bridgeADCGainFactor) results
     * in an argument for _itofQ16() that is the fraction of ADC fullscale
     * multiplied by 256.
     * Multiplying by V_BRIDGE_CORRESPONDING_TO_ADC_FULLSCALE_DIVIDED_BY_256
     * returns a float that has units of volts that were actually observed
     * across the Wheatstone bridge.
     * V_BRIDGE_CORRESPONDING_TO_ADC_FULLSCALE_DIVIDED_BY_256 must be
     * calculated after considering the Wheatstone bridge board buffer amp
     * gain, the ADC buffer amp gain and the voltage corresponding to
     * a fullscale reading at the ADC
     */
    float f1BridgeVolts = _itofQ16(_Q16shlNoSat((_Q16)amplitude[0], 9 - bridgeADCGainFactor)) * V_BRIDGE_CORRESPONDING_TO_ADC_FULLSCALE_DIVIDED_BY_256;
    float f2BridgeVolts = _itofQ16(_Q16shlNoSat((_Q16)amplitude[1], 9 - bridgeADCGainFactor)) * V_BRIDGE_CORRESPONDING_TO_ADC_FULLSCALE_DIVIDED_BY_256;
    float fdiffBridgeVolts = _itofQ16(_Q16shlNoSat((_Q16)amplitude[2], 9 - bridgeADCGainFactor)) * V_BRIDGE_CORRESPONDING_TO_ADC_FULLSCALE_DIVIDED_BY_256;
    float fsumBridgeVolts;
    if (f1PlusF2OutOfRangeFlag)
    {
        fsumBridgeVolts = 0;
    }
    else
    {
        fsumBridgeVolts = _itofQ16(_Q16shlNoSat((_Q16)amplitude[3], 9 - bridgeADCGainFactor)) * V_BRIDGE_CORRESPONDING_TO_ADC_FULLSCALE_DIVIDED_BY_256;
    }
    float f2CoilAmps = _itofQ16((_Q16)asm16X16Mult(amplitude[4], A_COIL_CORRESPONDING_TO_ADC_FULLSCALE));//casting to _Q16, divides by 2; asm16X16Mult left justifies the 31 bit product in a 32-bit register, thereby multiplying by 2

    /* phaseAngle[] is in units of radians divided by pi.
     * we must multiply by 180 to get units of degrees.
     * casting to _Q16 has the effect of dividing by two,
     * while using asm16X16Mult returns a result that is
     * shifted left by one bit
     */
    float f1BridgeDegrees = _itofQ16((_Q16)asm16X16Mult(phaseAngle[0], 180));
    float f2BridgeDegrees = _itofQ16((_Q16)asm16X16Mult(phaseAngle[1], 180));
    float fdiffBridgeDegrees = _itofQ16((_Q16)asm16X16Mult(phaseAngle[2], 180));
    float fsumBridgeDegrees;
    if (f1PlusF2OutOfRangeFlag)
    {
        fsumBridgeDegrees = 0;
    }
    else
    {
        fsumBridgeDegrees = _itofQ16((_Q16)asm16X16Mult(phaseAngle[3], 180));
    }
    float f2CoilDegrees = _itofQ16((_Q16)asm16X16Mult(phaseAngle[4], 180));

    //TODO: output the results via the UART

    if (bridgeADCClipFlag)
    {
        transmitError(BRIDGE_ADC_CLIP);
    }
    if (coilADCClipFlag)
    {
        transmitError(COIL_ADC_CLIP);
    }
    if (bridgeDigitalClipFlag)
    {
        transmitError(BRIDGE_DIGITAL_CLIP);
    }
}

void transmitError(uint8_t errorCode)
{
    //TODO: output an error code via UART
}
