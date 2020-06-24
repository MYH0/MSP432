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
	GPIO_setAsInputPinWithPullUpResistor(1, BIT1+BIT4);
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
		case 1:delay = 2; break;
		case 2:delay = 4; break;
		case 3:delay = 8; break;
		default:delay = 2; break;
	}

	GPIO_toggleOutputOnPin(1, BIT0);

	Delay_ms(delay);

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

//IO中断*************************************************************************

//P13中断按键功能

//void GPIO_Interrupt_P13_Function()
//{
//	Key_P13();
//}

//IO中断检测(阻塞延时型)
//按键检测是有阻塞的！

//void GPIO_Interrupt_Scan()
//{
//	uint Input = 0;
//	Input = P1IFG & (~P1DIR);		//只检测输入口造成的中断，排除输出口的影响
//	Delay_us(500);
//	if ((P1IN & Input) == 0)		//判断与延时前是否一致
//	{
//		switch (Input)			//需要判断是哪个输入口的中断
//		{
//			case BIT3:GPIO_Interrupt_P13_Function(); break;
//
//			default:break;
//		}
//	}
//}

//进入IO中断
//P1IFG中断标志位要手动清0

//void PORT1_IRQHandler(void)
//{
//	P1_Interrupt_Function();
//	GPIO_clearInterruptFlag(1, P1_status);
//
//}


//按键检测-WDTimer中断(无阻塞),需要后置Watching_Dog文件*********************************
//WDTimer设定在16~20ms左右比较好

//void WDT_Interrupt_Key_Scan()
//{
//	static uchar Key_Now = 0;
//	uchar Key_Past = 0;
//	Key_Past = Key_Now;
//
//	if ((P1IN & BIT3) == 0)
//		Key_Now = 0;
//	else
//		Key_Now = 1;
//
//	if ((Key_Past == 1) && (Key_Now == 0))
//		Key_P13();
//}
//
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
			if(Key_Action==2)
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
