/*
 * M432_GPIO.H
 *
 *  Created on: 2020年6月21日
 *      Author: menyu
 */

#ifndef M432P4_GPIO_H_
#define M432P4_GPIO_H_

/*板上LED快速操作*/
//LED1.0-红,快速操作
#define LED1_Activate GPIO_setAsOutputPin(1, BIT0)
#define LED1_Off GPIO_setOutputLowOnPin(1, BIT0)
#define LED1_On GPIO_setOutputHighOnPin(1, BIT0)
#define LED1_Toggle GPIO_toggleOutputOnPin(1, BIT0)
//LED2.0-红，快速操作
#define LED2_0_Activate GPIO_setAsOutputPin(2, BIT0)
#define LED2_0_Off GPIO_setOutputLowOnPin(2, BIT0)
#define LED2_0_On GPIO_setOutputHighOnPin(2, BIT0)
#define LED2_0_Toggle GPIO_toggleOutputOnPin(2, BIT0)
//LED2.1-绿，快速操作
#define LED2_1_Activate GPIO_setAsOutputPin(2, BIT1)
#define LED2_1_Off GPIO_setOutputLowOnPin(2, BIT1)
#define LED2_1_On GPIO_setOutputHighOnPin(2, BIT1)
#define LED2_1_Toggle GPIO_toggleOutputOnPin(2, BIT1)
//LED2.2-蓝，快速操作
#define LED2_2_Activate GPIO_setAsOutputPin(2, BIT2)
#define LED2_2_Off GPIO_setOutputLowOnPin(2, BIT2)
#define LED2_2_On GPIO_setOutputHighOnPin(2, BIT2)
#define LED2_2_Toggle GPIO_toggleOutputOnPin(2, BIT2)
/*上升沿OR下降沿选择*/
#define GPIO_LTH GPIO_LOW_TO_HIGH_TRANSITION //上升沿
#define GPIO_HTL GPIO_HIGH_TO_LOW_TRANSITION //下降沿
/*Key激活*/
//Key1-P1.1
#define KEY1_Activate GPIO_setAsInputPinWithPullUpResistor(1, BIT1)
//Key2-P1.4
#define KEY2_Activate GPIO_setAsInputPinWithPullUpResistor(1, BIT4)

//LED初始化
void LED_Init()
{
    LED1_Activate;
    LED1_Off;
    //LED2_0_Activate;
    //LED2_0_Off;
    //LED2_1_Activate;
    //LED2_1_Off;
    //LED2_2_Activate;
    //LED2_2_Off;
}

//Key初始化
void Key_Init()
{
    KEY1_Activate;
    KEY2_Activate;
}

//GPIO最终初始化
void GPIO_Init()
{
    LED_Init();
    //Key_Init();
}

void GPIO_Interrupt_Init()
{
    GPIO_interruptEdgeSelect(1, BIT8_ALL, GPIO_HTL);
    GPIO_clearInterruptFlag(1, BIT1);
    GPIO_enableInterrupt(1, BIT1);
    Interrupt_enableInterrupt(INT_PORT1);
}

/* GPIO ISR */

uint P1_status = 0;

void P1_Interrupt_Function();
void P1_Interrupt_Scan();
void Key1_Function();
void Key2_Function();

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

//中断扫描
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
            case BIT4:Key2_Function(); break;
            default:break;
        }
    }

}

//KEY1功能

void Key1_Function()
{
    ;
}

//KEY2功能

void Key2_Function()
{
    ;
}

#endif /* M432P4_GPIO_H_ */
