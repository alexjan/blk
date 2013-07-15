#include <htc.h>

#include "main.h"
//#include "Functions.c"

// #define _12F629

__CONFIG (FOSC_INTRCIO & BOREN_ON & CPD_OFF & CP_OFF & MCLRE_OFF & PWRTE_ON & WDTE_ON);	

__IDLOC(FFFF);

volatile unsigned char cnt, TimeOut,BlockFlag, ClearBlockFlag, FlagGun, Gun;
volatile unsigned char Count200uS,Count10mS, Count1S, Block, Rise, Pin;
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
	while (true){
// System Timer    	
    	if(Count200uS > 50){
            if(Count10mS++ > 100){
                if(Count1S++ > 120) Count1S = 0;
                if(Buffer) TimeOut++;
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
        }
        else if (InputControl == false) BlockFlag = true;

        if(InputControl2 == true && ClearBlockFlag){
            Block = false;
            ClearBlockFlag = false;
        }
        else if (InputControl2 = false)ClearBlockFlag = true;

        #elif PT2272_L4

    	Block = InputControl;

        #endif

        if (TRISIObits.TRISIO0 == true) Gun = ~ContrGun;

        if (Gun == false && FlagGun == false) FlagGun = true;

        if(InputPin == true && Pin){
            //Buffer = 10000;
            if(TimeOut > WaitForNext || FlagGun)Buffer = 0;
            FlagGun = false;
            TimeOut = 0;
            Buffer ++;//= 250;
            Pin = false;
        }    
        else if (InputPin == false) Pin = true; 
         
        if(Block) TRISIObits.TRISIO0 = true;
        else {
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
                    TRISIObits.TRISIO0  = false;
                    asm("nop");
                    ContrGun = false;
                    Gun = true;
                    while(cnt_--);
                }
            }
            else if(Gun) {
                ContrGun = true;
                TRISIObits.TRISIO0 = true;
            }
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
