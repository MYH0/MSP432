/*
 * M432_GPIO.H
 *
 *  Created on: 2020年6月21日
 *      Author: menyu
 */

#ifndef M432P4_GPIO_H_
#define M432P4_GPIO_H_

#define GPIO_LTH GPIO_LOW_TO_HIGH_TRANSITION //上升沿
#define GPIO_HTL GPIO_HIGH_TO_LOW_TRANSITION //下降沿

volatile ulint delay = 5000;
//volatile uint i = 24 * 500;



void GPIO_Init()
{
    GPIO_setAsOutputPin(1, BIT0);
    GPIO_setOutputLowOnPin(1, BIT0);

}

void GPIO_Interrupt_Init()
{
    GPIO_setAsInputPinWithPullUpResistor(1, BIT1);
    GPIO_interruptEdgeSelect(1,BIT8_ALL,GPIO_HTL);
    GPIO_clearInterruptFlag(1, BIT1);
    GPIO_enableInterrupt(1, BIT1);
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
    GPIO_clearInterruptFlag(1, P1_status);

}

void P1_Interrupt_Function()
{
    P1_Interrupt_Scan();

}

void P1_Interrupt_Scan()
{   
    P1_status = GPIO_getEnabledInterruptStatus(1);  
    P1_status &= ~GPIO_getInputPinValue(1,BIT8_ALL);            //只考虑输入端造成的中断
    //for (; i > 0; i--);
    Delay_ms(24);
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
    ;
}


#endif /* M432P4_GPIO_H_ */
