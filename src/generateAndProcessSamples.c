#include "p33exxxx.h"
#include "spintronics.h"
#include "uartDrv.h"
#include "muxControl.h"

_Q15 readBridgeSampleAndApplyGain(bool* bridgeDigitalClipFlag);
void signalGenerator(unsigned char runOrReset, _Q15 *freqT, _Q15 *cosOmega1T, _Q15 *cosOmega2T);
void shiftRegister( _Q15 cosOmega1T, _Q15 cosOmega2T, _Q15 *cosOmega1TTimeAligned, _Q15 *cosOmega2TTimeAligned);
void measurePhase(_Q15 bridgeSample, _Q15 coilSample, _Q15 *freqT, _Q15 cosOmega1TTimeAligned, _Q15 cosOmega2TTimeAligned, int64_t *cosAccumulator, int64_t *sinAccumulator);
void measureAmplitude(_Q15 bridgeSample, _Q15 coilSample, _Q15 *freqT, int64_t *cosAccumulator, _Q15 *phaseAngle);
void calculateShiftAmount(uint32_t timer, int64_t *cosAccumulator, int64_t *sinAccumulator, uint8_t *shiftAmt);
void calculatePhase(uint32_t timer, int64_t *cosAccumulator, int64_t *sinAccumulator, _Q15 *phaseAngle, uint8_t *shiftAmt);
void calculateAmplitude(uint32_t timer, int64_t *cosAccumulator, _Q15 *amplitude);

#ifdef TRIG_USES_LUT
extern _Q15 _Q15cosPILUT(_Q15 phiOverPI);
extern _Q15 _Q15sinPILUT(_Q15 phiOverPI);
#endif

