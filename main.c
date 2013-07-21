#include <htc.h>
#include "main.h"

__CONFIG (FOSC_INTRCIO & BOREN_ON & CPD_OFF & CP_OFF & MCLRE_OFF & PWRTE_ON & WDTE_ON);	

__IDLOC(FFFF);

#define PT2272_M4

/********** Varianble defination *******************************************/

                 bit    Block,                      \
                        BlockFlag,                  \
                        ClearBlockFlag,             \
                        FlGun,                      \
                        Gun,                        \
                        WriteBufFlag,               \
                        Rise,                       \
                        Pin;

volatile unsigned char  cnt             = 0,        \
                        TimeOut         = 0,        \
                        TimeOutGun      = 0,        \
                        Count200uS      = 0,        \
                        Count10mS       = 0,        \
                        Count1S         = 0;

        unsigned int    Buffer          = 0,        \
                        count           = 0;

/************** End of Block Variable***************************************/

void main(void){
    
    di();
    OPTION_REG = 0b10001101;                // WDT - 18mS x 32 = 576mS
    OSCCAL = 0x34;                          //__osccal_val();
    
    if(!nPOR) nPOR = true;                  // Detect power on reset
    else if(!nBOD) nBOD = true;             // Detect brown out
    else if(!nTO);                          // WDT reset enable
   
    CLRWDT();
    SetupPins();
    SetupTMR0();                            // Setup for internal timer
    SetupTMR1();                            //Setup for count input impuls
    PEIE = true;
    ei();

    RunTimer0;
    RunTimer1;

    InitBitVar();

    while (true){

/*************************** System Timer **********************************/

        if(Count200uS > 50){

            if(Count10mS++ > 100){

                if(Count1S++ > 120) Count1S = 0;

                if(Gun) TimeOutGun++;
                else TimeOutGun = 0;

                if(Buffer || WriteBufFlag) TimeOut++;
                else TimeOut = 0;

                Count10mS = 0;
            }
            CLRWDT();
            Count200uS = 0;                 // count 10 mS
        }

/************* System timer END ********************************************/
        
 /*********** Control Block out RF reciver**********************************/
        
        #ifdef PT2272_M4

        if(InputControl && BlockFlag){
            Block = true;
            BlockFlag = false;
        }
        else if (!InputControl) BlockFlag = true;

        if(InputControl2 && ClearBlockFlag){
            Block = false;
            ClearBlockFlag = false;
        }
        else if (!InputControl2)ClearBlockFlag = true;

        #else #ifdef PT2272_L4

            Block = InputControl;

        #endif

/************** End block **************************************************/
        
/************** Read & Control GUN *****************************************/

        if(WriteBufFlag){
            Gun = ~ContrGun;
            OutGun = ContrGun;
        }

        if(!FlGun && !Gun) FlGun = true;

/**************** End Block ************************************************/
        
/********** Read Impuls ****************************************************/
        
        if (InputPin && Pin){

            if (FlGun){
                Buffer = 0;
                FlGun = false;
            }

            Buffer ++;
            Pin = false;
        }    
        else if (!InputPin) Pin = true;
        
/************ End Block ****************************************************/
        
/************ Control Blocking *********************************************/

        if (Block) {

            if (TimeOutGun > 60 && Gun){
                count  = 3500;
                OutGun = true;
                while(count--);
                TimeOutGun = 0;
            }
        }
        else{
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
                    count  = 4544;
                    Gun = true;
                    OutGun = false;
                    while(count--);
                    WriteBufFlag  = true;
                }
            }
            else if (TimeOut > 1){

                if(TimeOutGun > 60 && Gun){
                    count  = 3500;
                    OutGun = true;
                    while(count--);
                    TimeOutGun = 0;
                    FlGun = true;
                }
                
                if (WriteBufFlag) WriteBufFlag = false;
            }
        }
        
/*********** End Block ******************************************************/
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
