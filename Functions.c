#include <htc.h>
//#include "main.h" 

#define true 1
#define false 0

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

void SetupPins(void){
  // Init GPIO as digital I/O
     
    GPIO    = 0b11111111;			 
    TRISIO  = 0b11111111;			// 0 - as output, 1 - as input
    WPU     = 0b00000000;			// 0 - Pull-up disabled, 1 - Pull-up enabled
    IOC     = 0b00000000;			// 0 - int-on-change disable, 1 - int-on-change enable
    CMCON  |= 0b00000111;           // All port config as digit i/o
    
    #ifdef _12F675
    	
    ANSEL |= 0b00001111; 

    #endif
    
    #ifdef OutputPin == GP2
        TRISIO &= 0b11111011;
    #elif OutputPin == GP4 
        TRISIO &= 0b11101111;
    #elif OutputPin == GP5 
        TRISIO &= 0b11011111;
    #endif
    
}

