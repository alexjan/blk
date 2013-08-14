#include <htc.h>
#include "main.h"

#ifdef _12F629

__CONFIG (FOSC_INTRCIO      \
        & BOREN_ON          \
        & CPD_ON            \
        & CP_ON            \
        & MCLRE_OFF         \
        & PWRTE_ON          \
        & WDTE_ON);

#else #ifdef _16F628

__CONFIG (LVP_OFF           \
        & FOSC_INTOSCIO     \
        & BOREN_ON          \
        & CPD_ON            \
        & CP_ALL            \
        & MCLRE_OFF         \
        & PWRTE_ON          \
        & WDTE_ON);

#endif

__IDLOC(FFFF);

#define PT2272_M4

/********** Varianble defination **********************************************/

                 bit    ModeBlock,                  \
                        BlockFlag,                  \
                        ClearBlockFlag,             \
                        FlGun,                      \
                        FullBuf,                    \
                        ModeGun,                    \
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

/********** End of Block Variable *********************************************/

void main(void){

    di();
    OPTION_REG = 0b10001101;                // WDT - 18mS x 32 = 576mS

    #ifdef _12F629

//   OSCCAL = __osccal_val();

    #endif

    if(!nPOR) nPOR = true;                  // Detect power on reset
    else if(!nBOD) nBOD = true;             // Detect brown out
    else if(!nTO) nTO = true;               // WDT reset enable

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

/*************************** System Timer *************************************/

        if(Count200uS > 50){

            if(Count10mS++ > 100){

//                if(Count1S++ > 120) Count1S = 0;

                if(Gun && (!FullBuf || ModeBlock)) TimeOutGun++;
                else TimeOutGun = 0;

                if(!FullBuf && !WriteBufFlag) TimeOut++;
                else TimeOut = 0;

                Count10mS = 0;
            }
            CLRWDT();
            Count200uS = 0;                 // count 10 mS
        }

/************* System timer END ***********************************************/

/************ Control Block out RF reciver*************************************/

        #ifdef PT2272_M4

        if(Block && BlockFlag){
            ModeBlock = true;
            BlockFlag = false;
        }
        else if (!Block) BlockFlag = true;

        if(uBlock && ClearBlockFlag){
            ModeBlock = false;
            if(FullBuf) RunWriteBuff();
            ClearBlockFlag = false;
        }
        else if (!uBlock)ClearBlockFlag = true;

        #else #ifdef PT2272_L4

            ModeBlock = Block;

        #endif

/************** End block *****************************************************/

/************** Read & Control GUN ********************************************/

        if(!WriteBufFlag){
            ModeGun = ~Gun;
            OGun = Gun;

            #ifdef _16F628

                Start = Gun;

            #endif
        }

         if(!FlGun && !ModeGun) FlGun = true;      // if(!FlGun && TimeOut > WaitForNext) FlGun = true;

/**************** End Block ***************************************************/

/********** Read Impuls *******************************************************/

        if (Impuls && Pin){

            if (FlGun){
                Buffer = 0;
                FlGun = false;
            }

            Buffer++;
            FullBuf = true;
            Pin = false;
        }
        else if (!Impuls) Pin = true;

/************ End Block *******************************************************/

/************ Control Blocking ************************************************/

        if (ModeBlock){
            if(TimeOutGun > 60 && ModeGun) ClearGun();
        }
        else{
            if(FullBuf){
                if (ModeGun){
                    if(Rise){
                        if(cnt > WidthImp){
                            Impuls = true;
                            cnt = 0;
                            Rise = false;
                        }
                    }
                    else if(cnt > PauseImp) {
                        Rise = true;
                        OImpuls = false;
                        cnt = 0;
                        if(!Buffer--) FullBuf = false;
                    }
                }
                else RunWriteBuff();
            }
            
            else if (WriteBufFlag) WriteBufFlag = false;

            else if(TimeOutGun > 60 && ModeGun) ClearGun();
        }
/*********** End Block ********************************************************/
    }
}

void interrupt MyInt (void){

    if(T0IE && T0IF){

        if (FullBuf) cnt++;
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
