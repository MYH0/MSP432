#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "M432P4_Typedef.h"
#include "M432P4_Clock.h"
#include "M432P4_GPIO.H"
#include "M432P4_WDT.H"

void main(void)
{    
    WDT_WD_STOP();      //นุนท
    Clock_Init(24000000, 32, 'D', 1, 'D', 1, 'R', 1, 'R', 1, 'R', 1);
    GPIO_Init();
    WDT_Timer_Init('S', 15);

    Interrupt_enableMaster();

    while (1);
}

