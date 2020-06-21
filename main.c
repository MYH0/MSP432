#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Typedef.h"

volatile ulint delay = 5000;
volatile uint i = 24*500;

void main(void)
{
    /* Halting the Watchdog */
    WDT_A_holdTimer();

    /* Configuring P1.0 as output and P1.1 (switch) as input */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring P1.1 as an input and enabling interrupts */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    Interrupt_enableInterrupt(INT_PORT1);

    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();

    while (1)
    {
        volatile uint t = delay;

        GPIO_toggleOutputOnPin(1, BIT0);

        for (; t > 0; t--);
    }
}

/* GPIO ISR */
void PORT1_IRQHandler(void)
{
    uint P1_status;
    volatile static uchar key_mode = 1;
    P1_status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, P1_status);
    P1_status &= ~P1DIR;            //只考虑输入端造成的中断
    for (; i > 0; i--);
    if((P1_status & P1IN)==0)
    {
        switch (P1_status)
        {
            case BIT1: 
                {
                    key_mode++;
                    if (key_mode > 8)
                        key_mode = 1;
                    switch (key_mode)
                    {
                        case 1:delay = 5000; break;
                        case 2:delay = 10000; break;
                        case 3:delay = 50000; break;
                        case 4:delay = 100000; break;
                        case 5:delay = 200000; break;
                        case 6:delay = 300000; break;
                        case 7:delay = 400000; break;
                        case 8:delay = 500000; break;
                        default:delay = 5000; break;
                    }
                }
        }       
    }

}
