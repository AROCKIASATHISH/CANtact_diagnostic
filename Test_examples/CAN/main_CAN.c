


#include "stm32f0xx_hal.h"
#include "can.h"
#include "led.h"
#include "string.h"

#define CAN_TX
//#define CAN_RX
//#define INTERNAL_OSCILLATOR
#define EXTERNAL_OSCILLATOR


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void led_init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_CAN_INIT();
    led_init();

    HAL_Delay(1000); 
    for (;;) {
         #ifdef CAN_TX
		  can_tx_packet("led on" ,6);
		  HAL_Delay(1000);  
		  can_tx_packet("led off" ,7);
		  HAL_Delay(1000);  
   
         #elif defined CAN_RX
                uint32_t status;
                uint8_t msg_buf[10]={'\0'};
		while (!is_can_msg_pending(CAN_FIFO0));
                      led_process();
		status = can_rx_packet((uint8_t *)&msg_buf);
                if (status == HAL_OK) { 
			if(strcmp(msg_buf,"led on") == 0){
			    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
                        }
			else if(strcmp(msg_buf,"led off") == 0){
			    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
                        }        
                }
		led_process();
                HAL_Delay(1000); 
         #endif
	
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
void led_init() {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

