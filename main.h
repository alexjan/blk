#include <htc.h>


#define true 1
#define false 0


#define InputPin GP2
#define OutputPin GP5
#define InputControl GP3
#define RunTimer0 T0CS = 0;             //Clock Select system CLK
#define RunTimer1 TMR1ON = true;

// Stage of error changes
#define CountOver 1

void SetupTMR0(void);
void SetupTMR1(void);
void SetupPins(void);