#ifndef _UART_H
#define _UART_H




void UART_init(uint32_t b_rate);
void UART_IRQ_Enable();
void UART_gpio_init();
uint32_t UART_Tx_packet(uint8_t *aTxBuffer , uint16_t length, uint32_t timeout);
uint32_t UART_Rx_packet(uint8_t *aRxBuffer , uint16_t length, uint32_t timeout);
uint8_t  UART_Data_available();
void UART_Rx_Int_Enable();
uint8_t UART_Rx_IT_Char();
#endif
