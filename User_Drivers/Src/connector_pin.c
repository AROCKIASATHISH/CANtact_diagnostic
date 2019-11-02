#include "stm32f0xx_hal.h"
#include "connector_pin.h"    


void All_header_pin_direction_ouput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = ALL_HEADER_PINS_PORTA;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = ALL_HEADER_PINS_PORTB ;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void All_header_pin_direction_input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = ALL_HEADER_PINS_PORTA ;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = ALL_HEADER_PINS_PORTB;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void all_header_set_high(void)
{
    HAL_GPIO_WritePin(GPIOA , ALL_HEADER_PINS_PORTA ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB , ALL_HEADER_PINS_PORTB ,GPIO_PIN_SET);
}

void all_header_set_low(void)
{
    HAL_GPIO_WritePin(GPIOA , ALL_HEADER_PINS_PORTA ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB , ALL_HEADER_PINS_PORTB ,GPIO_PIN_RESET);
}









