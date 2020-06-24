#ifndef WATCHING_DOG_H_
#define WATCHING_DOG_H_
#endif /* WATCHING_DOG_H_ */

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

