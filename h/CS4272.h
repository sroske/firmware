#ifdef CS4272_CONTROL_PORT_MODE
//pre-defined payload bit patterns
#define CS4272_SPI_HEADER 0x20 //CS4272 address = 0b001000, write = 0

//memory address pointers
#define MODE_CONTROL_1 0x1 //memory address pointer for modeControl1
#define DAC_CONTROL 0x2 //memory address pointer for DACControl
#define DAC_VOLUME 0x3 //memory address pointer for DACVolume and Mixing
#define DAC_VOLUME_A 0x4 //memory address pointer for DACVolumeA
#define DAC_VOLUME_B 0x5 //memory address pointer for DACVolumeB
#define ADC_CONTROL 0x6 //memory address pointer for ADC Control
#define MODE_CONTROL_2 0x7 //memory address pointer for ModeControlTwo

#define MODE_CONTROL_ONE_DEFAULT_PAYLOAD 0x28

#define DAC_CONTROL_DEFAULT_PAYLOAD 0x00

#define DAC_VOLUME_DEFAULT_PAYLOAD 0x00//0x30

#define ADC_CONTROL_HPF_DISABLE_B 0x01
#define ADC_CONTROL_HPF_DISABLE_A 0x02
#define ADC_CONTROL_I2S 0x10
#define ADC_CONTROL_DITHER16 0x20

#define MODE_CONTROL_2_PDN 0x01
#define MODE_CONTROL_2_CPEN 0x02
#define MODE_CONTROL_2_LOOP 0x10

extern void enableADCHpf(bool enable);
#endif

extern void cs4272Init(void);

