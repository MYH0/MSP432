#ifndef M432P4_TIMERA_H_
#define M432P4_TIMERA_H_

/*TA-基本参数初始化*/

uint TA_CLK_Choose(char TA_CLK)
{
    switch (TA_CLK)
    {
        case 'A':
        case 'a':return TIMER_A_CLOCKSOURCE_ACLK; break;
        case 'S':
        case 's':return TIMER_A_CLOCKSOURCE_SMCLK; break;
        case 'T':
        case 't':return TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK; break;      //TACLK
        case 'I':
        case 'i':return TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK; break;     //INCLK
        default:return TIMER_A_CLOCKSOURCE_SMCLK; break;
    }
}

uint TA_CLK_DIV(uint TA_DIV)
{
    switch (TA_DIV)
    {
        case 1:return TIMER_A_CLOCKSOURCE_DIVIDER_1; break;
        case 2:return TIMER_A_CLOCKSOURCE_DIVIDER_2; break;
        case 3:return TIMER_A_CLOCKSOURCE_DIVIDER_3; break;
        case 4:return TIMER_A_CLOCKSOURCE_DIVIDER_4; break;
        case 5:return TIMER_A_CLOCKSOURCE_DIVIDER_5; break;
        case 6:return TIMER_A_CLOCKSOURCE_DIVIDER_6; break;
        case 7:return TIMER_A_CLOCKSOURCE_DIVIDER_7; break;
        case 8:return TIMER_A_CLOCKSOURCE_DIVIDER_8; break;
        case 10:return TIMER_A_CLOCKSOURCE_DIVIDER_10; break;
        case 12:return TIMER_A_CLOCKSOURCE_DIVIDER_12; break;
        case 14:return TIMER_A_CLOCKSOURCE_DIVIDER_14; break;
        case 16:return TIMER_A_CLOCKSOURCE_DIVIDER_16; break;
        case 20:return TIMER_A_CLOCKSOURCE_DIVIDER_20; break;
        case 24:return TIMER_A_CLOCKSOURCE_DIVIDER_24; break;
        case 28:return TIMER_A_CLOCKSOURCE_DIVIDER_28; break;
        case 32:return TIMER_A_CLOCKSOURCE_DIVIDER_32; break;
        case 40:return TIMER_A_CLOCKSOURCE_DIVIDER_40; break;
        case 48:return TIMER_A_CLOCKSOURCE_DIVIDER_48; break;
        case 56:return TIMER_A_CLOCKSOURCE_DIVIDER_56; break;
        case 64:return TIMER_A_CLOCKSOURCE_DIVIDER_64; break;
        default:return TIMER_A_CLOCKSOURCE_DIVIDER_1; break;
    }
}

uint TA_Choose(uint TA_Num)
{
    switch (TA_Num)
    {
        case 0:return TIMER_A0_BASE; break;
        case 1:return TIMER_A1_BASE; break;
        case 2:return TIMER_A2_BASE; break;
        case 3:return TIMER_A3_BASE; break;
        default:return TIMER_A0_BASE; break;
    }
}

uint TA_INT_0_Num(uint TA_Num)
{
    switch (TA_Num)
    {
        case 0:return INT_TA0_0; break;
        case 1:return INT_TA1_0; break;
        case 2:return INT_TA2_0; break;
        case 3:return INT_TA3_0; break;
        default:return INT_TA0_0; break;
    }
}

/*TA-工作模式设定*/

//增计数模式
/*例子
1.SMCLK-20分频-A1定时器-计数50000
    TA_Up_Mode_Init('S', 20, 1, 50000);

*/
void TA_Up_Mode_Init(char TA_CLK, uint TA_DIV,uint TA_Num,uint TA_Period)
{
    const Timer_A_UpModeConfig TA_upConfig =
    {
        TA_CLK_Choose(TA_CLK),                  // TA时钟调用
        TA_CLK_DIV(TA_DIV),                     // TA时钟分频
        TA_Period,                              // 计时长度
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // 禁用计数器溢出中断
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // 使能TACCR0中断
        TIMER_A_DO_CLEAR                        // Clear value
    };
    Timer_A_configureUpMode(TA_Choose(TA_Num), &TA_upConfig);
    Interrupt_enableInterrupt(TA_INT_0_Num(TA_Num));
    Timer_A_startCounter(TA_Choose(TA_Num), TIMER_A_UP_MODE);

}


/*TA-进入中断*/

//TA1-进入中断

void TA1_Interrupt_Function();
void TA1_Interrupt_Clear();

void TA1_0_IRQHandler()
{
    TA1_Interrupt_Function();
    TA1_Interrupt_Clear();
}

void TA1_Interrupt_Function()
{
    LED1_Toggle;

}

void TA1_Interrupt_Clear()
{
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}


#endif /* M432P4_TIMERA_H_ */