void spintronicsStateMachine()
{
    volatile _Q15 bridgeSample;
    volatile _Q15 coilSample;
    static unsigned char state = IDLE;
    static uint32_t timer;
    static uint8_t sensor;
    _Q15 cosOmega1T;//fractions of full-scale //aligned to compensate for ADCDAC_GROUP_DELAY
    _Q15 cosOmega2T;//fractions of full-scale //aligned to compensate for ADCDAC_GROUP_DELAY
    static _Q15 cosOmega1TTimeAligned;//fractions of full-scale
    static _Q15 cosOmega2TTimeAligned;//fractions of full-scale
    static _Q15 freqT[6];//array contents: 2*f1*t, 2*f2*t, 2*f1*(t + ADCDAC_GROUP_DELAY), 2*f2*(t + ADCDAC_GROUP_DELAY), 2*fdiff*(t + ADCDAC_GROUP_DELAY), 2*fsum*(t + ADCDAC_GROUP_DELAY)
    static int64_t cosAccumulator[5];
    static int64_t sinAccumulator[5];
    static _Q15 phaseAngle[5];//units are radians divided by PI
    static _Q15 amplitude[5];//fractions of full-scale
    static bool bridgeADCClipFlag;
    static bool coilADCClipFlag;
    static bool bridgeDigitalClipFlag;

#ifdef SIMULATION_MODE
    uint16_t RXBUF2 = rand();//only to give random stimulus during simulation
#endif


    if (RXBUF0 == 0x7FFF || RXBUF0 == 0x8000)
    {
        bridgeADCClipFlag = true;
    }
    if (RXBUF2 == 0x7FFF || RXBUF2 == 0x8000)
    {
        coilADCClipFlag = true;
    }
    bridgeSample = readBridgeSampleAndApplyGain(&bridgeDigitalClipFlag);
    coilSample = RXBUF2;

    if (GUIRequestingRun == false)
    {
        state = IDLE;
    }
    
    switch (state)
    {
    case IDLE:
            timer = 0;
            sensor = 0;
            configSensor(sensor);
            signalGenerator(RESET, freqT, &cosOmega1T, &cosOmega2T);
            shiftRegister(cosOmega1T, cosOmega2T, &cosOmega1TTimeAligned, &cosOmega2TTimeAligned);
            cosAccumulator[0] = 0; cosAccumulator[1] = 0; cosAccumulator[2] = 0; cosAccumulator[3] = 0; cosAccumulator[4] = 0;
            sinAccumulator[0] = 0; sinAccumulator[1] = 0; sinAccumulator[2] = 0; sinAccumulator[3] = 0; sinAccumulator[4] = 0;
            phaseAngle[0] = 0; phaseAngle[1] = 0; phaseAngle[2] = 0; phaseAngle[3] = 0; phaseAngle[4] = 0;
            amplitude[0] = 0; amplitude[1] = 0; amplitude[2] = 0; amplitude[3] = 0; amplitude[4] = 0;
            bridgeADCClipFlag = false;
            coilADCClipFlag = false;
            bridgeDigitalClipFlag = false;

#ifdef CS4272_CONTROL_PORT_MODE
            state = CALIBRATE_ADC;
            enableADCHpf(true);//enable the hpf to calibrate for systematic DC offset
#else
            state = START_SIGNAL_GEN;
#endif

            break;

#ifdef CS4272_CONTROL_PORT_MODE
        case CALIBRATE_ADC:
            signalGenerator(RESET, freqT, &cosOmega1T, &cosOmega2T);
            shiftRegister(cosOmega1T, cosOmega2T, &cosOmega1TTimeAligned, &cosOmega2TTimeAligned);
            ++timer;
            if (timer == ADC_CALIBRATION_TIME)
            {
                timer = 0;
                state = WAIT_FOR_HPF_DISABLE_MESSAGE_TX;
                enableADCHpf(false);//DC calibration complete; disable hpf
            }
            break;

         case WAIT_FOR_HPF_DISABLE_MESSAGE_TX:
            signalGenerator(RESET, freqT, &cosOmega1T, &cosOmega2T);
            shiftRegister(cosOmega1T, cosOmega2T, &cosOmega1TTimeAligned, &cosOmega2TTimeAligned);
            ++timer;
            if (timer == CS4272_SPI_TX_LATENCY)
            {
                timer = 0;
                state = START_SIGNAL_GEN;
            }
            break;
#endif

        case START_SIGNAL_GEN:
            signalGenerator(RUN, freqT, &cosOmega1T, &cosOmega2T);
            shiftRegister(cosOmega1T, cosOmega2T, &cosOmega1TTimeAligned, &cosOmega2TTimeAligned);
            ++timer;
            if (timer == SETUP_TIME)
            {
                timer = 0;
                state = MEASURE_PHASE;
            }
            break;            

        case MEASURE_PHASE:
            measurePhase(bridgeSample, coilSample, freqT, cosOmega1TTimeAligned, cosOmega2TTimeAligned, cosAccumulator, sinAccumulator);
            signalGenerator(RUN, freqT, &cosOmega1T, &cosOmega2T);
            shiftRegister(cosOmega1T, cosOmega2T, &cosOmega1TTimeAligned, &cosOmega2TTimeAligned);
            ++timer;
            if (timer == phaseMeasurementTime)
            {
                timer = 0;
                state = CALCULATE_PHASE;
            }
            break;

        case CALCULATE_PHASE:
        {
            static uint8_t shiftAmt[5];
            if (timer < 5)
            {
                calculateShiftAmount(timer, cosAccumulator, sinAccumulator, shiftAmt);
            }
			else
            {
                calculatePhase(timer - 5, cosAccumulator, sinAccumulator, phaseAngle, shiftAmt);
            }
            signalGenerator(RUN, freqT, &cosOmega1T, &cosOmega2T);
            shiftRegister(cosOmega1T, cosOmega2T, &cosOmega1TTimeAligned, &cosOmega2TTimeAligned);
            ++timer;
            if (timer == 10)
            {    
                cosAccumulator[0] = 0; cosAccumulator[1] = 0; cosAccumulator[2] = 0; cosAccumulator[3] = 0; cosAccumulator[4] = 0; 
                sinAccumulator[0] = 0; sinAccumulator[1] = 0; sinAccumulator[2] = 0; sinAccumulator[3] = 0; sinAccumulator[4] = 0; 
                timer = 0;
                state = MEASURE_AMPLITUDE;
            }
            break;
        }

        case MEASURE_AMPLITUDE:
            measureAmplitude(bridgeSample, coilSample, freqT, cosAccumulator, phaseAngle);
            signalGenerator(RUN, freqT, &cosOmega1T, &cosOmega2T);
            shiftRegister(cosOmega1T, cosOmega2T, &cosOmega1TTimeAligned, &cosOmega2TTimeAligned);
            ++timer;
            if (timer == amplitudeMeasurementTime)
            {
                timer = 0;
                state = CALCULATE_AMPLITUDE;
            }
            break;

        case CALCULATE_AMPLITUDE:
            calculateAmplitude(timer, cosAccumulator, amplitude);
            signalGenerator(RUN, freqT, &cosOmega1T, &cosOmega2T);
            shiftRegister(cosOmega1T, cosOmega2T, &cosOmega1TTimeAligned, &cosOmega2TTimeAligned);
            ++timer;  
            if (timer == 5)
            {
                uint8_t index;
                uint8_t txSensor;//temporary variable to store the value for transmission
                _Q15 txPhaseAngle[5];//temporary array to store values for transmission
                _Q15 txAmplitude[5];//temporary array to store values for transmission
                bool txBridgeADCClipFlag;//temporary variable to store the flag for transmission
                bool txCoilADCClipFlag;//temporary variable to store the flag for transmission
                bool txBridgeDigitalClipFlag;//temporary variable to store the flag for transmission

                //store variables for transmission
                txSensor = sensor;
                for (index = 0; index < 5; ++index)
                {
                    txPhaseAngle[index] = phaseAngle[index];
                    txAmplitude[index] = amplitude[index];
                }
                txBridgeADCClipFlag = bridgeADCClipFlag;
                txCoilADCClipFlag = coilADCClipFlag;
                txBridgeDigitalClipFlag = bridgeDigitalClipFlag;

                //clear static variables for next iteration of the state machine
                cosAccumulator[0] = 0; cosAccumulator[1] = 0; cosAccumulator[2] = 0; cosAccumulator[3] = 0; cosAccumulator[4] = 0; 
                timer = 0;
                                bridgeADCClipFlag = false;
                                coilADCClipFlag = false;
                                bridgeDigitalClipFlag = false;

                state = START_SIGNAL_GEN;
                ++sensor;
                if (sensor == NUMBER_OF_SENSORS)
                {
                    sensor = 0;
                }
                configSensor(sensor);

                //transmit results
                transmitResults(txSensor, txPhaseAngle, txAmplitude, txBridgeADCClipFlag, txCoilADCClipFlag, txBridgeDigitalClipFlag);//don't do this until the state machine is ready for the next measurment period; transmitting results takes more than one sample period.
            }
            break;
            
        default:
            timer = 0;
            sensor = 0;
            configSensor(sensor);
            signalGenerator(RESET, freqT, &cosOmega1T, &cosOmega2T);
            shiftRegister(cosOmega1T, cosOmega2T, &cosOmega1TTimeAligned, &cosOmega2TTimeAligned);
            cosAccumulator[0] = 0; cosAccumulator[1] = 0; cosAccumulator[2] = 0; cosAccumulator[3] = 0; cosAccumulator[4] = 0; 
            sinAccumulator[0] = 0; sinAccumulator[1] = 0; sinAccumulator[2] = 0; sinAccumulator[3] = 0; sinAccumulator[4] = 0; 
            phaseAngle[0] = 0; phaseAngle[1] = 0; phaseAngle[2] = 0; phaseAngle[3] = 0; phaseAngle[4] = 0;
            amplitude[0] = 0; amplitude[1] = 0; amplitude[2] = 0; amplitude[3] = 0; amplitude[4] = 0; 
            break;
    }
}

