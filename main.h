#include <htc.h>

#define PT2272_M4

#define true 1
#define false 0

#define DirectGun       TRISIObits.TRISIO0
#define WaitForNext 20                         // Wait for next otpusk in Second
#define WidthImp 10                     // x 200 uS - now 2mS
#define PauseImp 10                     // x 200 uS - now 2mS (F = 250Hz -> 150 l/min)
#define InputPin GP5
#define OutputPin GP2
#define InputControl GP4
#define InputControl2 GP1
#define ContrGun GP0
#define RunTimer0 T0CS = 0;             //Clock Select system CLK
#define RunTimer1 TMR1ON = true;

// Stage of error 
#define CountOver 1

void SetupTMR0(void);
void SetupTMR1(void);
void SetupPins(void);
