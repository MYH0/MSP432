#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "M432P4_Typedef.h"
#include "M432P4_Clock.h"
#include "M432P4_GPIO.H"
#include "M432P4_TimerA.H"
#include "M432P4_WDT.H"

void main(void)
{    
    WDT_WD_STOP();      //�ع�
    Clock_Init(16000000, 32);
    GPIO_Init();
    TA_Up_Mode_Init('S', 20, 1, 25000);

    Interrupt_enableMaster();

    while (1);
}