_Q15 readBridgeSampleAndApplyGain(bool* bridgeDigitalClipFlag)
{

#ifdef SIMULATION_MODE
    uint16_t RXBUF0 = rand();//only to give random stimulus during simulation
    uint16_t RXBUF1 = rand();//only to give random stimulus during simulation
#endif
        
    if (bridgeADCGainFactor == 0)
    {
        return RXBUF0;
    }
    else
    {
        int16_t clipTest;
        int16_t maskForTruncationBitsPlusOne = 0x8000;//make it signed so that the shift uses sign extension
        maskForTruncationBitsPlusOne = maskForTruncationBitsPlusOne >> bridgeADCGainFactor;
        clipTest = RXBUF0 & maskForTruncationBitsPlusOne;
        if (clipTest != 0x0000 && clipTest != maskForTruncationBitsPlusOne)
        {
            *bridgeDigitalClipFlag = true;
            if ((int16_t)RXBUF0 < 0)
            {
                return 0x8000;//interpret the clipped sample as the most negative value
            }
            else
            {
                return 0x7FFF;//interpret the clipped sample as the most positive value
            }
        }
        else
        {
            uint32_t tempSample;
            uint16_t *tempSampleLowWord = (uint16_t *)&tempSample;
            uint16_t *tempSampleHighWord = tempSampleLowWord + 1;
            *tempSampleLowWord = RXBUF1;
            *tempSampleHighWord = RXBUF0;
            *tempSampleLowWord = *tempSampleLowWord >> (16 - bridgeADCGainFactor);
            *tempSampleHighWord = *tempSampleHighWord << bridgeADCGainFactor;
            return *tempSampleLowWord + *tempSampleHighWord;
        }
    }
}

