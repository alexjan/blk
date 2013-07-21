#include <htc.h>

#define PT2272_M4

#define true            1
#define false           0

#define DirectGun       TRISIObits.TRISIO0
#define WaitForNext     20           // Wait for next otpusk in Second
#define WidthImp        10           // x 200 uS - now 2mS
#define PauseImp        10           // x 200 uS - now 2mS (F = 250Hz -> 150 l/min)

#define ContrGun        GP0          // -> 7 -  Output Control Gun for MINI500
#define InputControl2   GP1          // -> 6 -  Input from RF Reciver (clear Block)
#define OutputPin       GP2          // -> 5 -  Output Impuls for MINI500
//#define ContrGun        GP3          // -> 4 -  Input Read Gun from TRK
#define InputControl    GP4          // -> 3 -  Input from RF Reciver (set Block)
#define InputPin        GP5          // -> 2 -  Input Impuls from TRK

#define RunTimer0       T0CS = 0;    //Clock Select system CLK
#define RunTimer1       TMR1ON = true;

void SetupTMR0(void);
void SetupTMR1(void);
void SetupPins(void);
