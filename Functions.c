#include <htc.h>
//#include "main.h" 

#define true    1
#define false   0

extern bit BlockFlag,          \
           ClearBlockFlag,     \
           FlGun,              \
           Pin;

void SetupTMR0(void){
    TMR0 = 150;
    T0IE = true;
    T0IF = false;
}

void SetupTMR1(void){
    TMR1IE = true;
    TMR1IF = false;
    T1CON = 0b00000010;
//            |||||||+---- TMR1ON
//            ||||||+----- TMR1CS
//            |||||+------ nT1SYNC
//            ||||+------- T1OSCEN
//            |||+-------- T1CKPS0
//            ||+--------- T1CKPS1
//            |+---------- TMR1GE
//            +----------- ---
}

void InitBitVar(void){
    BlockFlag       = true;
    ClearBlockFlag  = true;
    FlGun           = true;
    Pin             = true;
}

void SetupPins(void){
// Init GPIO as digital I/O

    #ifdef _12F629
    
    GPIO    = 0b11111111;
    TRISIO  = 0b11111010;			// 0 - as output, 1 - as input
    WPU     = 0b11111111;			// 0 - Pull-up disabled, 1 - Pull-up enabled
    IOC     = 0b00000000;			// 0 - int-on-change disable, 1 - int-on-change enable
//                |||||+--> GP0 -> 7 pin
//                ||||+---> GP1 -> 6 pin
//                |||+----> GP2 -> 5 pin
//                ||+-----> GP3 -> 4 pin
//                |+------> GP4 -> 3 pin
//                +-------> GP5 -> 2 pin

    #else #ifdef _16F628

    PORTA   = 0b11101111;
    TRISA   = 0b11101111;
//              |||||||+--> 17 pin
//              ||||||+---> 18 pin
//              |||||+---->  1 pin
//              ||||+----->  2 pin
//              |||+------>  3 pin
//              ||+------->  4 pin
//              |+--------> 15 pin
//              +---------> 16 pin
    
    PORTB   = 0b11110110;
    TRISB   = 0b11110110;
//              |||||||+-->  6 pin
//              ||||||+--->  7 pin
//              |||||+---->  8 pin
//              ||||+----->  9 pin
//              |||+------> 10 pin
//              ||+-------> 11 pin
//              |+--------> 12 pin
//              +---------> 13 pin

    #endif

    CMCON  |= 0b00000111;           // All port config as digit i/o
    
    #ifdef _12F675
    	
    ANSEL |= 0b00001111; 

    #endif
    
    INTE = false;
    INTF = false;
    INTEDG = true;

}
