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

#elif _12F675

#define OGun            GP0          // -> 7 -  Output Control Gun for MINI500
#define uBlock          GP1          // -> 6 -  Input from RF Reciver (clear Block)
#define OImpuls         GP2          // -> 5 -  Output Impuls for MINI500
#define Gun             GP3          // -> 4 -  Input Read Gun from TRK
#define Block           GP4          // -> 3 -  Input from RF Reciver (set Block)
#define Impuls          GP5          // -> 2 -  Input Impuls from TRK

#elif _16F628 || _16F628A

#define OGun            PORTBbits.RB0          // ->  6 -  Output Control Gun for MINI500
#define OStart          PORTBbits.RB3          // ->  9 -  Output Start
#define Block           PORTAbits.RA0          // -> 17 -  Input Impuls from TRK
#define uBlock          PORTAbits.RA1          // -> 18 -  Input Read Gun from TRK
#define Impuls          PORTAbits.RA2          // ->  1 -  Input from RF Reciver (clear Block)
#define OImpuls         PORTAbits.RA3          // ->  2 -  Input from RF Reciver (set Block)
#define Gun             PORTAbits.RA4          // ->  3 -  Output Impuls for MINI500

#endif

#define RunTimer0       T0CS = false;    //Clock Select system CLK

void SetupTMR0(void);
void SetupPins(void);
