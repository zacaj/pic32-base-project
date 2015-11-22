#define _SUPPRESS_PLIB_WARNING
//#include <peripheral/adc10.h>
//#include <peripheral/bmx.h>
//#include <peripheral/cmp.h>
//#include <peripheral/cvref.h>
//#include <peripheral/dma.h>
//#include <peripheral/i2c.h>
//#include <peripheral/incap.h>
#include <peripheral/int.h>
//#include <peripheral/nvm.h>
//#include <peripheral/outcompare.h>
//#include <peripheral/pcache.h>
//#include <peripheral/pmp.h>
#include <peripheral/ports.h>
#include <peripheral/pps.h>
//#include <peripheral/power.h>
//#include <peripheral/reset.h>
//#include <peripheral/rtcc.h>
#include <peripheral/spi.h>
#include <peripheral/system.h>
#include <peripheral/timer.h>
//#include <peripheral/uart.h>
//#include <peripheral/wdt.h>
//#include <peripheral/eth.h>
//#include <peripheral/CAN.h>
#include <stdint.h>

#define Ob(x)  ((unsigned)Ob_(0 ## x ## uL))
#define Ob_(x) (x & 1 | x >> 2 & 2 | x >> 4 & 4 | x >> 6 & 8 |		\
	x >> 8 & 16 | x >> 10 & 32 | x >> 12 & 64 | x >> 14 & 128)

#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (1x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF              // Watchdog Timer Enable (WDT Enabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = ON              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#define bA IOPORT_A
#define bB IOPORT_B
#define p15                       (1 << 15)
#define p14                       (1 << 14)
#define p13                       (1 << 13)
#define p12                       (1 << 12)
#define p11                       (1 << 11)
#define p10                       (1 << 10)
#define p9                        (1 << 9)
#define p8                        (1 << 8)
#define p7                        (1 << 7)
#define p6                        (1 << 6)
#define p5                        (1 << 5)
#define p4                        (1 << 4)
#define p3                        (1 << 3)
#define p2                        (1 << 2)
#define p1                        (1 << 1)
#define p0                        (1 << 0)




typedef struct {
    IoPortId port;
    uint32_t pin;
} Pin;

#define SYS_FREQ         (40000000L)

#define msElapsed ((ReadCoreTimer()/20000))
#define dmsElapsed ((ReadCoreTimer()/2000))
#define usElapsed ((ReadCoreTimer()/20))

#include "timer.c"

void initPic32() {
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    ANSELA = 0;
    ANSELB = 0;
    REFOCON = 0;

    PORTSetPinsDigitalOut(IOPORT_A, 31);
    PORTClearBits(IOPORT_A, 31);
    PORTSetPinsDigitalOut(IOPORT_B, 65535);
    PORTClearBits(IOPORT_B, 65535);
    
    initTimers();
    
    
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


