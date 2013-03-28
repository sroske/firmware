#include "p33exxxx.h"
#include "spintronics.h"
#include "generateAndProcessSamples.h"

/*=============================================================================
i2sInit(): Initialise DCI peripheral for I2S Interface
=============================================================================*/

void i2sInit (void)
{
    /*  NOTE: RFn pins are fixed as digital pins on,
     *  dsPIC33EPXXXMC806; therefore, there is no
     *  ANSELF register
    */

    ANSELE = ANSELE & ~0x00E0;//make RE5, RE6, RE7 digital pins
    ANSELG = ANSELG & ~0x0040;//make RG6 a digital pin

    RPINR25bits.COFSR = 118;//route PIN4/RP118/RG6 to COFS
    RPINR24bits.CSCKR = 87;//route PIN3/RP87/RE7 to CSCK
    RPINR24bits.CSDIR = 86;//route PIN2/RP86/RE6 to CSDI
    RPOR6bits.RP85R = 0b001011;//route CSDO to PIN1/RP85/RE5
    RPOR3bits.RP70R = 0b001011;

    ANSELD = ANSELD & ~0x0040;
/*
In this section we will set up the DCI module for I2S operation to interface 
with a stereo audio codec sampling data at SAMPLE_RATE Hz. The timing diagram is
provided in Fig 1 below:
                                  FIGURE 1
       
					   _______________________________
      |		 	    	          |	                          |
COFS: |___________________________________| 	     	                  |
       _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _    
CSCK:_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
          |<--------Left Channel Data---->|<----Right Channel Data------->|
          |<---------32 bits------------->|<---------32 bits------------->|
          |<---TXBUF0---->|<----TXBUF1--->|<---TXBUF2---->|<----TXBUF3--->|
          |<---RXBUF0---->|<----RXBUF1--->|<---RXBUF2---->|<----RXBUF3--->|
          |<--TimeSlot0-->|<--TimeSlot1-->|<--TimeSlot0-->|<--TimeSlot1-->|
          |<--------------------1/SAMPLE_RATE---------------------------->|
*/

// DCI Control Register DCICON1 Initialization
    DCICON1bits.DCISIDL = 0;	// module operates in CPU Idle Mode
    DCICON1bits.DLOOP = 0;		// Digital Loopback disabled
    DCICON1bits.CSCKD = 1;		// CSCK is an input
    DCICON1bits.CSCKE = 1;		// data sampled on sck rising edge
    DCICON1bits.COFSD = 1;		// COFS is an input
    DCICON1bits.UNFM = 0;		// xmit 0 on underflow
    DCICON1bits.CSDOM = 0;		// CSDO drives 0 during disabled time slots
#if defined(CODEC_USES_I2S)
    DCICON1bits.DJST = 0;		// xmit/rx begins on serial clock after fsync
#else
    DCICON1bits.DJST = 1;		// xmit/rx begins same serial clock as fsync
#endif
    DCICON1bits.COFSM = 1;		// i2s mode

// DCI Control Register DCICON2 Initialization
    DCICON2bits.BLEN = 3;		// 4 words = 64 bits buffered between ineterupts
    DCICON2bits.COFSG = 1;		// data frame has 4 words = 64 bits
    DCICON2bits.WS = 15;		// data word size is 16 bits

// DCI Control Register DCICON3 Initialization
    DCICON3 = 0;	    		//serial clock provided externally

// Transmit Slot Control Register Initialization
    TSCONbits.TSE0 = 1;	    	// Transmit on Time Slot 0
    TSCONbits.TSE1 = 1;	    	// Transmit on Time Slot 1

// Receiver Slot Control Register Initialization
    RSCONbits.RSE0 = 1;	    	// Receive on Time Slot 0
    RSCONbits.RSE1 = 1;	    	// Receive on Time Slot 1

	
// Initialize the TX buffers
    TXBUF0 = 0x0000;
    TXBUF1 = 0x0000;
    TXBUF2 = 0x0000;
    TXBUF3 = 0x0000;

// Enable DCI module
    IPC15bits.DCIIP=6; 			// Enable the interrupts
    IFS3bits.DCIIF=0;
    IEC3bits.DCIIE=1;
    IFS3bits.DCIIF=0;
    DCICON1bits.DCIEN = 1;		//DCI Module Enabled
        
}

/*=============================================================================
DCI Interrupt Handler
=============================================================================*/
void __attribute__((__interrupt__, no_auto_psv)) _DCIInterrupt(void)
{
    IFS3bits.DCIIF = 0;
    spintronicsStateMachine();
}
