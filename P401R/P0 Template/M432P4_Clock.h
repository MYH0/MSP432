/*信息：
1.默认下：
	DCO=3MHz
	REFO=32.768kHz
	所有时钟信号不分频
	DCO用作MCLK,HSMCLK,SMCLK
	LFXTCLK用作ACLK,BCLK;若有故障，则换为REFO
	晶振默认为GPIO,LFXT禁用直到引脚配置成LFXT
	HFXT引脚默认为GPIO,HFXT禁止
	DCORSEL=1
2.各时钟源频率：
	内置：
		DCO:1.5M~48MHz,MCLK,HSMCLK,SMCLK
		MODOSC:24MHz,一些外设
		SYSOSC:5MHz,可为ADC提供直接时钟
		REFO:32.768kHz或128kHz，所有时钟都能用
		VLO:9.4kHz,除BCLK均可用，可用于WDT
	外置：
		HFXT:等同于DCO
		LFXT:32.768kHz，所有时钟都能用
3.FPU浮点运算协处理器，加速DCO频率计算（可能涉及到非整数倍运算）
*/
/*注意：
1.BCLK最大不可超过32.768kHz
2.SMCLK最大不超过HSMCLK的一半！
3.！！不知道为什么，代码中设定的频率所对应的周期是实际的10倍，延时运算请注意/10！
4.如果仿真期间出错请修改优化等级来解决，优化等级越高，越容易出幺蛾子
5.MCLK先上到24MHZ为止，48MHZ会有幺蛾子待目前无法解决
6.用自编外置函数设置SMCLK会出莫名其妙的问题
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

uchar CS_REFO_Freq(uint);


/*时钟初始化*/
void Clock_Init(uint DCO_Freq, uint REFO_Freq)
{
	FPU_enableModule();
	CS_setDCOFrequency(DCO_Freq);
	CS_setReferenceOscillatorFrequency(CS_REFO_Freq(REFO_Freq));
	CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_DIV1);
	CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_DIV1);
	CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_DIV16);
	CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_DIV1);
	CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_DIV1);
	mclk_freq = CS_getMCLK() / 10;
}

//REFOCLK设定频率
uchar CS_REFO_Freq(uint REFO_Freq)
{
	switch (REFO_Freq)
	{
		case 32:return CS_REFO_32KHZ; break;
		case 128:return CS_REFO_128KHZ; break;
		default:return CS_REFO_32KHZ; break;
	}
}

/*--延时*/

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
