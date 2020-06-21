#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Typedef.h"
#include "Clock.h"

int main(void)
{
    volatile ulint i;
  
    WDT_A_holdTimer();   // Stop watchdog timer
    Clock_Init();
    GPIO_setAsOutputPin(1, BIT0);
    GPIO_setOutputLowOnPin(1, BIT0);
    
    while(1)
    {
        GPIO_toggleOutputOnPin(1, BIT0);

        Delay_ms(1000);
    }
}
