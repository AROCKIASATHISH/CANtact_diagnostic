#ifndef _HEADER_PIN_H
#define _HEADER_PIN_H

  /**USART2 GPIO Configuration
	PA2     ------> USART2_TX
	PA3     ------> USART2_RX
	*/ 
   
    /**SPI GPIO Configuration
        PA4     ------> SPI_CS
	PA5     ------> SPI_CLK
	PA6     ------> SPI_MISO
        PA7     ------> SPI_MOSI
     */  

     /**I2C1 GPIO Configuration
        PB5     ------> I2C1_INT   
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA   
     */   
      
     /**CAN GPIO Configuration
	PB8     ------> CAN_RX
	PB9     ------> CAN_TX

     */


//PORTA
#define USART2_TX  GPIO_PIN_2
#define USART2_RX  GPIO_PIN_3

#define SPI_CS     GPIO_PIN_4
#define SPI_CLK    GPIO_PIN_5
#define SPI_MISO   GPIO_PIN_6
#define SPI_MOSI   GPIO_PIN_7

#define SWDIO   GPIO_PIN_13
#define SWCLK   GPIO_PIN_14

//PORTB
#define I2C1_INT   GPIO_PIN_5
#define I2C1_SCL   GPIO_PIN_6
#define I2C1_SDA   GPIO_PIN_7

#define CAN_RX     GPIO_PIN_8
#define CAN_TX     GPIO_PIN_9


#define ALL_HEADER_PINS_PORTA  (USART2_TX | USART2_RX | SPI_CS | \
                                   SPI_CLK | SPI_MISO | SPI_MOSI | SWDIO | SWCLK ) 

#define ALL_HEADER_PINS_PORTB  (I2C1_INT | I2C1_SCL | I2C1_SDA | CAN_RX | CAN_TX ) 



void All_header_pin_direction_ouput(void);
void All_header_pin_direction_input(void);
void all_header_set_high(void);
void all_header_set_low(void);

#endif 



