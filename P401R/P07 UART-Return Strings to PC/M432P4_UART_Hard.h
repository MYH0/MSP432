/*信息：
1.MSB为先发低位，LSB为先发高位
2.注意所选时钟的频率
3.UART计算器网址：
    http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
  
*/

#ifndef M432P4_UART_HARD_H_
#define M432P4_UART_HARD_H_

void EUSCI_A0_UART_Init()
{
    //GPIO-UART初始化
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
        GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    const eUSCI_UART_ConfigV1 uartConfig =
    {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                     // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
    };
    UART_initModule(EUSCI_A0_BASE, &uartConfig);
    UART_enableModule(EUSCI_A0_BASE);       //使能EUSCI-A0模块
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);    //使能EUSCI-A0-UART中断
    Interrupt_enableInterrupt(INT_EUSCIA0);         //使能EUSCI-A0中断
}

/* EUSCI A0 UART 中断 - 回显输入内容 */

void EUSCI_A0_Interrupt_Function();

void EUSCIA0_IRQHandler(void)
{
    uint status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        EUSCI_A0_Interrupt_Function();
    }
}

void EUSCI_A0_Interrupt_Function()
{
    UART_transmitData(EUSCI_A0_BASE, UART_receiveData(EUSCI_A0_BASE));

}


#endif /* M432P4_UART_HARD_H_ */
