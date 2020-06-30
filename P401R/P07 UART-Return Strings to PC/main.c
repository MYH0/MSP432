#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "M432P4_Typedef.h"
#include "M432P4_Clock.h"
#include "M432P4_GPIO.H"
#include "M432P4_Timer32.h"
#include "M432P4_TimerA.H"
#include "M432P4_UART_Hard.h"
#include "M432P4_WDT.H"

void main(void)
{    
    WDT_WD_STOP();      //นุนท
    Clock_Init(12000000, 32);
    EUSCI_A0_UART_Init();

    Interrupt_enableMaster();

    while (1);
}

