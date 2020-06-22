/*��Ϣ��
1.Ĭ���£�
	DCO=3MHz
	REFO=32.768kHz
	����ʱ���źŲ���Ƶ
	DCO����MCLK,HSMCLK,SMCLK
	LFXTCLK����ACLK,BCLK;���й��ϣ���ΪREFO
	����Ĭ��ΪGPIO,LFXT����ֱ���������ó�LFXT
	HFXT����Ĭ��ΪGPIO,HFXT��ֹ
	DCORSEL=1
2.��ʱ��ԴƵ�ʣ�
	���ã�
		DCO:1M~48MHz,MCLK,HSMCLK,SMCLK
		MODOSC:24MHz,һЩ����
		SYSOSC:5MHz,��ΪADC�ṩֱ��ʱ��
		REFO:32.768kHz��128kHz������ʱ�Ӷ�����
		VLO:9.4kHz,��BCLK�����ã�������WDT
	���ã�
		HFXT:��ͬ��DCO
		LFXT:32.768kHz������ʱ�Ӷ�����
3.FPU��������Э������������DCOƵ�ʼ��㣨�����漰�������������㣩
*/
/*ע�⣺
1.BCLK��󲻿ɳ���32.768kHz
2.SMCLK��󲻳���HSMCLK��һ�룡
3.������֪��Ϊʲô���������趨��Ƶ������Ӧ��������ʵ�ʵ�10������ʱ������ע��/10��
4.��������ڼ�������޸��Ż��ȼ���������Ż��ȼ�Խ�ߣ�Խ���׳��۶���
5.MCLK���ϵ�24MHZΪֹ��48MHZ�����۶��Ӵ�Ŀǰ�޷����
*/

#ifndef M432P4_CLOCK_H_
#define M432P4_CLOCK_H_

#define CS_DIV1 CS_CLOCK_DIVIDER_1
#define CS_DIV2 CS_CLOCK_DIVIDER_2
#define CS_DIV4 CS_CLOCK_DIVIDER_4
#define CS_DIV8 CS_CLOCK_DIVIDER_8
#define CS_DIV16 CS_CLOCK_DIVIDER_16
#define CS_DIV32 CS_CLOCK_DIVIDER_32
#define CS_DIV64 CS_CLOCK_DIVIDER_64
#define CS_DIV128 CS_CLOCK_DIVIDER_128

ulint mclk_freq = 0;

/*ʱ�ӳ�ʼ��*/
void Clock_Init()
{
	FPU_enableModule();
	CS_setDCOFrequency(CS_24MHZ);
	CS_setReferenceOscillatorFrequency(CS_REFO_32KHZ);
	CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_DIV1);
	CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_DIV1);
	CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_DIV2);
	CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_DIV1);
	CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_DIV1);
	mclk_freq = CS_getMCLK() / 10;
}

/*++--��ʱ*/

void Delay_ms(uint timelong_ms)
{
	volatile uint t = timelong_ms;
	volatile ulint m = mclk_freq / 1000;
	for (; t > 0; t--)
	{
		for (; m > 0; m--);
		m = mclk_freq / 1000;
	}
		
}

void Delay_us(uint timelong_us)
{
	volatile uint t = timelong_us;
	volatile ulint m = mclk_freq / 1000000;
	for (; t > 0; t--)
	{
		for (; m > 0; m--);
		m = mclk_freq / 1000000;
	}
		
}


#endif /* M432P4_CLOCK_H_ */
