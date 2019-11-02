#include "stm32f0xx_hal.h"
#include "uart.h"

UART_HandleTypeDef UARTHandle;
uint8_t UART_rxData;

void UART_init(uint32_t b_rate)
{
    UART_gpio_init();
    __USART2_CLK_ENABLE();
    UARTHandle.Instance = USART2;
    UARTHandle.Init.BaudRate = b_rate;
    UARTHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UARTHandle.Init.StopBits = UART_STOPBITS_1;
    UARTHandle.Init.Parity = UART_PARITY_NONE;
    UARTHandle.Init.Mode = UART_MODE_TX_RX;
    UARTHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UARTHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&UARTHandle);
    __HAL_UART_ENABLE(&UARTHandle);
       
}
 
void UART_IRQ_Enable()
{
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    NVIC_EnableIRQ(USART2_IRQn);
    __HAL_UART_ENABLE_IT(&UARTHandle, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&UARTHandle, UART_IT_TC);
    UART_Rx_Int_Enable();

}

void UART_gpio_init()
{

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitTypeDef GPIO_InitStruct;
    __GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_2 ;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
uint32_t UART_Tx_packet(uint8_t *aTxBuffer , uint16_t length , uint32_t timeout)
{
    uint32_t status;
    status = HAL_UART_Transmit(&UARTHandle, (uint8_t*)aTxBuffer, length, timeout);
    while(__HAL_UART_GET_FLAG(&UARTHandle, UART_FLAG_TXE) !=1 );
    return status;
}

uint32_t UART_Rx_packet(uint8_t *aRxBuffer , uint16_t length , uint32_t timeout)
{
    uint32_t status;
    //while(__HAL_UART_GET_FLAG(&UARTHandle, UART_FLAG_RXNE) !=1 );
    status = HAL_UART_Receive(&UARTHandle, (uint8_t *)aRxBuffer,   length , timeout); 
    return status; 
}


uint8_t UART_Data_available()
{
    if(__HAL_UART_GET_FLAG(&UARTHandle, UART_FLAG_RXNE) == 1 )
        return 1;
    else
        return 0;
}

void UART_Rx_Int_Enable()
{
    HAL_UART_Receive_IT(&UARTHandle, &UART_rxData, 1);
}


uint8_t UART_Rx_IT_Char()
{
    return UART_rxData;
}


uint8_t UART_data_available()
{
    uint8_t data=0;
    data = __HAL_UART_GET_FLAG(&UARTHandle , UART_FLAG_RXNE);
    return data;
}

/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        HAL_UART_Transmit(&UARTHandle, &byte, 1, 100);
        CDC_Transmit_FS(&byte, 1);
        HAL_UART_Receive_IT(&UARTHandle, &byte, 1);
    }
}*/



