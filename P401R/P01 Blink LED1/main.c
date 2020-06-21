#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Typedef.h"

int main(void)
{
    volatile uint i;
  
    WDT_A_holdTimer();   // Stop watchdog timer

    GPIO_setAsOutputPin(1, BIT0);
    GPIO_setOutputLowOnPin(1, BIT0);

    while(1)
    {
        GPIO_toggleOutputOnPin(1, BIT0);

        for (i = 100000; i > 0; i--);
    }
}
