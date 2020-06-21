#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Typedef.h"
#include "M432P4_GPIO.H"

void main(void)
{    
    WDT_A_holdTimer();      //นุนท
    GPIO_Init();
    GPIO_Interrupt_Init();
    Interrupt_enableMaster();

    while (1)
    {

    }
}

