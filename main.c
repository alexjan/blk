#include <htc.h>

#include "main.h"
//#include "Functions.c"

// #define _12F629

__CONFIG (FOSC_INTRCIO & BOREN_ON & CPD_OFF & CP_OFF & MCLRE_OFF & PWRTE_ON & WDTE_ON);	

__IDLOC(FFFF);

volatile unsigned char Error, cnt, TimeOut,FlgInputPin; 
volatile unsigned char Count200uS,Count10mS, Count1S, Next, Block, Rise, Pin;
unsigned int Buffer;

void main(void){
    
    di();
	
    OSCCAL = 0x34; //__osccal_val();
    
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
	cnt = 0;
	Rise = false;
	Next = true;
	while (true){
// System Timer    	
    	if(Count200uS > 50){
        	if(Count10mS++ > 100){
            	if(Count1S++ > 120){
                	
            	    Count1S = 0;
            	}
            	if(Buffer) TimeOut++;
        	    Count10mS = 0;
        	}
        	CLRWDT();
        	Count200uS = 0;              // count 10 mS
    	}
//System timer END
    	
    	Block = InputControl;
        
        if (InputPin && Next) Next = false; 
        else if (!InputPin){
        	Buffer = 3;        
        	Next = true;
        }
            
        if(Block){    
            
//            Buffer = 3;
        
        }
        else  if(Buffer){
            if(Rise){
                if(cnt > WidthImp){
                    OutputPin = true;                
                   	cnt = 0;
                   	Rise = false; 
               	}               
            }
        	else {
            	if(cnt > PauseImp) {
                	Rise = true;                // For test
                   	OutputPin = false;
                   	cnt = 0;
                   	Buffer--;
            	}
            }   	
        }
    }
}

void interrupt MyInt (void){
	if(T0IE && T0IF){
    	if (Buffer) cnt++;
    	else cnt = 0;
    	Count200uS++;
		TMR0 = 90;
		T0IF = false;
	}
	
	if(TMR1IE && TMR1IF){
    	Error = CountOver;
		TMR1IF = false;
	}
}
