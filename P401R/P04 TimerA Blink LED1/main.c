#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "M432P4_Typedef.h"
#include "M432P4_Clock.h"
#include "M432P4_GPIO.H"
#include "M432P4_TimerA.H"

void main(void)
{    
    WDT_A_holdTimer();      //นุนท
    Clock_Init();
    GPIO_Init();
    TA_Up_Mode_Init('S', 20, 1, 50000);

    Interrupt_enableMaster();

    while (1);
}

