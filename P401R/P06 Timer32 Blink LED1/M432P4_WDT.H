/*信息
1.时钟源
	SMCLK(默认)
	ACLK
	VLO
	BCLK
2.定时长度：
				CODE:WDT_A_CLOCKITERATIONS_?
	2^   Num		  Code   --------------Time------------------------------         
							  32.768K		  128K			   9.4K		  
	6	 64			  64	 1.95ms          0.5ms			  6.8ms 
	9	 512		  512	 15.625ms		 4ms			  54.47ms
	13	 8192		  8192	 250ms			 64ms			  871.49ms
	15	 32768		  32K    1s				 256ms			  3.486s
	19	 524288		  512K   16s			 4.096s			  55.775s
	23	 8388608      8192K  4min-16s		 1min-5.5s		  14min-52.4s
	27	 134217728    128M   1h-8min-16s	 17min-28.5s	  3h-57min58.48s
	31	 2147483648   2G     18h-12min-16s   4h-39min-37.2s   63h-27min-35.7s

*/
/*注意

*/

#ifndef M432P4_WDT_H_
#define M432P4_WDT_H_

/*关狗*/

void WDT_WD_STOP()
{
	WDT_A_holdTimer();
}

/*WDT定时器*/

uchar WDT_CLK_Choose(char);
uchar WDT_Timelong(uint);


//最终初始化

/*例子
1.SMCLK-32768
	WDT_Timer_Init('S', 15);

*/

void WDT_Timer_Init(char WDT_CLK, uint WDT_Time)
{
	WDT_A_initIntervalTimer(WDT_CLK_Choose(WDT_CLK), WDT_Timelong(WDT_Time));	//设定调用时钟和时长
	Interrupt_enableInterrupt(INT_WDT_A);
	WDT_A_startTimer();
}

//WDT时钟选择
uchar WDT_CLK_Choose(char WDT_CLK)
{
	switch (WDT_CLK)
	{
		case 'S':
		case 's':return WDT_A_CLOCKSOURCE_SMCLK; break;
		case 'V':
		case 'v':return WDT_A_CLOCKSOURCE_VLOCLK; break;
		case 'A':
		case 'a':return WDT_A_CLOCKSOURCE_ACLK; break;
		case 'B':
		case 'b':return WDT_A_CLOCKSOURCE_BCLK; break;
		default:return WDT_A_CLOCKSOURCE_SMCLK; break;
	}
}

//定时时长(参数为2的乘方:2^?)
uchar WDT_Timelong(uint WDT_Time)
{
	switch (WDT_Time)
	{
		case 6:return WDT_A_CLOCKITERATIONS_64; break;
		case 9:return WDT_A_CLOCKITERATIONS_512; break;
		case 13:return WDT_A_CLOCKITERATIONS_8192; break;
		case 15:return WDT_A_CLOCKITERATIONS_32K; break;
		case 19:return WDT_A_CLOCKITERATIONS_512K; break;
		case 23:return WDT_A_CLOCKITERATIONS_8192K; break;
		case 27:return WDT_A_CLOCKITERATIONS_128M; break;
		case 31:return WDT_A_CLOCKITERATIONS_2G; break;
		default:return WDT_A_CLOCKITERATIONS_64; break;
	}
}

/*进入中断*/

void WDT_Timer_Interrupt_Function();


void WDT_A_IRQHandler(void)
{
	WDT_Timer_Interrupt_Function();
}

void WDT_Timer_Interrupt_Function()
{
	LED1_Toggle;

}


#endif /* M432P4_WDT_H_ */
