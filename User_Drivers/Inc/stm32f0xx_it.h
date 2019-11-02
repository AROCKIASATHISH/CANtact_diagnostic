
#ifndef __STM32F0xx_IT_H
#define __STM32F0xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 


void USB_IRQHandler(void);
void SysTick_Handler(void);
void USART2_IRQHandler(void); 
void SPI1_IRQHandler(void);
void I2C1_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
void CEC_CAN_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif


