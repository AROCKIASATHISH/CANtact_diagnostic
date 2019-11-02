#include "stm32f0xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "diag.h"
#include "can.h"
#include "led.h"
#include "spi.h"
#include "i2c.h"
#include "uart.h"
#include "connector_pin.h"
#include "string.h"
#include "tui.h"


#define I2C_address 125
#define HAL_DELAY 500
#define UART_Rx_buff_size 20

enum test_status {
     IDLE, 
     UART_TRANSMITTER, 
     UART_RECEIVER,
     SPI_MASTER, 
     SPI_SLAVE, 
     I2C_MASTER, 
     I2C_SLAVE, 
     CAN_TRANSMITTER, 
     CAN_RECEIVER, 
     LED, 
     HEADER_PINS ,
     DIAG_RESULT
};

enum result { FAIL ,PASS };
enum test_status test_state = IDLE ;
static uint32_t last_on = 0;
static uint32_t last_off = 0;
static uint8_t  last_state = 0; 
extern  int test_results[15];

uint8_t UART_RxBuffer[UART_Rx_buff_size];
uint8_t UART_RX_buff_index = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        UART_RxBuffer[UART_RX_buff_index]=UART_Rx_IT_Char();
        UART_RX_buff_index++;
        if(UART_RX_buff_index == UART_Rx_buff_size)
            UART_RX_buff_index = 0;
        UART_Rx_Int_Enable();
    }
}


void uart_tx_diag_check(void) 
{
    if (test_state != UART_TRANSMITTER) {
        last_state = 0 ;
        diag_check_stop();
 	UART_init(9600);
        HAL_Delay(2000); 
    }

    uint8_t buffer1[20] = {'\0'};
    if(last_state == 0 && (HAL_GetTick() - last_off)> 1000) {
        strcpy((char *)buffer1, "led on");
        UART_Tx_packet((uint8_t *)&buffer1,sizeof(buffer1), HAL_DELAY);
        last_on = HAL_GetTick();
        last_state = 1 ;
    }else if(last_state == 1 && (HAL_GetTick() - last_on)> 1000) {
        strcpy((char *)buffer1, "led off");
        UART_Tx_packet((uint8_t *)&buffer1,sizeof(buffer1), HAL_DELAY);
        last_off = HAL_GetTick();
        last_state = 0 ; 
   }
   test_state = UART_TRANSMITTER ;      		 
}


void uart_rx_diag_check(void) 
{
    if (test_state != UART_RECEIVER) {
        last_state = 0 ;
        diag_check_stop();
	UART_init(9600); 
        UART_IRQ_Enable();
        HAL_Delay(2000);
    }

    if( UART_RX_buff_index != 0 ) {
        HAL_Delay(10);
	if(strstr((char *)UART_RxBuffer, "led on")) {
            led2_on();
            test_results[UART_RECEIVER] = PASS;
	}else if(strstr((char *)UART_RxBuffer, "led off")) {
            led2_off();
            test_results[UART_RECEIVER] = PASS;
        }
        UART_RX_buff_index = 0;
        memset(UART_RxBuffer,0,UART_Rx_buff_size) ;
    }
    test_state = UART_RECEIVER ; 
}

void spi_master_diag_check(void) 
{
    if (test_state != SPI_MASTER) {
        last_state = 0 ;
        diag_check_stop();
	SPI_Master_init();
        HAL_Delay(2000);
    }

    uint8_t buffer1[20] = {'\0'};
    if(last_state == 0 && (HAL_GetTick() - last_off)> 1000) {
        strcpy((char *)buffer1, "led on");
        SPI_tx_packet((uint8_t *)&buffer1,sizeof(buffer1),HAL_DELAY);
        last_on = HAL_GetTick();
        last_state = 1 ;
    }else if(last_state == 1 && (HAL_GetTick() - last_on)> 1000) {
        strcpy((char *)buffer1, "led off");
        SPI_tx_packet((uint8_t *)&buffer1,sizeof(buffer1),HAL_DELAY);
        last_off = HAL_GetTick();
        last_state = 0 ; 
    }
    test_state = SPI_MASTER ;
}