void shiftRegister(_Q15 cosOmega1T, _Q15 cosOmega2T, _Q15 *cosOmega1TTimeAligned, _Q15 *cosOmega2TTimeAligned)
{
    static _Q15 delayArrayA[ADCDAC_GROUP_DELAY];
    static _Q15 delayArrayB[ADCDAC_GROUP_DELAY];

    static int16_t pointer = 0;
    int16_t pointerPlusOne = pointer + 1;
    if(pointerPlusOne == ADCDAC_GROUP_DELAY)
    {
        pointerPlusOne = 0;
    }
    delayArrayA[pointer] = cosOmega1T;
    delayArrayB[pointer] = cosOmega2T;
    *cosOmega1TTimeAligned = delayArrayA[pointerPlusOne];
    *cosOmega2TTimeAligned = delayArrayB[pointerPlusOne];
    ++pointer;
        if(pointer == ADCDAC_GROUP_DELAY)
    {
        pointer = 0;
    }
}

void signalGenerator(unsigned char runOrReset, _Q15 *freqT, _Q15 *cosOmega1T, _Q15 *cosOmega2T)
{
    if (runOrReset == RUN)
    {
#ifdef TRIG_USES_LUT
        *cosOmega1T = _Q15cosPILUT(freqT[0]);//generating cos(omega1 * t)
        *cosOmega2T = _Q15cosPILUT(freqT[1]);//generating cos(omega2 * t)
#else
        *cosOmega1T = _Q15cosPI(freqT[0]);//generating cos(omega1 * t)
        *cosOmega2T = _Q15cosPI(freqT[1]);//generating cos(omega2 * t)
#endif

        uint32_t tempSample;
        if (a1 == 0x7FFF)
        {
            TXBUF0 = *cosOmega1T;
            TXBUF1 = 0x0000;
        }
        else
        {
            tempSample = asm16X16Mult(*cosOmega1T, a1);
            TXBUF0 = *((uint16_t *)&tempSample + 1);
            TXBUF1 = *((uint16_t *)&tempSample);
        }

        if (a2 == 0x7FFF)
        {
            TXBUF2 = *cosOmega2T;
            TXBUF3 = 0x0000;
        }
        else
        {
            tempSample = asm16X16Mult(*cosOmega2T, a2);
            TXBUF2 = *((uint16_t *)&tempSample + 1);
            TXBUF3 = *((uint16_t *)&tempSample);
        }

        freqT[0] += f1;
        freqT[1] += f2;
        freqT[2] += f1;
        freqT[3] += f2;
        freqT[4] += fdiff;
        freqT[5] += fsum;
    }
    
    else
    {
        *cosOmega1T = 0;
        *cosOmega2T = 0;
        TXBUF0 = 0x0000;
        TXBUF1 = 0x0000;
        TXBUF2 = 0x0000;
        TXBUF3 = 0x0000;
        freqT[0] = 0;
        freqT[1] = 0;
        freqT[2] = (f1 * ADCDAC_GROUP_DELAY) % 65536;
        freqT[3] = (f2 * ADCDAC_GROUP_DELAY) % 65536;
        freqT[4] = (fdiff * ADCDAC_GROUP_DELAY) % 65536;
        freqT[5] = (fsum * ADCDAC_GROUP_DELAY) % 65536;
    }
}

