#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "M432P4_Typedef.h"
#include "M432P4_Clock.h"
#include "M432P4_GPIO.H"

void main(void)
{    
    WDT_A_holdTimer();      //นุนท
    Clock_Init();
    GPIO_Init();

    Interrupt_enableMaster();

    while (1)
    {


    }
}

