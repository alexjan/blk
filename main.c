#include <htc.h>

#include "main.h"
//#include "Functions.c"

// #define _12F629

__CONFIG (FOSC_INTRCIO & BOREN_ON & CPD_OFF & CP_OFF & MCLRE_OFF & PWRTE_ON & WDTE_ON);	

__IDLOC(FFFF);

volatile unsigned char cnt, TimeOut, TimeOutGun, BlockFlag, ClearBlockFlag, Gun, FlGun, FlGun2;
volatile unsigned char Count200uS,WriteBufFlag, Count10mS, Count1S, Block, Rise, Pin;
unsigned int Buffer,cnt_;

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
	Pin = true;
    BlockFlag = true;
	FlGun = true;
	TimeOutGun = 0;
    WriteBufFlag  = false;
        while (true){
// System Timer    	
    	if(Count200uS > 50){
            if(Count10mS++ > 100){
                if(Count1S++ > 120) Count1S = 0;
                if(Gun) TimeOutGun++;
                else TimeOutGun = 0;
                if(Buffer || WriteBufFlag) TimeOut++;
                Count10mS = 0;
            }
            CLRWDT();
            Count200uS = 0;              // count 10 mS
    	}
//System timer END
        #ifdef PT2272_M4

        if(InputControl == true && BlockFlag){
            Block = true;
            BlockFlag = false;
            FlGun = true;
        }
        else if (InputControl == false) BlockFlag = true;

        if(InputControl2 == true && ClearBlockFlag){
            Block = false;
            ClearBlockFlag = false;
        }
        else if (InputControl2 == false)ClearBlockFlag = true;

        #else #ifdef PT2272_L4

    	Block = InputControl;

        #endif

        if(InputPin == true && Pin){
                                       //Buffer = 10000;
            if(!FlGun2){
                Buffer = 0;            // if(TimeOut > WaitForNext)Buffer = 0;
                FlGun2 = true;
            }
                                       // TimeOut = 0;
            Buffer ++;                 //= 250;
            Pin = false;
        }    
        else if (InputPin == false) Pin = true; 

        if(FlGun2 && !Gun) FlGun2 = false;

        if(WriteBufFlag == false){
            OutGun = ContrGun;
            Gun = ~ContrGun;
        }    
        
        if (Gun && TimeOutGun > 60) {
            TimeOutGun = 0;
            cnt_ = 9000;
            OutGun = true;
            while(cnt--);
            OutGun = false;
        }
        
        if(Block);
        else {
            if(!WriteBufFlag)
            if(Buffer){
                if (Gun){
                    if(Rise){
                        if(cnt > WidthImp){
                            OutputPin = true;
                            cnt = 0;
                            Rise = false;
                        }
                    }
                    else if(cnt > PauseImp) {
                        Rise = true;
                        OutputPin = false;
                        cnt = 0;
                        Buffer--;
                    }
                }
                else {
                    cnt_  = 4544;
                    OutGun = false;
                    Gun = true;
                    WriteBufFlag  = true;
                    while(cnt_--);
                }
            }
            else if (WriteBufFlag)WriteBufFlag = false; 
        }
    }
}

void interrupt MyInt (void){
    if(T0IE && T0IF){
        if (Buffer) cnt++;
        else cnt = 0;
        Count200uS++;
        TMR0 = 81;
        T0IF = false;
    }

    if(TMR1IE && TMR1IF){
        TMR1IF = false;
    }

    if(INTE && INTF){
        INTF = false;
    }   	 
}
