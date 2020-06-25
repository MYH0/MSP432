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
5.MCLK���ϵ�24MHZΪֹ��48MHZ�����۶���Ŀǰ�޷����
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

uint CS_Choose(char);
uint CS_DIV(uint);
uint CS_REFO_Freq(uint);

//���ճ�ʼ��

/*����
1.DCO=24M,REFO=32.768K,DCO_1-DCO_1-REFO_1-REFO_1-REFO_1
	Clock_Init(24000000, 32, 'D', 1, 'D', 1, 'R', 1, 'R', 1, 'R', 1);
2.DCO=24M,REFO=32.768K,DCO_1-DCO_1-DCO_2-REFO_1-REFO_1
	Clock_Init(24000000, 32, 'D', 1, 'D', 1, 'D', 2, 'R', 1, 'R', 1);

*/

void Clock_Init(uint DCO_Freq, uint REFO_Freq,
	char MCLK_Choose, uint MCLK_DIV, char HSMCLK_Choose, uint HSMCLK_DIV,
	char SMCLK_Choose, uint SMCLK_DIV, char ACLK_Choose, uint ACLK_DIV,
	char BCLK_Choose, uint BCLK_DIV)
{
	FPU_enableModule();
	CS_setDCOFrequency(DCO_Freq);
	CS_setReferenceOscillatorFrequency(CS_REFO_Freq(REFO_Freq));
	CS_initClockSignal(CS_MCLK, CS_Choose(MCLK_Choose), CS_DIV(MCLK_DIV));
	CS_initClockSignal(CS_HSMCLK, CS_Choose(HSMCLK_Choose), CS_DIV(HSMCLK_DIV));
	CS_initClockSignal(CS_SMCLK, CS_Choose(SMCLK_Choose), CS_DIV(SMCLK_DIV));
	CS_initClockSignal(CS_ACLK, CS_Choose(ACLK_Choose), CS_DIV(ACLK_DIV));
	CS_initClockSignal(CS_BCLK, CS_Choose(BCLK_Choose), CS_DIV(BCLK_DIV));
	mclk_freq = CS_getMCLK() / 10;
}

//ѡʱ��Դ
uint CS_Choose(char CS_CLK)
{
	switch (CS_CLK)
	{
		case 'D':
		case 'd':return CS_DCOCLK_SELECT; break;
		case 'R':
		case 'r':return CS_REFOCLK_SELECT; break;
		case 'V':
		case 'v':return CS_VLOCLK_SELECT; break;
		case 'M':
		case 'm':return CS_MODOSC_SELECT; break;
		case 'H':
		case 'h':return CS_HFXTCLK_SELECT; break;
		case 'L':
		case 'l':return CS_LFXTCLK_SELECT; break;
		default:return CS_DCOCLK_SELECT; break;
	}
}

//ʱ��Դ��Ƶ
uint CS_DIV(uint CLK_DIV)
{
	switch (CLK_DIV)
	{
		case 1:CS_DIV1; break;
		case 2:CS_DIV2; break;
		case 4:CS_DIV4; break;
		case 8:CS_DIV8; break;
		case 16:CS_DIV16; break;
		case 32:CS_DIV32; break;
		case 64:CS_DIV64; break;
		case 128:CS_DIV128; break;
		default:CS_DIV1; break;
	}
}

//REFOCLK�趨Ƶ��
uint CS_REFO_Freq(uint REFO_Freq)
{
	switch (REFO_Freq)
	{
		case 32:return CS_REFO_32KHZ; break;
		case 128:return CS_REFO_128KHZ; break;
		default:return CS_REFO_32KHZ; break;
	}
}

/*--��ʱ*/

//ms
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

//us
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
