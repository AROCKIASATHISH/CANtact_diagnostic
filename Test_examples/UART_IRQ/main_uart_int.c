
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "uart.h"
#include "string.h"


//#define UART_TX_BOARD
#define UART_RX_BOARD

#define EXTERNAL_OSCILLATOR
#define HAL_MAX_DELAY 1000


static void MX_GPIO_Init(void);
static void led_init(void);


#ifdef UART_RX_BOARD
#define UART_Rx_buff_size 20
uint8_t UART_RxBuffer[UART_Rx_buff_size];
uint8_t UART_RX_buff_index = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    
    UART_RxBuffer[UART_RX_buff_index]=UART_Rx_IT_Char();
    UART_RX_buff_index++;

    if(UART_RX_buff_index == UART_Rx_buff_size)
     UART_RX_buff_index = 0;
   
   
    UART_Rx_Int_Enable();
  }
}
#endif 


int main(void)
{
        HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	led_init();
        UART_init(9600);

        #ifdef UART_RX_BOARD
       		 UART_IRQ_Enable();
        #endif 

	HAL_Delay(3000);

    for (;;) {

       #ifdef UART_TX_BOARD
		 uint8_t buffer1[10];
		 strcpy(buffer1, "led on");
		 UART_Tx_packet(&buffer1,sizeof(buffer1), HAL_MAX_DELAY);
		 HAL_Delay(1000); 
		 strcpy(buffer1, "led off");
		 UART_Tx_packet(&buffer1,sizeof(buffer1), HAL_MAX_DELAY);
		 HAL_Delay(1000);
 
       #elif defined UART_RX_BOARD
		if(UART_RX_buff_index != 0)
                {
                        HAL_Delay(100);
		  	if(strstr(UART_RxBuffer, "led on"))
			     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			else if(strstr(UART_RxBuffer, "led off"))
			     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
                        UART_RX_buff_index = 0;
                        memset(UART_RxBuffer,0,UART_Rx_buff_size);
               }

        #endif

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_Delay(200);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_Delay(200); 
    
}

 
}

/** System Clock Configuration
 */

void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

#ifdef INTERNAL_OSCILLATOR
    // set up the oscillators
    // use internal HSI48 (48 MHz), no PLL
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    // set sysclk, hclk, and pclk1 source to HSI48 (48 MHz)
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK |
				   RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    // set USB clock source to HSI48 (48 MHz)
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;


#elif defined EXTERNAL_OSCILLATOR
    // set up the oscillators
    // use external oscillator (16 MHz), enable 3x PLL (48 MHz)
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;

    // set sysclk, hclk, and pclk1 source to PLL (48 MHz)
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK |
				   RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    // set USB clock source to PLL (48 MHz)
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLCLK;

#else
	#error "Please define whether to use an internal or external oscillator"
#endif

    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
    __SYSCFG_CLK_ENABLE();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}



/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
void MX_GPIO_Init(void)
{

    /* GPIO Ports Clock Enable */
    __GPIOF_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
}

/* USER CODE BEGIN 4 */
static void led_init() {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}











