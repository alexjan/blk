#include <htc.h>
//#include "main.h" 

#define true    1
#define false   0

void SetupTMR0(void) {
    TMR0 = 150;
    T0IE = true;
    T0IF = false;
}

void SetupPins(void) {
    // Init GPIO as digital I/O
#ifdef _12F675

    ANSEL &= 0b11110000;
    GPIO = 0b11111111;
    TRISIO = 0b11111010; // 0 - as output, 1 - as input
    WPU = 0b11111111; // 0 - Pull-up disabled, 1 - Pull-up enabled
    IOC = 0b00000000; // 0 - int-on-change disable, 1 - int-on-change enable
    //      |||||+--> GP0 -> 7 pin
    //      ||||+---> GP1 -> 6 pin
    //      |||+----> GP2 -> 5 pin
    //      ||+-----> GP3 -> 4 pin
    //      |+------> GP4 -> 3 pin
    //      +-------> GP5 -> 2 pin

#elif _12F629 

    GPIO = 0b11111111;
    TRISIO = 0b11111010; // 0 - as output, 1 - as input
    WPU = 0b11111111; // 0 - Pull-up disabled, 1 - Pull-up enabled
    IOC = 0b00000000; // 0 - int-on-change disable, 1 - int-on-change enable
    //      |||||+--> GP0 -> 7 pin
    //      ||||+---> GP1 -> 6 pin
    //      |||+----> GP2 -> 5 pin
    //      ||+-----> GP3 -> 4 pin
    //      |+------> GP4 -> 3 pin
    //      +-------> GP5 -> 2 pin


#elif _16F628 || _16F628A

    PORTA = 0b11111111;
    TRISA = 0b11110111;
    //        |||||||+--> 17 pin --> Block
    //        ||||||+---> 18 pin --> uBlock
    //        |||||+---->  1 pin --> Impuls
    //        ||||+----->  2 pin --> OImpuls
    //        |||+------>  3 pin --> Gun
    //        ||+------->  4 pin
    //        |+--------> 15 pin
    //        +---------> 16 pin

    PORTB = 0b11111111;
    TRISB = 0b11110110;
    //        |||||||+-->  6 pin --> OGun
    //        ||||||+--->  7 pin
    //        |||||+---->  8 pin
    //        ||||+----->  9 pin --> OStart
    //        |||+------> 10 pin
    //        ||+-------> 11 pin
    //        |+--------> 12 pin
    //        +---------> 13 pin

#endif

    CMCON |= 0b00000111; // All port config as digit i/o

}
