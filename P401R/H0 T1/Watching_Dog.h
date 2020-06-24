#ifndef WATCHING_DOG_H_
#define WATCHING_DOG_H_
#endif /* WATCHING_DOG_H_ */

//1.�ع�*****************************************************************

void WDT_STOP()
{
    WDT_A_holdTimer();
}

//2.WDTimer*****************************************************************

//2-1.��ʼ��WDTimer
//WDT�жϲ���_EINT();����

void WDTimer_Init()
{
    WDTCTL = WDT_A_CTL_IS_6;
    Interrupt_enableInterrupt(INT_WDT_A);                        //����WDT�ж�
}

//2-2.WDTimer�ж�
//ֻ������Ϊ��ʱ���Ļ�����һ�㶨ʱ��һ��

//2-2-1.WDTimer�жϺ�ʵ�ֵĹ���

void WDT_Interrupt_Function()
{
    WDT_Interrupt_Key_Scan_SM();
}

//2-2-2.����WDTimer�ж�


void WDT_A_IRQHandler(void)
{
    WDT_Interrupt_Function();
}

