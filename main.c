#define _SUPPRESS_PLIB_WARNING
#include <plib.h>
#include <stdint.h>

#define Ob(x)  ((unsigned)Ob_(0 ## x ## uL))
#define Ob_(x) (x & 1 | x >> 2 & 2 | x >> 4 & 4 | x >> 6 & 8 |		\
	x >> 8 & 16 | x >> 10 & 32 | x >> 12 & 64 | x >> 14 & 128)

#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_1         // PLL Input Divider (1x Divider)
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
#pragma config FWDTEN = ON              // Watchdog Timer Enable (WDT Enabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = ON              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)



typedef struct {
    IoPortId port;
    uint32_t pin;
} Pin;

#define SYS_FREQ         (40000000L)

#define msElapsed ((ReadCoreTimer()/40000))

uint32_t lastChange=0;

int main(void)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //STEP 1. Configure cache, wait states and peripheral bus clock
    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    ANSELA = 0;
    ANSELB = 0;
    REFOCON = 0;
    
    /*INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();

    OpenCoreTimer(2000);
    mConfigIntCoreTimer(CT_INT_ON | CT_INT_PRIOR_5 | CT_INT_SUB_PRIOR_1);
    mEnableIntCoreTimer();*/

    PORTSetPinsDigitalOut(IOPORT_A, 31);
    PORTClearBits(IOPORT_A, 31);
    PORTSetPinsDigitalOut(IOPORT_B, 65535);
    PORTClearBits(IOPORT_B, 65535);
    PORTSetBits(IOPORT_A, BIT_1);
    
    while(1) {
        if(msElapsed-lastChange>2000) {
            PORTToggleBits(IOPORT_B, 65535);
            PORTToggleBits(IOPORT_A, 31);
            lastChange=msElapsed;
        }
    }

}
/*
void __ISR(_CORE_TIMER_VECTOR, IPL5SOFT) CoreTimerInterruptServiceRoutine(void);
void CoreTimerInterruptServiceRoutine() {
    msElapsed++;
    INTClearFlag(INT_CT);
    UpdateCoreTimer(40000);
}*/
