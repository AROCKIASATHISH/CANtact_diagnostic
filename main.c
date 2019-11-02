#include "string.h"
#include "stdlib.h"
#include "stm32f0xx_hal.h"
#include "system_config.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "diag.h"
#include "tui.h"
#include "led.h"

#define Auto_Test_duration 7000 

void get_option_from_usb(uint8_t* option , uint8_t length);
void mfg_diag_actions(uint8_t option);
void TUI_display_menu(uint8_t cnt);
void auto_diag(void);


uint8_t usb_rx_buff[10] ={'\0'};
uint8_t test_no;
char count= 1;
uint8_t auto_diag_enable  = 0;
static uint32_t last_test= 0;
#define UP 1
#define DOWN 0



char *menu[] = { "     Menu ",
	" 1.UART tx test " ,
	" 2.UART rx test " ,
	" 3.SPI MASTER Test " ,
	" 4.SPI SLAVE test " ,
	" 5.I2C MASTER Test " ,
	" 6.I2C SLAVE test " ,
	" 7.CAN TX test " ,
	" 8.CAN RX test " ,
	" 9.LED test " ,
	" 10.Header pin test " ,
	" 11.Auto diag" ,
	" 12.View diag result" ,
	" 13.Exit "
};




void TUI_display_menu(uint8_t cnt)
{
        uint8_t i=0;
        for (i = 0; i < 14; i++) {
		if(i==cnt)
			TUI_color_set();
		if(i!=cnt)
			TUI_color_notset();
                CDC_Transmit_FS((uint8_t *)menu[i] , strlen(menu[i]));
                if(i != 13 )
                    CDC_Transmit_FS((uint8_t *)"\n\r" , 2);
        }
        for(i=0;i<21;i++){
	    TUI_move_backward();
	}
        TUI_init_position(13);          
}




//main
int main(void)
{
        HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	led_init();
	MX_USB_DEVICE_Init();
        last_test = HAL_GetTick(); 
        for (;;) {
             if(test_no <=10 && strstr((char *)usb_rx_buff ,"[C")) {
                 TUI_menu_process(test_no ,menu , 0);
                 memset(usb_rx_buff,0,10);
             }else if(test_no <=10 &&  strstr((char *)usb_rx_buff ,"[D")) {
                 TUI_menu_process(test_no ,menu , 1);
                 memset(usb_rx_buff,0,10);
             }else if(strstr((char *)usb_rx_buff ,"[B")) {
		 count++;
                 if(count > 13)
                     count=1;
                 TUI_display_menu(count);
                 memset(usb_rx_buff,0,10);
             }else if(strstr((char *)usb_rx_buff ,"[A")) {
		 count-- ;
                 if(count < 1)
                     count=13 ;
                 TUI_display_menu(count);
                 memset(usb_rx_buff,0,10);
             }else if(usb_rx_buff[0]=='\r') {
                 test_no = count;
                 TUI_menu_process(test_no ,menu , 2);
                 if(test_no == 11)
                       count = 1;
                 memset(usb_rx_buff,0,10);
             }else {           
                 mfg_diag_actions(test_no);  
                 if(auto_diag_enable == 1) {
                     auto_diag();
                 }
             }                                       
       } 
}


void auto_diag(void)
{
	if (HAL_GetTick() - last_test > Auto_Test_duration) {
                if(count == 11 )
                    count++;
		TUI_display_menu(count);
                //TUI_set_position(DOWN,1,menu,count);
		test_no = count ; 
                TUI_menu_process(test_no ,menu , 2);
		count++;
		if(count >=13)
		    count=1;
               last_test = HAL_GetTick();
	}
}

void mfg_diag_actions(uint8_t option)
{
        switch (option) {
	case 0:
		//CANtact_diag_menu();
                 ;
		break;
	case 1:
		uart_tx_diag_check();
		break;
        case 2:
		uart_rx_diag_check();
		break;
	case 3:
		spi_master_diag_check();
		break;
        case 4:
		spi_slave_diag_check();
		break;
	case 5:
		i2c_master_diag_check();
		break;
        case 6:
		i2c_slave_diag_check();
		break;
	case 7:
		can_tx_diag_check();
		break;
        case 8:
		can_rx_diag_check();
		break;
	case 9:
		led_diag_check();
		break;
        case 10:
		header_pin_diag_check();
		break;
        case 11:
                auto_diag_enable = 1; 
		break;
        case 12:
                auto_diag_enable = 0;
                diag_check_stop();
		break;
        case 13:
		diag_check_stop();
		break;
	default:
		break;
	}   
}



void get_option_from_usb(uint8_t* option , uint8_t length)
{  
        uint8_t i=0;
	for(i=0; i< 10; i++) {
	    usb_rx_buff[i] = 0 ;
	}
	for(i=0; i< length; i++) {
	    usb_rx_buff[i] =  option[i] ;
	}
        
}




