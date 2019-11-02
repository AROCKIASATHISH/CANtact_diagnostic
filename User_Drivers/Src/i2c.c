#include "stm32f0xx_hal.h"
#include "i2c.h"


I2C_HandleTypeDef i2c;
uint8_t I2C_rxData;

void I2C_Master_init(uint32_t FREQ)
{
    I2C_gpio_init();
    __I2C1_CLK_ENABLE();
    i2c.Instance = I2C1;
    i2c.Init.Timing = (FREQ)*1000;
    i2c.Init.OwnAddress1 = 0;
    i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    i2c.Init.OwnAddress2 = 0;
    i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
    HAL_I2C_Init(&i2c);
    __HAL_I2C_ENABLE(&i2c);
}

void I2C_Slave_init(uint32_t address)
{
    I2C_gpio_init();
    __I2C1_CLK_ENABLE();
    i2c.Instance = I2C1;
    i2c.Init.Timing = 100000;
    i2c.Init.OwnAddress1 = address;
    //250; /* Own Address 125 */
    i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    i2c.Init.OwnAddress2 = 0;
    i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
    HAL_I2C_Init(&i2c);
   __HAL_I2C_ENABLE(&i2c);
}

void I2C_gpio_init()
{
    //I2C1 GPIO Configuration    
    //  PB6     ------> I2C1_SCL
    //  PB7     ------> I2C1_SDA 
    GPIO_InitTypeDef GPIO_InitStruct;
    __GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void I2C_IRQ_Enable()
{
    HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_IRQn);
    __HAL_I2C_ENABLE_IT(&i2c, I2C_IT_RXI);
    // __HAL_I2C_ENABLE_IT(&i2c,I2C_IT_ADDRI);
    // HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    //HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
    I2C_Rx_Int_Enable();
	

}

uint32_t I2C_Master_tx_packet(uint8_t *aTxBuffer , uint16_t device_address,uint16_t length , uint32_t timeout)
{
    uint32_t status;
    status = HAL_I2C_Master_Transmit(&i2c, (uint16_t)(device_address << 1), (uint8_t *)aTxBuffer, length, timeout);
    while(__HAL_I2C_GET_FLAG(&i2c,I2C_FLAG_TXE)!=1);
    return status;
}

uint32_t I2C_Master_rx_packet(uint8_t *aRxBuffer , uint16_t device_address, uint16_t length , uint32_t timeout)
{
    uint32_t status;
    while(__HAL_I2C_GET_FLAG(&i2c,I2C_FLAG_RXNE)!=1);
    status = HAL_I2C_Master_Receive(&i2c, (uint16_t)device_address, (uint8_t *)aRxBuffer, length, timeout);
    return status;
}

uint32_t I2C_Slave_rx_packet(uint8_t *aRxBuffer ,  uint16_t length , uint32_t timeout)
{
    uint32_t status;
    status = HAL_I2C_Slave_Receive(&i2c, (uint8_t *)aRxBuffer,length, timeout);
    return status;
}

uint32_t I2C_Slave_tx_packet(uint8_t *aTxBuffer , uint16_t length , uint32_t timeout)
{
    uint32_t status;
    status = HAL_I2C_Slave_Transmit(&i2c, (uint8_t*)aTxBuffer, length, timeout);
    return status;
}

HAL_StatusTypeDef Isfind_i2c_address(uint16_t address)
{
    HAL_StatusTypeDef result;
    result = HAL_I2C_IsDeviceReady(&i2c,(uint16_t)(address<<1) , 2 ,2);
    return result;
}

void I2C_Rx_Int_Enable()
{
    HAL_I2C_Slave_Receive_IT(&i2c, &I2C_rxData, 1);
}

uint8_t I2C_Rx_IT_Char()
{
    return I2C_rxData;
}