void measurePhase(_Q15 bridgeSample, _Q15 coilSample, _Q15 *freqT, _Q15 cosOmega1TTimeAligned, _Q15 cosOmega2TTimeAligned, int64_t *cosAccumulator, int64_t *sinAccumulator)
{
#ifdef TRIG_USES_LUT
    _Q15 cosFDiffT = _Q15cosPILUT(freqT[4]);
    _Q15 cosFSumT = _Q15cosPILUT(freqT[5]);
    _Q15 sinF1T = _Q15sinPILUT(freqT[2]);
    _Q15 sinF2T = _Q15sinPILUT(freqT[3]);
    _Q15 sinFDiffT = _Q15sinPILUT(freqT[4]);
    _Q15 sinFSumT = _Q15sinPILUT(freqT[5]);
#else
    _Q15 cosFDiffT = _Q15cosPI(freqT[4]);
    _Q15 cosFSumT = _Q15cosPI(freqT[5]);
    _Q15 sinF1T = _Q15sinPI(freqT[2]);
    _Q15 sinF2T = _Q15sinPI(freqT[3]);
    _Q15 sinFDiffT = _Q15sinPI(freqT[4]);
    _Q15 sinFSumT = _Q15sinPI(freqT[5]);
#endif
    cosAccumulator[0] = cosAccumulator[0] + asm16X16Mult(bridgeSample, cosOmega1TTimeAligned);
    cosAccumulator[1] = cosAccumulator[1] + asm16X16Mult(bridgeSample, cosOmega2TTimeAligned);
    cosAccumulator[2] = cosAccumulator[2] + asm16X16Mult(bridgeSample, cosFDiffT);
    cosAccumulator[3] = cosAccumulator[3] + asm16X16Mult(bridgeSample, cosFSumT);
    cosAccumulator[4] = cosAccumulator[4] + asm16X16Mult(coilSample, cosOmega2TTimeAligned);

    sinAccumulator[0] = sinAccumulator[0] + asm16X16Mult(bridgeSample, sinF1T);
    sinAccumulator[1] = sinAccumulator[1] + asm16X16Mult(bridgeSample, sinF2T);
    sinAccumulator[2] = sinAccumulator[2] + asm16X16Mult(bridgeSample, sinFDiffT);
    sinAccumulator[3] = sinAccumulator[3] + asm16X16Mult(bridgeSample, sinFSumT);
    sinAccumulator[4] = sinAccumulator[4] + asm16X16Mult(coilSample, sinF2T);
}

void measureAmplitude(_Q15 bridgeSample, _Q15 coilSample, _Q15 *freqT, int64_t *cosAccumulator, _Q15 *phaseAngle)
{    
    _Q15 freqTPlusPhi[4];
    _Q15 f2TPlusCoilPhi;
    uint8_t i;
    _Q15 cosF1TPlusPhi;
    _Q15 cosF2TPlusPhi;
    _Q15 cosFDiffTPlusPhi;
    _Q15 cosFSUMTPlusPhi;
    _Q15 cosF2TPlusCoilPhi;
    
    for (i = 0; i < 4; ++i)
    {
        freqTPlusPhi[i] = freqT[i + 2] + phaseAngle[i];
    }
    
    f2TPlusCoilPhi = freqT[3] + phaseAngle[4];

#ifdef TRIG_USES_LUT
    cosF1TPlusPhi = _Q15cosPILUT(freqTPlusPhi[0]);
    cosF2TPlusPhi = _Q15cosPILUT(freqTPlusPhi[1]);
    cosFDiffTPlusPhi = _Q15cosPILUT(freqTPlusPhi[2]);
    cosFSUMTPlusPhi = _Q15cosPILUT(freqTPlusPhi[3]);
    cosF2TPlusCoilPhi = _Q15cosPILUT(f2TPlusCoilPhi);
#else
    cosF1TPlusPhi = _Q15cosPI(freqTPlusPhi[0]);
    cosF2TPlusPhi = _Q15cosPI(freqTPlusPhi[1]);
    cosFDiffTPlusPhi = _Q15cosPI(freqTPlusPhi[2]);
    cosFSUMTPlusPhi = _Q15cosPI(freqTPlusPhi[3]);
    cosF2TPlusCoilPhi = _Q15cosPI(f2TPlusCoilPhi);
#endif
    cosAccumulator[0] = cosAccumulator[0] + asm16X16Mult(bridgeSample, cosF1TPlusPhi);
    cosAccumulator[1] = cosAccumulator[1] + asm16X16Mult(bridgeSample, cosF2TPlusPhi);
    cosAccumulator[2] = cosAccumulator[2] + asm16X16Mult(bridgeSample, cosFDiffTPlusPhi);
    cosAccumulator[3] = cosAccumulator[3] + asm16X16Mult(bridgeSample, cosFSUMTPlusPhi);
    cosAccumulator[4] = cosAccumulator[4] + asm16X16Mult(coilSample, cosF2TPlusCoilPhi);
}

