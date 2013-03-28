#include "p33exxxx.h"
#include "spintronics.h"

void muxInit (void)
{
    //TODO: set ANSELx for the mux pins, set TRISx=0 for the mux pins
    //ANSELD = ANSELD & ~0x0E1F;
    //TRISE = 0xF1E0; // RE pins <11:9> and <4:0> set as outputs

}

void configSensor(uint8_t sensor)
{
    //PORTE=sensor;
}

