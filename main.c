#include <htc.h>

#include "main.h"
//#include "Functions.c"

// #define _12F629

__CONFIG (FOSC_INTRCIO & BOREN_ON & CPD_OFF & CP_OFF & MCLRE_OFF & PWRTE_ON & WDTE_ON);	

__IDLOC(FFFF);

unsigned char Error;


void main(void){
    
    di();
	
    OSCCAL = __osccal_val();
    
    if(!nPOR) {
    	nPOR = true;				// Detect power on reset 
    }
    else if(!nBOD){
    	nBOD = true;				// Detect brown out
    }

    else if(!nTO){
    	CLRWDT();
    }						// WDT reset enable
	
	OPTION_REG = 0b10001101;		// WDT - 18mS x 32 = 576mS

	//Init TMR0 for system clock 

	//Init TMR1 for count impuls 
	
	CLRWDT();
	SetupPins();
	SetupTMR0();                    // Setup for internal timer
	SetupTMR1();                    //Setup for count input impuls
	PEIE = true;
	ei();
	RunTimer0;
	RunTimer1;
	while (true){
	    asm("nop");	
		
	}
}

void interrupt MyInt (void){
	if(T0IE && T0IF){
		TMR0 = 150;
		TMR0IF = false;
	}
	if(TMR1IE && TMR1IF){
    	Error = CountOver;
		TMR1IF = false;
	}
}
