//��ʼ��GPIO*********************************************************************

#define GPIO_LTH GPIO_LOW_TO_HIGH_TRANSITION //������
#define GPIO_HTL GPIO_HIGH_TO_LOW_TRANSITION //�½���

uint delay = 2;


//LED��ʼ��
void GPIO_LED_Init()
{
	GPIO_setAsOutputPin(1, BIT0);
	GPIO_setAsOutputPin(2, BIT0 + BIT1 + BIT2);
	GPIO_setOutputHighOnPin(1, BIT0);
	GPIO_setOutputHighOnPin(2, BIT0);
	GPIO_setOutputLowOnPin(2, BIT1 + BIT2);

}

//Key��ʼ��
void GPIO_Key_Init()
{
	GPIO_setAsInputPinWithPullUpResistor(1, BIT1+BIT4);
	//GPIO_interruptEdgeSelect(1, BIT8_ALL, GPIO_HTL);
	//GPIO_clearInterruptFlag(1, BIT1 + BIT4);
	//GPIO_enableInterrupt(1, BIT1 + BIT4);
	//Interrupt_enableInterrupt(INT_PORT1);
}

//�ܳ�ʼ��
void GPIO_Init()
{
	GPIO_LED_Init();
	GPIO_Key_Init();

}

//P11��������********************************************************************

//�̰�

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

//����

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

//IO�ж�*************************************************************************

//P13�жϰ�������

//void GPIO_Interrupt_P13_Function()
//{
//	Key_P13();
//}

//IO�жϼ��(������ʱ��)
//����������������ģ�

//void GPIO_Interrupt_Scan()
//{
//	uint Input = 0;
//	Input = P1IFG & (~P1DIR);		//ֻ����������ɵ��жϣ��ų�����ڵ�Ӱ��
//	Delay_us(500);
//	if ((P1IN & Input) == 0)		//�ж�����ʱǰ�Ƿ�һ��
//	{
//		switch (Input)			//��Ҫ�ж����ĸ�����ڵ��ж�
//		{
//			case BIT3:GPIO_Interrupt_P13_Function(); break;
//
//			default:break;
//		}
//	}
//}

//����IO�ж�
//P1IFG�жϱ�־λҪ�ֶ���0

//void PORT1_IRQHandler(void)
//{
//	P1_Interrupt_Function();
//	GPIO_clearInterruptFlag(1, P1_status);
//
//}


//�������-WDTimer�ж�(������),��Ҫ����Watching_Dog�ļ�*********************************
//WDTimer�趨��16~20ms���ұȽϺ�

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
//�������-WDTimer�ж�(������)-Melay״̬��**********************************************
//WDTimer�趨��16~20ms���ұȽϺ�

//״̬���궨��
#define FREE 0
#define SHORT 1
#define LONG 2
#define MUST_BE_LONG 60    //����15*60=900ms��,�϶�Ϊ����

volatile uchar WDT_Count = 0;       //�԰���ʱ�����

//�������
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

//״̬��

void WDT_Interrupt_Key_Scan_SM()
{
	static uchar State = 0;
	static uchar Key_Now = 0;
	uchar Key_Past = 0;
	uchar Key_Action = 0;          //�����Ķ�̬�仯��2Ϊ����1Ϊ����
								   
	Key_Past = Key_Now;
	if (P1IN & BIT1)				
		Key_Now = 1;
	else
		Key_Now = 0;

	if ((Key_Past == 1) && (Key_Now == 0))		//��ƽ�ȸߺ�ͣ�Ϊ����
		Key_Action = 1;
	if ((Key_Past == 0) && (Key_Now == 1))		//��ƽ�ȵͺ�ߣ�Ϊ����
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
