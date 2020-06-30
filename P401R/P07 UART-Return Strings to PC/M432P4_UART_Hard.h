/*��Ϣ��
1.MSBΪ�ȷ���λ��LSBΪ�ȷ���λ
2.ע����ѡʱ�ӵ�Ƶ��
3.UART��������ַ��
    http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
  
*/

#ifndef M432P4_UART_HARD_H_
#define M432P4_UART_HARD_H_

void EUSCI_A0_UART_Init()
{
    //GPIO-UART��ʼ��
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
    UART_enableModule(EUSCI_A0_BASE);       //ʹ��EUSCI-A0ģ��
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);    //ʹ��EUSCI-A0-UART�ж�
    Interrupt_enableInterrupt(INT_EUSCIA0);         //ʹ��EUSCI-A0�ж�
}

/* EUSCI A0 UART �ж� - ������������ */

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