void calculateShiftAmount(uint32_t timer, int64_t *cosAccumulator, int64_t *sinAccumulator, uint8_t *shiftAmt)
{
    shiftAmt[timer] = 0;
    uint16_t mask = 0xFFFF;
    uint16_t cosAccumTopBits = (uint16_t)_Q15abs(*((_Q15 *)(cosAccumulator + timer) + 2));
    uint16_t sinAccumTopBits = (uint16_t)_Q15abs(*((_Q15 *)(sinAccumulator + timer) + 2));
    while(cosAccumTopBits != 0x0000 || sinAccumTopBits != 0x0000)
    {
        ++shiftAmt[timer];
        if (shiftAmt[timer] == 15)
        {
            break;
        }
        mask = mask << 1;
        cosAccumTopBits = mask & cosAccumTopBits;
        sinAccumTopBits = mask & sinAccumTopBits;
    }
    ++shiftAmt[timer];
}

void calculatePhase(uint32_t timer, int64_t *cosAccumulator, int64_t *sinAccumulator, _Q15 *phaseAngle, uint8_t *shiftAmt)
{
    /*
     * A1 is the reference amplitude, A2 is the measured amplitude
     * max xxxAccumulator[i] value is given by A1 * A2 * phaseMeasurmentTime / 2.
     * A1 and A2 are 16 bit; the asm16X16Mult() left justifies the result in a 32-bit register
     * phase MeasurementTime <= 65535 (16bit)
     * thus, xxxAccumulator[i] may be up to 48 bits in length
     * SO shifting both arguments right by 16
     * ensures that the values will fit within the _Q16 (32bit) type
     * HOWEVER, we can retain maximum precision by only shifting the
     * arguments as necessary!
     * The accumulators look like this:
     * 0x 0000 xxxx xxxx xxxx
     * bits 47-32 may or may not contain data;
     * It will be assumed that bit32 may be a sign bit, thus
     * 1 is the minimum right shift to fit the data as 32-bit
     * arguments
    */
    phaseAngle[timer] = (_Q15)(_Q16atanYByXByPI(_Q16neg((_Q16)(cosAccumulator[timer] >> shiftAmt[timer])), (_Q16)sinAccumulator[timer] >> shiftAmt[timer]) >> 1);//shift right by one to convert from _Q16 to _Q15
}

void calculateAmplitude(uint32_t timer, int64_t *cosAccumulator, _Q15 *amplitude)
{
    /*
     * A1 is the reference amplitude, A2 is the measured amplitude
     * max xxxAccumulator[i] value is given by A1 * A2 * amplitudeMeasurmentTime / 2.
     * A1 and A2 are 16 bit; the asm16X16Mult() left justifies the result in a 32-bit register
     * phase MeasurementTime <= 65535 (16bit)
     * thus, xxxAccumulator[i] may be up to 48 bits in length
     * SO shifting both arguments right by 16
     * ensures that the values will fit within the _Q16 (32bit) type
     * Shifting right by 16 also divides by 65536
     * Note that A1 is 32768, and that the asm16X16Mult() left
     * justifies the 31-bit product in the 32bit register;
     * Shifting right by 16 takes care of both of these factors.
     * Thus the result of the computation below is A2 in Q15 format,
     * a normalized signal amplitude
     */
    amplitude[timer] = (_Q15)(((int32_t)(cosAccumulator[timer] >> 16)) / (amplitudeMeasurementTime >> 1));
}
