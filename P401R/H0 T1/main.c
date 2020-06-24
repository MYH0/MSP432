#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

typedef long int lint;

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long int ulint;

#define CS_DIV1 CS_CLOCK_DIVIDER_1
#define CS_DIV2 CS_CLOCK_DIVIDER_2
#define CS_DIV4 CS_CLOCK_DIVIDER_4
#define CS_DIV8 CS_CLOCK_DIVIDER_8
#define CS_DIV16 CS_CLOCK_DIVIDER_16
#define CS_DIV32 CS_CLOCK_DIVIDER_32
#define CS_DIV64 CS_CLOCK_DIVIDER_64
#define CS_DIV128 CS_CLOCK_DIVIDER_128

ulint mclk_freq = 0;

/*时钟初始化*/
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

//初始化GPIO*********************************************************************

#define GPIO_LTH GPIO_LOW_TO_HIGH_TRANSITION //上升沿
#define GPIO_HTL GPIO_HIGH_TO_LOW_TRANSITION //下降沿

uint delay = 2;


//LED初始化
void GPIO_LED_Init()
{
	GPIO_setAsOutputPin(1, BIT0);
	GPIO_setAsOutputPin(2, BIT0 + BIT1 + BIT2);
	GPIO_setOutputHighOnPin(1, BIT0);
	GPIO_setOutputHighOnPin(2, BIT0);
	GPIO_setOutputLowOnPin(2, BIT1 + BIT2);

}

//Key初始化
void GPIO_Key_Init()
{
	GPIO_setAsInputPinWithPullUpResistor(1, BIT1 + BIT4);
	//GPIO_interruptEdgeSelect(1, BIT8_ALL, GPIO_HTL);
	//GPIO_clearInterruptFlag(1, BIT1 + BIT4);
	//GPIO_enableInterrupt(1, BIT1 + BIT4);
	//Interrupt_enableInterrupt(INT_PORT1);
}

//总初始化
void GPIO_Init()
{
	GPIO_LED_Init();
	GPIO_Key_Init();

}

//P11按键功能********************************************************************

//短按

void Key_P11_Short()
{

	volatile static uchar key1short_mode = 1;

	key1short_mode++;
	if (key1short_mode > 3)
		key1short_mode = 1;
	switch (key1short_mode)
	{
	case 1:delay = 500; break;
	case 2:delay = 1000; break;
	case 3:delay = 2000; break;
	default:delay = 500; break;
	}

}

//长按

void Key_P11_Long()
{
	volatile static uchar key1long_mode = 1;
	key1long_mode++;
	if (key1long_mode > 3)
		key1long_mode = 1;
	switch (key1long_mode)
	{
	case 1:GPIO_setOutputHighOnPin(2, BIT0); GPIO_setOutputLowOnPin(2, BIT1 + BIT2); break;
	case 2:GPIO_setOutputHighOnPin(2, BIT1); GPIO_setOutputLowOnPin(2, BIT0 + BIT2); break;
	case 3:GPIO_setOutputHighOnPin(2, BIT2); GPIO_setOutputLowOnPin(2, BIT1 + BIT0); break;
	default:key1long_mode = 1; GPIO_setOutputHighOnPin(2, BIT0); GPIO_setOutputLowOnPin(2, BIT1 + BIT2); break;
	}

}

//按键检测-WDTimer中断(无阻塞)-Melay状态机**********************************************
//WDTimer设定在16~20ms左右比较好

//状态机宏定义
#define FREE 0
#define SHORT 1
#define LONG 2
#define MUST_BE_LONG 60    //按下15*60=900ms后,认定为长按

volatile uchar WDT_Count = 0;       //对按下时间计数

//长按检测
uchar Key1_Long_Scan()
{
	WDT_Count++;
	if (WDT_Count == MUST_BE_LONG)
	{
		WDT_Count = 0;
		return 1;
	}
	else
		return 0;
}

//状态机

void WDT_Interrupt_Key_Scan_SM()
{
	static uchar State = 0;
	static uchar Key_Now = 0;
	uchar Key_Past = 0;
	uchar Key_Action = 0;          //按键的动态变化，2为弹起，1为按下

	Key_Past = Key_Now;
	if (P1IN & BIT1)
		Key_Now = 1;
	else
		Key_Now = 0;

	if ((Key_Past == 1) && (Key_Now == 0))		//电平先高后低，为按下
		Key_Action = 1;
	if ((Key_Past == 0) && (Key_Now == 1))		//电平先低后高，为弹起
		Key_Action = 2;

	switch (State)
	{
	case FREE:
	{
		WDT_Count = 0;
		if (Key_Action == 1)
			State = SHORT;
		break;
	}
	case SHORT:
	{
		if (Key_Action == 2)
		{
			State = FREE;
			Key_P11_Short();
		}
		if (Key1_Long_Scan() == 1)
		{
			State = LONG;
			Key_P11_Long();
		}
		break;
	}
	case LONG:
	{
		WDT_Count = 0;
		if (Key_Action == 2)
		{
			State = FREE;
		}
		break;
	}
	default:State = FREE; break;
	}
}

//1.关狗*****************************************************************

void WDT_STOP()
{
	WDT_A_holdTimer();
}

//2.WDTimer*****************************************************************

//2-1.初始化WDTimer
//WDT中断不受_EINT();控制

void WDTimer_Init()
{
	WDTCTL = WDT_A_CTL_IS_6;
	Interrupt_enableInterrupt(INT_WDT_A);                        //允许WDT中断
}

//2-2.WDTimer中断
//只单纯作为定时器的话，与一般定时器一样

//2-2-1.WDTimer中断后实现的功能

void WDT_Interrupt_Function()
{
	WDT_Interrupt_Key_Scan_SM();
}

//2-2-2.进入WDTimer中断


void WDT_A_IRQHandler(void)
{
	WDT_Interrupt_Function();
}



void main() 
{
    WDT_A_holdTimer();	            //关狗

    Clock_Init();

    GPIO_Init();

    WDTimer_Init();

    Interrupt_enableMaster();

    WDT_A_startTimer();

    while (1)
    {
		GPIO_toggleOutputOnPin(1, BIT0);

		Delay_ms(delay);

    }
}

