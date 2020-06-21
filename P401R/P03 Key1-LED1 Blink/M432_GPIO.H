/*
 * M432_GPIO.H
 *
 *  Created on: 2020年6月21日
 *      Author: menyu
 */

#ifndef M432_GPIO_H_
#define M432_GPIO_H_

volatile ulint delay = 5000;
volatile uint i = 24 * 500;



void GPIO_Init()
{
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

}

void GPIO_Interrupt_Init()
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    Interrupt_enableInterrupt(INT_PORT1);

}

/* GPIO ISR */

uint P1_status = 0;

void P1_Interrupt_Function();
void P1_Interrupt_Scan();
void Key1_Function();

//进入P1-IO中断
void PORT1_IRQHandler(void)
{
    P1_Interrupt_Function();
    GPIO_clearInterruptFlag(GPIO_PORT_P1, P1_status);
}

void P1_Interrupt_Function()
{
    P1_Interrupt_Scan();

}

void P1_Interrupt_Scan()
{   
    P1_status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    P1_status &= ~P1DIR;            //只考虑输入端造成的中断
    for (; i > 0; i--);
    if ((P1_status & P1IN) == 0)
    {
        switch (P1_status)
        {
            case BIT1:Key1_Function(); break;
            default:break;
        }
    }

}

void Key1_Function()
{
    volatile static uchar key1_mode = 1;

    key1_mode++;
    if (key1_mode > 8)
        key1_mode = 1;
    switch (key1_mode)
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


#endif /* M432_GPIO_H_ */
