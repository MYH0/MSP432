#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "M432P4_Typedef.h"
#include "M432P4_Clock.h"
#include "M432P4_GPIO.H"
#include "M432P4_Timer32.h"
#include "M432P4_TimerA.H"
#include "M432P4_WDT.H"

void main(void)
{    
    WDT_WD_STOP();      //�ع�
    Clock_Init(16000000, 32);
    GPIO_Init();
    T32_0_Init(256, 32, 'P', 1, 62500);

    Interrupt_enableMaster();

    while (1);
}

