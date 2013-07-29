#include <htc.h>

#define true            1
#define false           0

#define WaitForNext     20           // Wait for next otpusk in Second
#define WidthImp        10           // x 200 uS - now 2mS
#define PauseImp        10           // x 200 uS - now 2mS (F = 250Hz -> 150 l/min)

#ifdef  _12F629

#define OutGun          GP0          // -> 7 -  Output Control Gun for MINI500
#define InputControl2   GP1          // -> 6 -  Input from RF Reciver (clear Block)
#define OutputPin       GP2          // -> 5 -  Output Impuls for MINI500
#define ContrGun        GP3          // -> 4 -  Input Read Gun from TRK
#define InputControl    GP4          // -> 3 -  Input from RF Reciver (set Block)
#define InputPin        GP5          // -> 2 -  Input Impuls from TRK

#else #ifdef _16F628

#define OutGun          PORTBbits.RB0          // ->  6 -  Output Control Gun for MINI500
#define InputPin        PORTAbits.RA0          // -> 17 -  Input Impuls from TRK
#define ContrGun        PORTAbits.RA1          // -> 18 -  Input Read Gun from TRK
#define InputControl2   PORTAbits.RA2          // ->  1 -  Input from RF Reciver (clear Block)
#define InputControl    PORTAbits.RA3          // ->  2 -  Input from RF Reciver (set Block)
#define OutputPin       PORTAbits.RA4          // ->  3 -  Output Impuls for MINI500

#endif

#define RunTimer0       T0CS = false;    //Clock Select system CLK
#define RunTimer1       TMR1ON = true;

void SetupTMR0(void);
void SetupTMR1(void);
void SetupPins(void);
void InitBitVar(void);
