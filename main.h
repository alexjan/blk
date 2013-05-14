#include <htc.h>


#define true 1
#define false 0

 #define Pause 5                         // Wait for next otpusk in Second
#define WidthImp 20                     // x 200 uS - now 5mS
#define PauseImp 20                     // x 200 uS - now 5mS 
#define InputPin GP2
#define OutputPin GP5
#define InputControl GP3
#define RunTimer0 T0CS = 0;             //Clock Select system CLK
#define RunTimer1 TMR1ON = true;

// Stage of error 
#define CountOver 1

void SetupTMR0(void);
void SetupTMR1(void);
void SetupPins(void);