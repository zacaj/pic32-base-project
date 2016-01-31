#ifndef COMMON_C
#define COMMON_C
#include "common.h"

void waitUs(uint32_t us) {
    uint32_t s = usElapsed;
    while(usElapsed-s < us);
}

#ifndef NO_TIMER
#include "timer.c"
#endif

void initPic32() {
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    ANSELA = 0;
    ANSELB = 0;
    REFOCON = 0;

    PORTSetPinsDigitalOut(IOPORT_A, 31);
    PORTClearBits(IOPORT_A, 31);
    PORTSetPinsDigitalOut(IOPORT_B, 65535);
    PORTClearBits(IOPORT_B, 65535);

#ifndef NO_TIMER
    initTimers();
#endif
    
    
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();

    /*OpenCoreTimer(2000);
    mConfigIntCoreTimer(CT_INT_ON | CT_INT_PRIOR_5 | CT_INT_SUB_PRIOR_1);
    mEnableIntCoreTimer();*/
}

void setOut(Pin pin, uint8_t value) {
    if(value)
        PORTSetBits(pin.port, pin.pin);
    else
        PORTClearBits(pin.port, pin.pin);
}

void initIn(Pin pin) {
    PORTSetPinsDigitalIn(pin.port, pin.pin);
}

void initOut(Pin pin, uint8_t initialValue) {
    PORTSetPinsDigitalOut(pin.port, pin.pin);
    setOut(pin, initialValue);
}



/*
void __ISR(_CORE_TIMER_VECTOR, IPL5SOFT) CoreTimerInterruptServiceRoutine(void);
void CoreTimerInterruptServiceRoutine() {
    msElapsed++;
    INTClearFlag(INT_CT);
    UpdateCoreTimer(40000);
}*/
uint32_t excep_code; 
uint32_t excep_addr; 

void _general_exception_handler (unsigned cause, unsigned status) 
{ 
   excep_code = (cause & 0x0000007C) >> 2; 
   excep_addr = __builtin_mfc0(_CP0_EPC, _CP0_EPC_SELECT); 
   if ((cause & 0x80000000) != 0) 
      excep_addr += 4;  
   
   while (1); 
}  


#endif