#include <htc.h>
#include "main.h"

__IDLOC(305a);

#ifdef _12F629

__CONFIG(FOSC_INTRCIO       \
        & BOREN_ON          \
        & CPD_ON            \
        & CP_ON             \
        & MCLRE_OFF         \
        & PWRTE_ON          \
        & WDTE_ON);

#else #ifdef _16F628

__CONFIG(LVP_OFF            \
        & FOSC_INTOSCIO     \
        & BOREN_ON          \
        & CPD_ON            \
        & CP_ALL            \
        & MCLRE_OFF         \
        & PWRTE_ON          \
        & WDTE_ON);

#endif



#define PT2272_M4

/********** Varianble defination **********************************************/

bit ModeBlock,                                                          \
                        BlockFlag,                                      \
                        ClearBlockFlag,                                 \
                        ResBuf,                                         \
                        FullBuf,                                        \
                        ModeGun,                                        \
                        BlockGun,                                       \
                        Rise,                                           \
                        Pin;

volatile unsigned char cnt = 0,                                         \
                        TimeOutGun = 0,                                 \
                        Count200uS = 0,                                 \
                        Count10mS = 0;

unsigned int Buffer = 0,                                                \
                        count = 0;

/********** End of Block Variable *********************************************/

void main(void) {

    di();
    OPTION_REG = 0b10001101; // WDT - 18mS x 32 = 576mS

#ifdef _12F629

    //   OSCCAL = __osccal_val();

#endif

    if (!nPOR) nPOR = true; // Detect power on reset
    else if (!nBOD) nBOD = true; // Detect brown out
    else if (!nTO) nTO = true; // WDT reset enable

    CLRWDT();
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
                        OStart = true;
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

#ifdef _16F628

            OStart = !Gun;

#endif
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
        } else if (!ResBuf) ResBuf = true;

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
