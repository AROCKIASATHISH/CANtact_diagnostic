#include "stm32f0xx_hal.h"
#include "tui.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#define UP 1
#define DOWN 0
int test_results[15];
enum result { FAIL , PASS };
uint8_t *tests[] = { " ","   UART TX   " , "UART RX    ", "   SPI MASTER", "SPI_SLAVE  " , "   I2C MASTER", \
                                    "I2C  SLAVE ", "   CAN TX    ", "CAN RX     ", "   LED       ", "HEADER PINS" };

void TUI_init(void)
{
        uint8_t buffer[7] = {'\0'} ;
	buffer[0] = 0x1B;
	buffer[1] = '[';
	buffer[2] = '?';
        buffer[3] = '5';
        buffer[4] = '0';
        buffer[5] = 'h';
	CDC_Transmit_FS((uint8_t *)&buffer , 6);
}
void TUI_Up(void)
{
	uint8_t buffer[5] = {'\0'} ;
	buffer[0] = 0x1B;
	buffer[1] = '[';
	buffer[2] = 'A';
	CDC_Transmit_FS((uint8_t *)&buffer , 3);
}

void TUI_Down(void)
{
	uint8_t buffer[5] = {'\0'} ;
	buffer[0] = 0x1B;
	buffer[1] = '[';
	buffer[2] = 'B';
	CDC_Transmit_FS((uint8_t *)&buffer , 3);
}

void TUI_move_forward(void)
{
	uint8_t buffer[5] = {'\0'} ;
	buffer[0] = 0x1B;
	buffer[1] = '[';
	buffer[2] = 'C';
	CDC_Transmit_FS((uint8_t *)&buffer , 3);
}

void TUI_move_backward(void)
{
	uint8_t buffer[5] = {'\0'} ;
	buffer[0] = 0x1B ;
	buffer[1] = '[';
	buffer[2] = 'D';
	CDC_Transmit_FS((uint8_t *)&buffer , 3);
}


void TUI_color_set(void)
{
        uint8_t buffer[12] = {'\0'};
        buffer[0] = 0x1B;
        buffer[1] = '[';
        buffer[2] = '7';
        buffer[3] = ';';
        buffer[4] = '3';
        buffer[5] = '7';
        buffer[6] = ';';
        buffer[7] = '4';
        buffer[8] = '0';
        buffer[9] = 'm';
	CDC_Transmit_FS((uint8_t *)&buffer , 10);
}

void TUI_color_notset(void)
{
	uint8_t buffer[12] = {'\0'};
        buffer[0] = 0x1B;
        buffer[1] = '[';
        buffer[2] = '0';
        buffer[3] = ';' ;
        buffer[4] = '3';
        buffer[5] = '7';
        buffer[6] = ';';
        buffer[7] = '4';
        buffer[8] = '0';
        buffer[9] = 'm';
	CDC_Transmit_FS((uint8_t *)&buffer , 10);
	
}


void TUI_init_position(uint8_t cnt)
{
	int i;
	for(i=0;i<cnt;i++){
	    TUI_Up();
	}
}



void TUI_menu_process(uint8_t choice,char *menu_sub1[], uint8_t status)
{

	int i;
        static uint8_t count=0;
        static uint8_t last_choice;
	for(i=0;i<14 ;i++){
		TUI_Down();
        }

        for(i=0;i<21;i++){
	    TUI_move_backward();
	}
        CDC_Transmit_FS((uint8_t *)"\n\r ", 3);
        CDC_Transmit_FS((uint8_t *)menu_sub1[choice], strlen(menu_sub1[choice]));
        CDC_Transmit_FS((uint8_t *)"          ", 10);

        if(status == 1 ) {
           test_results[choice] = PASS ;
           TUI_color_notset();
           CDC_Transmit_FS((uint8_t *)"\n\r  PASS" ,8);
           TUI_clear(1);
        }else if(status == 0) {
           test_results[choice] = FAIL ;
           TUI_color_notset();
           CDC_Transmit_FS((uint8_t *)"\n\r  FAIL" ,8);
           TUI_clear(1);
        }else if(choice == 12) {
            TUI_color_notset();
            CDC_Transmit_FS((uint8_t *)"\n\r" ,2);
            count+=5;
            for(i=1; i<11; i++) {
                CDC_Transmit_FS((uint8_t *)tests[i], strlen(tests[i]));
                if(test_results[i] == 1)
                    CDC_Transmit_FS((uint8_t *)"  PASS  ",8);
                else
                    CDC_Transmit_FS((uint8_t *)"  FAIL  ",8);
                if(i%2 == 0) {
                    CDC_Transmit_FS((uint8_t *)"\n\r" ,2);
                }
            }
        }else if(choice == 13 ) {
            TUI_color_notset();
            TUI_Up();
            for(i=1; i<=7; i++){
                 CDC_Transmit_FS((uint8_t *)"                                             " ,43);
	         if(i!=7)
		    CDC_Transmit_FS((uint8_t *)"\n\r" ,2); 
            }
            for(i=0;i<33;i++){
	        TUI_move_backward();
	    }
            for(i=1;i<5 ;i++){
	        TUI_Up();
	    }    
        }else {
            if(last_choice == 12) {
                for(i=1; i<=6; i++){
                    CDC_Transmit_FS((uint8_t *)"                                             " ,43);
	            if(i!=6)
		        CDC_Transmit_FS((uint8_t *)"\n\r" ,2); 
                }
                for(i=0;i<33;i++){
	            TUI_move_backward();
	        }
                for(i=1;i<6 ;i++){
	            TUI_Up();
	        }    
            } 
            CDC_Transmit_FS((uint8_t *)"\n\r  PASS /FAIL" ,14);
        }


        for(i=0;i<30;i++){
	    TUI_move_backward();
	}
        for(i=0;i<(16+count) ;i++){
             TUI_Up();
        }
        last_choice=choice;
        count=0;
        
}




void TUI_clear(uint8_t line)
{
	uint8_t i=0;
	for(i=1; i<=line; i++){
	    CDC_Transmit_FS((uint8_t *)"                " ,16);
	    if(i!=line)
		CDC_Transmit_FS((uint8_t *)"\n\r" ,2); 
	}
        for(i=0;i<17;i++){
	    TUI_move_backward();
	}
        for(i=1;i<line ;i++){
	    TUI_Up();
	}          

}

void TUI_set_position(char pos,int line,char *menu_sub[],int coun)
{
	int i;
	for(i=0;i<21;i++){
	    TUI_move_backward();
	}
	TUI_color_notset();
	if(coun==14){
	    CDC_Transmit_FS((uint8_t *)menu_sub[coun-1] ,strlen(menu_sub[coun-1]));
	    coun=2;
	}
	else if(coun==0){
	    CDC_Transmit_FS((uint8_t *)menu_sub[coun+1] ,strlen(menu_sub[coun+1]));
	    coun=12;
	}
	else{
	    CDC_Transmit_FS((uint8_t *)menu_sub[coun] ,strlen(menu_sub[coun]));
	}
        if(pos==DOWN) {
	    for(i=0;i<line;i++){
	        TUI_Down();
	    }
        }
        else if(pos==UP){
            for(i=0;i<line;i++){
	        TUI_Up();
	    }
        }
	for(i=0;i<21;i++){
	    TUI_move_backward();;
	}
	TUI_color_set();
	if(pos==DOWN)
	    CDC_Transmit_FS((uint8_t *)menu_sub[coun+1] ,strlen(menu_sub[coun+1]));
	else
	    CDC_Transmit_FS(menu_sub[coun-1] ,strlen(menu_sub[coun-1]));
}
