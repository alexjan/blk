#include <htc.h>
#include "main.h"

__IDLOC(308a);

#ifdef _12F629

__CONFIG(FOSC_INTRCIO                                                          \
        & BOREN_ON                                                             \
        & CPD_ON                                                               \
        & CP_ON                                                                \
        & MCLRE_OFF                                                            \
        & PWRTE_ON                                                             \
        & WDTE_ON);

#else #ifdef _16F628

__CONFIG(LVP_OFF                                                               \
        & FOSC_INTOSCIO                                                        \
        & BOREN_ON                                                             \
        & CPD_ON                                                               \
        & CP_ALL                                                               \
        & MCLRE_OFF                                                            \
        & PWRTE_ON                                                             \
        & WDTE_ON);

#endif



#define PT2272_M4

/********** Varianble defination **********************************************/


bit ModeBlock,                                                                 \
    BlockFlag,                                                                 \
    ClearBlockFlag,                                                            \
    ResBuf,                                                                    \
    FullBuf,                                                                   \
    BlockGun,                                                                  \
    ModeGun,                                                                   \
    Rise,                                                                      \
    Pin;

volatile unsigned char cnt = 0,                                                \
                       TimeOut = 0,                                            \
                       TimeOutGun = 0,                                         \
                       Count200uS = 0,                                         \
                       Count10mS = 0;

unsigned int Buffer = 0,                                                       \
             count = 0;

/********** End of Block Variable *********************************************/

void main(void) {

    di();
    OPTION_REG = 0b10001111; // WDT(nom)- 18mS x 128 = 2304mS                  \
                                WDT(min) - 7mS x 128 = 896mS                   \
                                WDT(max) - 33mS x 128 = 4224mS

#ifdef _12F629

    OSCCAL = __osccal_val();

#endif

    if (!nPOR || !nBOD) {
        SLEEP();
    } else {
        if (nTO) {
            if (!nPD) {
                // MCLR Reset during SLEEP
            } else {
                // MCLR Reset
            }
        } else {
            if (!nPD) {
                // WDT Reset during SLEEP
            } else {
                // WDT Reset 
            }
        }
    }

    PCON |= 0b00000011;
    //              |+---> nBOD
    //              +----> nPOR
    STATUS |= 0b00011000;
    //             |+----> nPD
    //             +-----> nTO

    SetupPins();
    SetupTMR0();
    PEIE = true;
    ei();

    RunTimer0;

    while (true) {

        /*************************** System Timer *****************************/

        if (Count200uS > 50) {
            if (Count10mS++ > 100) {
                if (ModeGun && (!FullBuf || ModeBlock)) {
                    if (TimeOutGun++ > 60) {
                        count = 3500;
                        OGun = true;
                        while (count--);
                        TimeOutGun = 0;
                    }
                } else TimeOutGun = 0;
                
                if(FullBuf)TimeOut = 0;
                else if(TimeOut < 3)TimeOut++;
                else BlockGun = false;
                Count10mS = 0;
            }
            CLRWDT();
            Count200uS = 0;
        }

        /************* System timer END ***************************************/

        /************ Control Block out RF reciver*****************************/

#ifdef PT2272_M4

        if (Block && BlockFlag) {
            ModeBlock = true;
            BlockFlag = false;
        } else if (!Block) BlockFlag = true;

        if (uBlock && ClearBlockFlag) {
            ModeBlock = false;
            if (FullBuf) {
                count = 4544;
                ModeGun = true;
                OGun = false;
                while (count--);
                BlockGun = true;
            }
            ClearBlockFlag = false;
        } else if (!uBlock)ClearBlockFlag = true;

#else #ifdef PT2272_L4

        ModeBlock = Block;

#endif

        /************** End block *********************************************/

        /************** Read & Control GUN ************************************/

        if (!BlockGun) {
            ModeGun = !Gun;
            OGun = Gun;
        }

        /**************** End Block *******************************************/

        /********** Read Impuls ***********************************************/
        if (ModeGun) {
            if (Impuls && Pin) {
                if (ResBuf && ModeBlock) {
                    Buffer = 0xFFFF;
                    ResBuf = false;
                }
                Buffer++;
                FullBuf = true;
                if (!ModeBlock) TimeOutGun = 0;
                Pin = false;
            } else if (!Impuls) Pin = true;
        } else ResBuf = true;

        /************ End Block ***********************************************/

        /************ Control Blocking ****************************************/

        if (!ModeBlock && FullBuf && ModeGun) {
            if (Rise) {
                if (cnt > WidthImp) {
                    OImpuls = true;
                    cnt = 0;
                    Rise = false;
                }
            } else if (cnt > PauseImp) {
                Rise = true;
                OImpuls = false;
                cnt = 0;
                //TimeOut = 0;
                if (!Buffer--) FullBuf = false;                
            }
        }
        
        //if(TimeOut > 5 && !FullBuf) BlockGun = false;

        /*********** End Block ************************************************/
    }
}

void interrupt MyInt(void) {
    if (T0IE && T0IF) {
        if (FullBuf) cnt++;
        else cnt = 0;
        Count200uS++;
        TMR0 = 81;
        T0IF = false;
    }
}
