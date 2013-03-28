#include "p33exxxx.h"
#include "spintronics.h"
#include "CS4272.h"
#include "spiDrv.h"
#include "uartDrv.h"
#include <TIME.H>

void setCS4272ResetState(bool reset);
void sleep_ms(uint16_t ms);

void cs4272Init(void)
{
    
#if defined(CS4272_CONTROL_PORT_MODE)
	spiInit();//initialize the SPI1 module for cummunication with the CS4272
#endif

    // Setup RE1 for connection to RSTbar on CS4272
    ANSELE = ANSELE & ~0x0001; //set RE0 to be a digital pin
    TRISEbits.TRISE0 = 0; // set RE0 to be an output


#ifndef SIMULATION_MODE
    setCS4272ResetState(true); // bring RST_bar low
    sleep_ms(100); // wait 100ms
    setCS4272ResetState(false); // bring RST_bar high
    sleep_ms(2); // wait 2ms
#endif

#if defined(CS4272_CONTROL_PORT_MODE)
    //CODEC START SEQUENCE
    writeCS4272Register(MODE_CONTROL_2, MODE_CONTROL_2_PDN | MODE_CONTROL_2_CPEN);// write 0x03 to register 0x07 within 10ms of bringing RST_bar high
    sleep(2);
    writeCS4272Register(MODE_CONTROL_1,MODE_CONTROL_ONE_DEFAULT_PAYLOAD);//write 0x29 to register 0x01
    sleep(2);
    writeCS4272Register(DAC_CONTROL, DAC_CONTROL_DEFAULT_PAYLOAD);
    sleep(2);
    writeCS4272Register(DAC_VOLUME, DAC_VOLUME_DEFAULT_PAYLOAD);
    sleep(2);
    writeCS4272Register(DAC_VOLUME_A, 0x00);
    sleep(2);
    writeCS4272Register(DAC_VOLUME_B, 0x00);
    sleep(2);
    writeCS4272Register(ADC_CONTROL, ADC_CONTROL_DITHER16);
    sleep(2);
    writeCS4272Register(MODE_CONTROL_2, MODE_CONTROL_2_CPEN);
    sleep(2);
#endif

    i2sInit();//initialize the DCI module for communication with CS4272
}

void setCS4272ResetState(bool reset)
{
    //if reset is true, hold the pin low; if reset is false hold the pin high
    PORTEbits.RE0 =(!reset);
}

void sleep_ms(uint16_t ms)
{
    clock_t start, stop, elapsed;
    int32_t cyclesToSleep = PROCESSOR_CYCLES_PER_MS * ms - 87;//subtract cycles to compensate for multiplication and funciton call overhead
    
    if (cyclesToSleep > 0)
    {
        start = clock();
        do
        {
            stop = clock();
            if (start < stop)
            {
                elapsed = stop - start;
            }
            else
            {
                elapsed = (clock_t)((uint32_t)stop - (uint32_t)start);
            }
        } while (elapsed < cyclesToSleep);
    }
}

#if defined(CS4272_CONTROL_PORT_MODE)
void enableADCHpf(bool enable)
{
    uint8_t dither16State;
    if (bridgeADCGainFactor == 0) // if we are going to use bits lower than the 16 MSB, we cannot dither to 16 bits
    {
        dither16State = ADC_CONTROL_DITHER16;
    }
    else
    {
        dither16State = 0;
    }
    if (enable)
    {
        writeCS4272Register(ADC_CONTROL, dither16State);
    }
    else
    {
        writeCS4272Register(ADC_CONTROL, ADC_CONTROL_HPF_DISABLE_B | ADC_CONTROL_HPF_DISABLE_A | dither16State);
    }
}
#endif
