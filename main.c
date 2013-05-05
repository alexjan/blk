
#include <htc.h>
#include "main.h"

#define _12F629

#define true 1
#define false 0

    __CONFIG (FOSC_INTRCIO & BOREN_ON & CPD_OFF & CP_OFF & MCLRE_OFF & PWRTE_ON & WDTE_ON)	

    __IDLOC(FFFF)

void main(void){
    
    di();
	
    OSCCAL = _READ_OSCCAL_DATA();
    
    if(!nPOW) {
    	nPOW = true;				// Detect power on reset 
    }
    else if(!nBOD){
    	nBOD = true;				// Detect brown out
    }

    else if(!nTO){
    	CLRWDT();
    }						// WDT reset enable
	
	OPTION_REG = 0b10001101;		// WDT - 18mS x 32 = 576mS


    // Init GPIO as digital I/O
     
    GPIO    = 0b11111111;			 
    TRISIO  = 0b11111111;			// 0 - as output, 1 - as input
    WPU     = 0b00000000;			// 0 - Pull-up disabled, 1 - Pull-up enabled
    IOC     = 0b00000000;			// 0 - int-on-change disable, 1 - int-on-change enable
    CMCON  |= 0b00000111;
    
    #ifdef _12F675
    	
    ANSEL |= 0b00001111; 

    #endif
	
	//Init TMR0 for system clock 

	//Init TMR1 for count impuls 
	
	CLRWDT();
	PEIE = true;
	ei();
	while (true){
		
		
	}
}

void interrupt MyInt (void){
	if(T0IE && T0IF){
		
		TMR0IF = false;
	}
	if(TMR1IE && TMR1IF){
		TMR1IF = false;
	}
}
