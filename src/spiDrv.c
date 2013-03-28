#include "p33exxxx.h"
#include "spintronics.h"
#include <TIME.H>

#define CS  PORTEbits.RE1
#define SECONDARY_CS PORTFbits.RF1

void sleepMicroSecs(uint16_t us);

void spiInit(void)
{
    ANSELE = ANSELE & ~0x0017;//RE0, RE1, RE2, RE4 to be digital pins
    RPOR5bits.RP82R = 0b000101; //SDA/CDIN
    RPOR5bits.RP84R = 0b000110; // SCL/CCLK
    TRISEbits.TRISE1 = 0;   //set Chip Select (CS) pin to output
    TRISFbits.TRISF1 = 0; //RF1 as CS for 2nd DAC

    //RPOR5bits.RP82R = 0b000111;//route SS1bar to PIN62/RP82/RE2 (not using)

    //In master mode, SS1* pin not controlled by module. Should be
    //configured as GPIO by setting SSEN (SPI1CON<7>)=1:
    SPI1CON1bits.SSEN = 1;
    PORTEbits.RE1 = 1;      //set CS = 1
    PORTFbits.RF1 = 1;

    //TODO: initialize the SPI port
    //use SPI1
    //set to 8-bit mode
    //data rate = 136 kHz (SPRE = 000, PPRE = 00)
    //disable interrupt for SPI1 receive
    //Master mode
    //non-framed SPI

    //SPI Port Configuration:
    IFS0bits.SPI1IF = 0;        //clear interrupt flag
    IEC0bits.SPI1IE = 0;        //disable interrupt

    SPI1CON1bits.DISSCK = 0;    //enable internal clock
    SPI1CON1bits.DISSDO = 0;    //enable SDO1 control by module
    SPI1CON1bits.MODE16 = 0;    //put module into 8-bit mode
    SPI1CON1bits.SMP = 0;       //Input data sampled at middle of data output time
    SPI1CON1bits.CKE = 1;       //Serial output data changes on transition from
                                //active clock state to idle clock state
    SPI1CON1bits.CKP = 0;       //Idle state for clock is low-level
    SPI1CON1bits.MSTEN = 1;     //Master Mode enabled
    SPI1CON2bits.FRMEN = 0;     //framed support is disabled
    
    //set baud rate to lowest possible (136 kHz) (1.09MHz)
    SPI1CON1bits.SPRE = 0x7;    //Sec. prescale = 1:1
    SPI1CON1bits.PPRE = 0x0;    //Prim. prscale = 64:1
    
    SPI1STATbits.SPIEN = 1;     //enable SPI module
}

#ifdef CS4272_CONTROL_PORT_MODE
void writeCS4272Register(uint8_t map, uint8_t data)
{
    //TODO: write the spiHeader, the map and data payload to the SPI port
    //first include (send) the chip address = 0b0010000
    uint8_t junk;                 //for junk data recieved

	INTCON2bits.GIE = 0;
    IFS0bits.SPI1IF = 0;          //clear interrupt flag for first tx

    CS = 0;
    sleepMicroSecs(1);
    SPI1BUF = 0b00100000;         //Send chip address with R/W* = 0
    while(!IFS0bits.SPI1IF);      //wait until transmission completes
    IFS0bits.SPI1IF = 0;          //clear interrupt flag for next tx
    junk = SPI1BUF;               //recieved data shifted in (junk data)
    
    SPI1BUF = map;                //Send chip address with R/W* = 0
    while(!IFS0bits.SPI1IF);      //wait until transmission completes
    IFS0bits.SPI1IF = 0;          //clear interrupt flag for next tx
    junk = SPI1BUF;

    SPI1BUF = data;               //Send chip address with R/W* = 0
    while(!IFS0bits.SPI1IF);      //wait until transmission completes
    IFS0bits.SPI1IF = 0;          //clear interrupt flag for next tx
    junk = SPI1BUF;
    sleepMicroSecs(1);
    CS = 1;

	INTCON2bits.GIE = 1;
}
#endif

void sleepMicroSecs(uint16_t uSecs)
{
    clock_t start, stop, elapsed;
    int32_t cyclesToSleep = PROCESSOR_CYCLES_PER_US * uSecs - 87;//subtract cycles to compensate for multiplication and funciton call overhead
    
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