void spi_slave_diag_check(void) 
{
    if (test_state != SPI_SLAVE) {
        diag_check_stop();
	SPI_Slave_init(); 
        HAL_Delay(2000);
    }

    uint8_t buffer[20] = {'\0'};
    if(SPI_rx_packet((uint8_t *)&buffer ,20 , HAL_DELAY)== HAL_OK) {
        if(strcmp((char *)buffer, "led on" ) == 0) {
             led2_on();
             test_results[SPI_SLAVE] = PASS;
        } else if (strcmp((char *)buffer, "led off" ) == 0) {
             led2_off();
             test_results[SPI_SLAVE] = PASS;
        }
    }
    test_state = SPI_SLAVE ;
}

void i2c_master_diag_check(void) 
{
	
    if (test_state != I2C_MASTER) {
        last_state = 0 ;
        diag_check_stop();
	I2C_Master_init(100);  //100khz 
        HAL_Delay(2000);
    }  
    uint8_t i;
    for (i=1; i<128; i++) {
 	if (Isfind_i2c_address(i) == HAL_OK) {
             test_results[I2C_MASTER] = PASS ;
             led1_on();
        }
    } 
   test_state = I2C_MASTER ;
}


void i2c_slave_diag_check(void) 
{
    if (test_state != I2C_SLAVE) {
        diag_check_stop();
	I2C_Slave_init(I2C_address << 1); // init slave address 250 
        HAL_Delay(2000);
    }

    uint8_t buffer[20] = {'\0'};
    if(I2C_Slave_rx_packet((uint8_t *)&buffer ,20 , HAL_DELAY)== HAL_OK) {
        if (strcmp((char *)buffer, "led on") == 0) {
     	    led2_on();
            test_results[I2C_SLAVE] = PASS;
        } else if (strcmp((char *)buffer, "led off") == 0) {
     	    led2_off();
            test_results[I2C_SLAVE] = PASS;
        }
    } 
    test_state = I2C_SLAVE ;
}

void can_tx_diag_check(void) 
{
    if (test_state != CAN_TRANSMITTER) {
        last_state = 0 ;
        diag_check_stop();
	MX_CAN_INIT(); 
        HAL_Delay(2000);
     }     
      
    if(last_state == 0 && (HAL_GetTick() - last_off)> 1000) {
        can_tx_packet((uint8_t *)"led on", 6);
        last_on = HAL_GetTick();
        last_state = 1 ;
    }else if(last_state == 1 && (HAL_GetTick() - last_on)> 1000) {
        can_tx_packet((uint8_t *)"led off", 7);
        last_off = HAL_GetTick();
        last_state = 0 ; 
    }
    test_state = CAN_TRANSMITTER;
}


void can_rx_diag_check(void) 
{
    if (test_state != CAN_RECEIVER) {
        diag_check_stop(); 
	MX_CAN_INIT();
        HAL_Delay(2000);
    }  


    uint32_t status;
    uint8_t msg_buf[10] = {'\0'};
    status = can_rx_packet((uint8_t *)&msg_buf);
    if (status == HAL_OK) { 
        if (strcmp((char *)msg_buf,"led on") == 0) {
	    led2_on();
            test_results[CAN_RECEIVER] = PASS;
        } else if (strcmp((char *)msg_buf,"led off") == 0) {
	    led2_off();
            test_results[CAN_RECEIVER] = PASS;
        }        
    }
    test_state = CAN_RECEIVER;
}

void led_diag_check(void) 
{    
   if (test_state != LED) {
      last_state = 0 ;
   }
   if(last_state == 0 && (HAL_GetTick() - last_off)> 1000) {
           both_led_on();  
           last_on = HAL_GetTick();
           last_state = 1 ;
    } else if(last_state == 1 && (HAL_GetTick() - last_on)> 1000) {
           both_led_off(); 
           last_off = HAL_GetTick();
           last_state = 0 ; 
   }
   test_state = LED;
}

void header_pin_diag_check(void)
{
    if (test_state != HEADER_PINS) {
        last_state = 0 ;
        diag_check_stop();
	All_header_pin_direction_ouput();
        HAL_Delay(2000);
    }
     if(last_state == 0 && (HAL_GetTick() - last_off)> 1000) {
           all_header_set_high();
           last_on = HAL_GetTick();
           last_state = 1 ;
    } else if(last_state == 1 && (HAL_GetTick() - last_on)> 1000) {
           all_header_set_low();
           last_off = HAL_GetTick();
           last_state = 0 ; 
    }

    test_state = HEADER_PINS ;
}

void CANtact_diag_menu(void)
{       
    diag_check_stop();
    all_header_set_low();
    test_state = IDLE ;
}


void diag_check_stop(void)
{
    both_led_off();
    all_header_set_low();
    test_state = IDLE;
}








