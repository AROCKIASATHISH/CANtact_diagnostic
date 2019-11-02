
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"


extern UART_HandleTypeDef UARTHandle;
//extern SPI_HandleTypeDef spi;
//extern I2C_HandleTypeDef i2c;
//extern CAN_HandleTypeDef hcan;




extern PCD_HandleTypeDef hpcd_USB_FS;


void USB_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{

  HAL_UART_IRQHandler(&UARTHandle);
      
}
/*
void SPI1_IRQHandler(void)
{
  
	HAL_SPI_IRQHandler(&spi);
 
}

void I2Cx_EV_IRQHandler(void)
{
  	HAL_I2C_EV_IRQHandler(&i2c);
}


void I2Cx_ER_IRQHandler(void)
{
 	 HAL_I2C_ER_IRQHandler(&i2c);
}


//void I2C1_IRQHandler(void)
//{
  
///	HAL_I2C_EV_IRQHandler(&i2c);
 
//}

void CEC_CAN_IRQHandler(void)
{
	 HAL_CAN_IRQHandler(&hcan);
}
*/


