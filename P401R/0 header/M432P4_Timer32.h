/*��Ϣ
1:   2^32>4.29G
2:   �⺯���У�T32_0������T32_1���Դ�����

*/

#ifndef M432P4_TIMER32_H_
#define M432P4_TIMER32_H_

/*��ʼ��*/

//ʱ�ӷ�Ƶ

uint T32_DIV(uint div_32)
{
	switch (div_32)
	{
		case 1:return TIMER32_PRESCALER_1; break;
		case 16:return TIMER32_PRESCALER_16; break;
		case 256:return TIMER32_PRESCALER_256; break;
		default:return TIMER32_PRESCALER_1; break;
	}
}

//ѡ����BIT

uint T32_BIT(uint BIT_32)
{
	if (BIT_32 == 16)
		return TIMER32_16BIT;
	else
		return TIMER32_32BIT;
}

//ѡ����ģʽ
//F=���ɣ�P=����

uint T32_Mode(char mode_32)
{
	if ((mode_32 == 'F') || (mode_32 == 'f'))
		return TIMER32_FREE_RUN_MODE;
	else
		return TIMER32_PERIODIC_MODE;
}

//���ڹ���ģʽ
//0Ϊ���μ�����1Ϊ��������

bool T32_Period_Mode(uint pm_32)
{
	if (pm_32 == 0)
		return true;
	else
		return false;
}

//T32_0

void T32_0_Init(uint div_32_0, uint BIT_32_0, char mode_32_0, uint period_mode_32_0, uint period_32_0)
{
	Timer32_initModule(TIMER32_0_BASE, T32_DIV(div_32_0), T32_BIT(BIT_32_0), T32_Mode(mode_32_0));
	Timer32_setCount(TIMER32_0_BASE, period_32_0);
	Timer32_enableInterrupt(TIMER32_0_BASE);		//����T32_0-timer�ж�
	Interrupt_enableInterrupt(INT_T32_INT1);		//����T32_0�ж�
	Timer32_startTimer(TIMER32_0_BASE, T32_Period_Mode(period_mode_32_0));
}

//T32_1

void T32_1_Init(uint div_32_1, uint BIT_32_1, char mode_32_1, uint period_mode_32_1,uint period_32_1)
{
	Timer32_initModule(TIMER32_1_BASE, T32_DIV(div_32_1), T32_BIT(BIT_32_1), T32_Mode(mode_32_1));
	Timer32_setCount(TIMER32_1_BASE, period_32_1);
	Timer32_enableInterrupt(TIMER32_1_BASE);		//����T32_1-timer�ж�
	Interrupt_enableInterrupt(INT_T32_INT2);		//����T32_1�ж�
	Timer32_startTimer(TIMER32_1_BASE, T32_Period_Mode(period_mode_32_1));
}


/* ����T32�ж� */

//T32_0
void T32_0_Interrupt_Function();

void T32_INT1_IRQHandler()
{
	T32_0_Interrupt_Function();
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
}

void T32_0_Interrupt_Function()
{
	LED1_Toggle;

}

//T32_1
void T32_1_Interrupt_Function();

void T32_INT2_IRQHandler()
{
	T32_1_Interrupt_Function();
	Timer32_clearInterruptFlag(TIMER32_1_BASE);
}

void T32_1_Interrupt_Function()
{
	LED1_Toggle;

}





#endif /* M432P4_TIMER32_H_ */
