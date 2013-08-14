#include <htc.h>

#define true            1
#define false           0


#define WaitForNext     20           // Wait for next otpusk in Second
#define WidthImp        10           // x 200 uS - now 2mS
#define PauseImp        10           // x 200 uS - now 2mS (F = 250Hz -> 150 l/min)

#ifdef  _12F629

#define OGun            GP0          // -> 7 -  Output Control Gun for MINI500
#define uBlock          GP1          // -> 6 -  Input from RF Reciver (clear Block)
#define OImpuls         GP2          // -> 5 -  Output Impuls for MINI500
#define Gun             GP3          // -> 4 -  Input Read Gun from TRK
#define Block           GP4          // -> 3 -  Input from RF Reciver (set Block)
#define Impuls          GP5          // -> 2 -  Input Impuls from TRK

#else #ifdef _16F628

#define OGun            PORTBbits.RB0          // ->  6 -  Output Control Gun for MINI500
#define Start           PORTBbits.RB3          // ->  9 -  Output Start
#define Impuls          PORTAbits.RA0          // -> 17 -  Input Impuls from TRK
#define Gun             PORTAbits.RA1          // -> 18 -  Input Read Gun from TRK
#define uBlock          PORTAbits.RA2          // ->  1 -  Input from RF Reciver (clear Block)
#define Block           PORTAbits.RA3          // ->  2 -  Input from RF Reciver (set Block)
#define OImpuls         PORTAbits.RA4          // ->  3 -  Output Impuls for MINI500

#endif

#define RunTimer0       T0CS = false;    //Clock Select system CLK
#define RunTimer1       TMR1ON = true;

#define RunWriteBuff()  count  = 4544;          \
                        ModeGun = true;         \
                        OGun = false;           \
                        while(count--);         \
                        WriteBufFlag  = true

#define ClearGun()      count  = 3500;          \
                        OGun = true;            \
                        while(count--);         \
                        TimeOutGun = 0
                        
void SetupTMR0(void);
void SetupTMR1(void);
void SetupPins(void);
void InitBitVar(void);
