
#ifndef __usb_device_H
#define __usb_device_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "usbd_def.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
void MX_USB_DEVICE_Init(void);

#ifdef __cplusplus
}
#endif
#endif 


