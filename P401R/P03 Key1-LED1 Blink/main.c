#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Typedef.h"
#include "M432_GPIO.H"


void main(void)
{
    /* Halting the Watchdog */
    WDT_A_holdTimer();

    /* Configuring P1.0 as output and P1.1 (switch) as input */
    GPIO_Init();

    GPIO_Interrupt_Init();

    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();

    while (1)
    {
        volatile uint t = delay;

        GPIO_toggleOutputOnPin(1, BIT0);

        for (; t > 0; t--);
    }
}

