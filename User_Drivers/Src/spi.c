#include "stm32f0xx_hal.h"
#include "spi.h"

SPI_HandleTypeDef spi;
uint8_t SPI_rxData;

void SPI_gpio_init()
{
    /**SPI GPIO Configuration
        PA4     ------> SPI_CS
	PA5     ------> SPI_CLK
	PA6     ------> SPI_MISO
        PA7     ------> SPI_MOSI
     */  

   __GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1 ;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin  = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
}

void SPI_Master_init()
{
    
   __SPI1_CLK_ENABLE();
    SPI_gpio_init();
    spi.Instance = SPI1;
    spi.Init.Mode = SPI_MODE_MASTER; 
    spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    spi.Init.Direction = SPI_DIRECTION_2LINES;
    spi.Init.CLKPhase = SPI_PHASE_2EDGE;
    spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    spi.Init.DataSize = SPI_DATASIZE_8BIT;
    spi.Init.FirstBit = SPI_FIRSTBIT_LSB;
    spi.Init.NSS = SPI_NSS_SOFT;
    spi.Init.TIMode = SPI_TIMODE_DISABLED; 
    HAL_SPI_Init(&spi);
    __HAL_SPI_ENABLE(&spi);

      
}

void SPI_Slave_init()
{
   __SPI1_CLK_ENABLE();
    SPI_gpio_init();
    spi.Instance = SPI1;
    spi.Init.Mode = SPI_MODE_SLAVE; 
    spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    spi.Init.Direction = SPI_DIRECTION_2LINES;
    spi.Init.CLKPhase = SPI_PHASE_2EDGE;
    spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    spi.Init.DataSize = SPI_DATASIZE_8BIT;
    spi.Init.FirstBit = SPI_FIRSTBIT_LSB;
    spi.Init.NSS = SPI_NSS_SOFT;
    spi.Init.TIMode = SPI_TIMODE_DISABLED;
    HAL_SPI_Init(&spi);
    __HAL_SPI_ENABLE(&spi);
}

void SPI_IRQ_Enable()
{
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    __HAL_SPI_ENABLE_IT(&spi, SPI_IT_RXNE);
    SPI_Rx_Int_Enable();
	

}

uint32_t SPI_tx_packet(uint8_t *aTxBuffer , uint16_t length , uint32_t timeout){
    uint32_t status;
    status = HAL_SPI_Transmit(&spi,(uint8_t *)aTxBuffer,length, timeout);
    while (__HAL_SPI_GET_FLAG(&spi,SPI_FLAG_TXE) != 1);
    return status;
}


uint32_t SPI_rx_packet(uint8_t *aRxBuffer , uint16_t length , uint32_t timeout){
    uint32_t status;
    status = HAL_SPI_Receive(&spi,(uint8_t *)aRxBuffer,length, timeout);
    return status;
}

uint8_t SPI_Data_available()
{
    if(__HAL_SPI_GET_FLAG(&spi,SPI_FLAG_RXNE) ==1)
        return 1;
    else
        return 0;

}

uint32_t SPI_Tx_rx_packet(uint8_t *aTxBuffer, uint8_t *aRxBuffer ,uint16_t length , uint32_t timeout)
{
    uint32_t status;
    status=HAL_SPI_TransmitReceive(&spi, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, length, timeout);
    return status;
}

void SPI_Rx_Int_Enable()
{
    HAL_SPI_Receive_IT(&spi, &SPI_rxData, 1);
}

uint8_t SPI_Rx_IT_Char()
{
    return SPI_rxData;
}
