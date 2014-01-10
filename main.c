#include <htc.h>
#include "main.h"

__IDLOC(309a);

#ifdef _12F629

__CONFIG(FOSC_INTRCIO                                                          \
        & BOREN_ON                                                             \
        & CPD_ON                                                               \
        & CP_ON                                                                \
        & MCLRE_OFF                                                            \
        & PWRTE_ON                                                             \
        & WDTE_ON);

#elif _12F675

__CONFIG(FOSC_INTRCIO                                                          \
        & BOREN_ON                                                             \
        & CPD_ON                                                               \
        & CP_ON                                                                \
        & MCLRE_OFF                                                            \
        & PWRTE_ON                                                             \
        & WDTE_ON);

#elif _16F628

__CONFIG(LVP_OFF                                                               \
        & FOSC_INTOSCIO                                                        \
        & BOREN_ON                                                             \
        & CPD_ON                                                               \
        & CP_ALL                                                               \
        & MCLRE_OFF                                                            \
        & PWRTE_ON                                                             \
        & WDTE_ON);

#elif  _16F628A

__CONFIG(LVP_OFF                                                               \
        & FOSC_INTOSCIO                                                        \
        & BOREN_ON                                                             \
        & CPD_ON                                                               \
        & CP_ON                                                                \
        & MCLRE_OFF                                                            \
        & PWRTE_ON                                                             \
        & WDTE_ON);

#endif

#define PT2272_M4

/********** Varianble defination **********************************************/

bit ModeBlock,                                                                 \
    BlockFlag,                                                                 \
    ResBuf,                                                                    \
    FullBuf,                                                                   \
    BlockGun,                                                                  \
    ModeGun,                                                                   \
    Rise,                                                                      \
    RunInit,                                                                   \
    RDimpuls;

volatile unsigned char cnt = 0,                                                \
                       TimeOutGun = 0,                                         \
                       Count200uS = 0,                                         \
                       Count10mS = 0;

unsigned int Buffer = 0,                                                       \
             count = 0;

/********** End of Block Variable *********************************************/

void main(void) {

    di();

#ifdef _12F629

    OSCCAL = __osccal_val();

#endif

    OPTION_REG = 0b10001111; // WDT(nom)- 18mS x 128 = 2304mS                  \
                                WDT(min) - 7mS x 128 = 896mS                   \
                                WDT(max) - 33mS x 128 = 4224mS

    RunInit = false;

    if (nPOR) {
        if (nBOR) {
            if (!nTO) {
                if (nPD) {
                    RunInit = true; // WDT Reset
                    // Here Set error.....
                } else; // WDT Wake-up "Nothing"
            }
        } else {
            RunInit = true; //Brown-out Reset
        }
    } else if (!nTO || !nPD) {
        RunInit = true; //Power-on Reset
    }

    if (RunInit) {
        //        PCON |= 0b00000011;
        //                        |+---> nBOD
        //                        +----> nPOR
        STATUS |= 0b00011000;
        //             |+----> nPD
        //             +-----> nTO
        SetupPins();
        SetupTMR0();
        RunInit = false;
        PEIE = true;
        RunTimer0;
    }

    ei();

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
                Count10mS = 0;
            }
            CLRWDT();
            Count200uS = 0;
        }

        /************* System timer END ***************************************/

        /************ Control Block out RF reciver*****************************/

#ifdef PT2272_M4

        if (ModeBlock) {
            if (uBlock) {
                ModeBlock = false;
                if (FullBuf) {
                    count = 4544;
                    ModeGun = true;
                    OGun = false;
                    while (count--);
                    BlockGun = true;
                }
            }
        } else if (Block) ModeBlock = true;

#elif PT2272_L4

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
        if (ModeGun) { //???? ???????? ???? (?? ????? ????????? ????, ???? ?????? ??????????), ??
            if (Impuls) {
                if (RDimpuls) {
                    if (ModeBlock) { //???? ???? ????????? ??????? ?????????? ? ????? ?????????? ???????, ??
                        if (ResBuf) {
                            Buffer = 0xFFFF; //???????? ??????, ???????? ???? ????????? ???????
                            ResBuf = false;
                        }
                    } else TimeOutGun = 0;
                    Buffer++; //????????? ?????? ?? ???????
                    FullBuf = true; //?????????? ???? ?????? ?????
                    RDimpuls = false;
                }
            } else RDimpuls = true;
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
                if (!Buffer--) {
                    BlockGun = false;
                    FullBuf = false;
                }
            }
        }

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
