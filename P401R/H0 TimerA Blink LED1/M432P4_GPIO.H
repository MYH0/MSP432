/*
 * M432_GPIO.H
 *
 *  Created on: 2020��6��21��
 *      Author: menyu
 */

#ifndef M432P4_GPIO_H_
#define M432P4_GPIO_H_

/*����LED���ٲ���*/
//LED1.0-��,���ٲ���
#define LED1_Activate GPIO_setAsOutputPin(1, BIT0)
#define LED1_Off GPIO_setOutputLowOnPin(1, BIT0)
#define LED1_On GPIO_setOutputHighOnPin(1, BIT0)
#define LED1_Toggle GPIO_toggleOutputOnPin(1, BIT0)
//LED2.0-�죬���ٲ���
#define LED2_0_Activate GPIO_setAsOutputPin(2, BIT0)
#define LED2_0_Off GPIO_setOutputLowOnPin(2, BIT0)
#define LED2_0_On GPIO_setOutputHighOnPin(2, BIT0)
#define LED2_0_Toggle GPIO_toggleOutputOnPin(2, BIT0)
//LED2.1-�̣����ٲ���
#define LED2_1_Activate GPIO_setAsOutputPin(2, BIT1)
#define LED2_1_Off GPIO_setOutputLowOnPin(2, BIT1)
#define LED2_1_On GPIO_setOutputHighOnPin(2, BIT1)
#define LED2_1_Toggle GPIO_toggleOutputOnPin(2, BIT1)
//LED2.2-�������ٲ���
#define LED2_2_Activate GPIO_setAsOutputPin(2, BIT2)
#define LED2_2_Off GPIO_setOutputLowOnPin(2, BIT2)
#define LED2_2_On GPIO_setOutputHighOnPin(2, BIT2)
#define LED2_2_Toggle GPIO_toggleOutputOnPin(2, BIT2)
/*������OR�½���ѡ��*/
#define GPIO_LTH GPIO_LOW_TO_HIGH_TRANSITION //������
#define GPIO_HTL GPIO_HIGH_TO_LOW_TRANSITION //�½���
/*Key����*/
//Key1-P1.1
#define KEY1_Activate GPIO_setAsInputPinWithPullUpResistor(1, BIT1)
//Key2-P1.4
#define KEY2_Activate GPIO_setAsInputPinWithPullUpResistor(1, BIT4)

//LED��ʼ��
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

//Key��ʼ��
void Key_Init()
{
    KEY1_Activate;
    KEY2_Activate;
}

//GPIO���ճ�ʼ��
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

//����P1-IO�ж�
void PORT1_IRQHandler(void)
{
    P1_Interrupt_Function();
    GPIO_clearInterruptFlag(1, P1_status);
}

void P1_Interrupt_Function()
{
    P1_Interrupt_Scan();

}

//�ж�ɨ��
void P1_Interrupt_Scan()
{   
    P1_status = GPIO_getEnabledInterruptStatus(1);  
    P1_status &= ~GPIO_getInputPinValue(1,BIT8_ALL);            //ֻ�����������ɵ��ж�
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

//KEY1����

void Key1_Function()
{
    ;
}

//KEY2����

void Key2_Function()
{
    ;
}

#endif /* M432P4_GPIO_H_ */