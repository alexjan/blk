
#include <htc.h>
#include "main.h"

#define _12F1822

#define true 1
#define false 0

    __CONFIG (FCMEN_OFF & IESO_OFF & CLKOUTEN_OFF & BOREN_ON & CPD_OFF & CP_OFF \
    		& MCLRE_OFF & PWRTE_ON & WDTE_ON & FOSC_HS)	
    __CONFIG (LVP_OFF & BORV_HI & STVREN_OFF & PLLEN_ON & WRT_OFF)
    __IDLOC(0000)

void main(void){
       
}
