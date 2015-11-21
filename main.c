#include "../test3.X/common.h"

uint32_t lastChange=0;

uint32_t timerTest(void *data) {
    PORTToggleBits(IOPORT_B, 65535);
    PORTToggleBits(IOPORT_A, 31);
    return 500;
}
int main(void)
{
    initPic32();
    PORTSetBits(IOPORT_A, BIT_1);
    
    callIn(timerTest,NULL,500);
    
    while(1) {
       /* if(msElapsed-lastChange>500) {
            lastChange = msElapsed;
    PORTToggleBits(IOPORT_B, 65535);
    PORTToggleBits(IOPORT_A, 31);
        }*/
    }
}
