extern void spiInit(void);
#ifdef CS4272_CONTROL_PORT_MODE
extern void writeCS4272Register(uint8_t map, uint8_t data);
#endif
