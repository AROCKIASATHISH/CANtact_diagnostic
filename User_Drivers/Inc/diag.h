
#ifndef _DIAG_H
#define _DIAG_H
void diag_check_stop(void);
void uart_tx_diag_check(void);
void uart_rx_diag_check(void);
void spi_master_diag_check(void);
void spi_slave_diag_check(void);
void i2c_master_diag_check(void);
void i2c_slave_diag_check(void);
void can_tx_diag_check(void);
void can_rx_diag_check(void);
void led_diag_check(void);
void header_pin_diag_check(void);
void CANtact_diag_menu(void);
void diag_result(void);
void diag_check_stop(void);  
 
#endif
