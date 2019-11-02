#ifndef _I2C_H
#define _I2C_H

void I2C_gpio_init();
void I2C_Master_init(uint32_t FREQ);
void I2C_Slave_init(uint32_t address);
uint32_t I2C_Master_tx_packet(uint8_t *aTxBuffer , uint16_t device_address,uint16_t length , uint32_t timeout);
uint32_t I2C_Master_rx_packet(uint8_t *aRxBuffer , uint16_t device_address, uint16_t length , uint32_t timeout);
uint32_t I2C_Slave_rx_packet(uint8_t *aRxBuffer ,  uint16_t length , uint32_t timeout);
uint32_t I2C_Slave_tx_packet(uint8_t *aTxBuffer , uint16_t length , uint32_t timeout);
void I2C_IRQ_Enable();
void I2C_Rx_Int_Enable();
uint8_t I2C_Rx_IT_Char();
HAL_StatusTypeDef Isfind_i2c_address(uint16_t address);
#endif
