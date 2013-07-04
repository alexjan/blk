#include <htc.h>

#define PT2272_M4

#define true 1
#define false 0

#define WaitForNext 20                         // Wait for next otpusk in Second
#define WidthImp 15                     // x 200 uS - now 8mS
#define PauseImp 15                     // x 200 uS - now 8mS (F = 166.67Hz) 
#define InputPin GP5
#define OutputPin GP2
#define InputControl GP4
#define InputControl2 GP1
#define RunTimer0 T0CS = 0;             //Clock Select system CLK
#define RunTimer1 TMR1ON = true;

// Stage of error 
#define CountOver 1

void SetupTMR0(void);
void SetupTMR1(void);
void SetupPins(void);