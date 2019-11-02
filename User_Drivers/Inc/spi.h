#ifndef _SPI_H
#define _SPI_H

void SPI_gpio_init();
void SPI_Master_init();
void SPI_Slave_init();
void SPI_IRQ_Enable();
uint32_t SPI_tx_packet(uint8_t *aTxBuffer , uint16_t length , uint32_t timeout);
uint32_t SPI_rx_packet(uint8_t *aRxBuffer , uint16_t length , uint32_t timeout);
uint8_t  SPI_Data_available();
uint32_t SPI_Tx_rx_packet(uint8_t *aTxBuffer, uint8_t *aRxBuffer ,uint16_t length , uint32_t timeout);
uint8_t SPI_Rx_IT_Char();
void SPI_Rx_Int_Enable();
#endif
