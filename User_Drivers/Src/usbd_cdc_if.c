
#include "usbd_cdc_if.h"
#include "can.h"

#define APP_RX_DATA_SIZE  32
#define APP_TX_DATA_SIZE  32

uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];


USBD_HandleTypeDef  *hUsbDevice_0;

extern USBD_HandleTypeDef hUsbDeviceFS;
extern uint8_t option_select;
extern void get_option_from_usb(uint8_t* option , uint8_t length);

static int8_t CDC_Init_FS     (void);
static int8_t CDC_DeInit_FS   (void);
static int8_t CDC_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS  (uint8_t* pbuf, uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
    CDC_Init_FS,
    CDC_DeInit_FS,
    CDC_Control_FS,
    CDC_Receive_FS
};


static int8_t CDC_Init_FS(void)
{
    hUsbDevice_0 = &hUsbDeviceFS;
    USBD_CDC_SetRxBuffer(hUsbDevice_0, UserRxBufferFS);
    return (USBD_OK);
}


static int8_t CDC_DeInit_FS(void)
{
    return (USBD_OK);
   
}

static int8_t CDC_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
 
    switch (cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:

	break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

	break;

    case CDC_SET_COMM_FEATURE:

	break;

    case CDC_GET_COMM_FEATURE:

	break;

    case CDC_CLEAR_COMM_FEATURE:

	break;

	/*******************************************************************************/
	/* Line Coding Structure                                                       */
	/*-----------------------------------------------------------------------------*/
	/* Offset | Field       | Size | Value  | Description                          */
	/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
	/* 4      | bCharFormat |   1  | Number | Stop bits                            */
	/*                                        0 - 1 Stop bit                       */
	/*                                        1 - 1.5 Stop bits                    */
	/*                                        2 - 2 Stop bits                      */
	/* 5      | bParityType |  1   | Number | Parity                               */
	/*                                        0 - None                             */
	/*                                        1 - Odd                              */
	/*                                        2 - Even                             */
	/*                                        3 - Mark                             */
	/*                                        4 - Space                            */
	/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
	/*******************************************************************************/
    case CDC_SET_LINE_CODING:

	break;

    case CDC_GET_LINE_CODING:
	pbuf[0] = (uint8_t)(115200);
	pbuf[1] = (uint8_t)(115200 >> 8);
	pbuf[2] = (uint8_t)(115200 >> 16);
	pbuf[3] = (uint8_t)(115200 >> 24);
	pbuf[4] = 0; // stop bits (1)
	pbuf[5] = 0; // parity (none)
	pbuf[6] = 8; // number of bits (8)
	break;

    case CDC_SET_CONTROL_LINE_STATE:

	break;

    case CDC_SEND_BREAK:

	break;

    default:
	break;
    }
    return (USBD_OK);
}


static int8_t CDC_Receive_FS (uint8_t* Buf, uint32_t *Len)
{
    get_option_from_usb(Buf, *Len);
    USBD_CDC_ReceivePacket(hUsbDevice_0);
    return (USBD_OK);
}


uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
    uint8_t result = USBD_OK;
    uint16_t i;

    for (i=0; i < sizeof(UserTxBufferFS); i++) {
	UserTxBufferFS[i] = 0;
    }

    for (i=0; i < Len; i++) {
	UserTxBufferFS[i] = Buf[i];
    }

    USBD_CDC_SetTxBuffer(hUsbDevice_0, UserTxBufferFS, Len);
    //do {
    result = USBD_CDC_TransmitPacket(hUsbDevice_0);
    //}while (result == HAL_BUSY);
    HAL_Delay(2);
    return result;
}


