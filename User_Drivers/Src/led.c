

#include "stm32f0xx_hal.h"
#include "led.h"

void led_init(void) 
{
    uint8_t i = 0;
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    both_led_on();
    for(i=0 ; i<5; i++) {
        led2_on();      
	HAL_Delay(100);
	led2_off();
        HAL_Delay(100);
    }
    both_led_off();
}


void led1_on(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);		
}

void led1_off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);		
}

void led2_on(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);		
}


void led2_off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);		
}

void both_led_on(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);	
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
}

void both_led_off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);	
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
}
