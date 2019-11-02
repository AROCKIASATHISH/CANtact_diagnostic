
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "usbd_def.h"
#include "usbd_core.h"

PCD_HandleTypeDef hpcd_USB_FS;


__IO uint32_t remotewakeupon=0;

static void SystemClockConfig_Resume(void);
void HAL_PCDEx_SetConnectionState(PCD_HandleTypeDef *hpcd, uint8_t state);
extern void SystemClock_Config(void);

void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
  if(hpcd->Instance==USB)
  {
    __USB_CLK_ENABLE();
    HAL_NVIC_SetPriority(USB_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_IRQn);
  }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
  if(hpcd->Instance==USB)
  {
    __USB_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(USB_IRQn);
  }
}


void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_SetupStage(hpcd->pData, (uint8_t *)hpcd->Setup);
}


void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  USBD_LL_DataOutStage(hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}


void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  USBD_LL_DataInStage(hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}


void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_SOF(hpcd->pData);
}


void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_SpeedTypeDef speed = USBD_SPEED_FULL;
  switch (hpcd->Init.speed)
  {
  case PCD_SPEED_FULL:
    speed = USBD_SPEED_FULL;
    break;

  default:
    speed = USBD_SPEED_FULL;
    break;
  }
  USBD_LL_SetSpeed(hpcd->pData, speed);
  USBD_LL_Reset(hpcd->pData);
}


void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_Suspend(hpcd->pData);
  if (hpcd->Init.low_power_enable)
  {
    
    SCB->SCR |= (uint32_t)((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
  }
}

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
  
  if ((hpcd->Init.low_power_enable)&&(remotewakeupon == 0))
  {
    SystemClockConfig_Resume();
    SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
  }
  remotewakeupon=0;
  USBD_LL_Resume(hpcd->pData);

}


void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  USBD_LL_IsoOUTIncomplete(hpcd->pData, epnum);
}


void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  USBD_LL_IsoINIncomplete(hpcd->pData, epnum);
}


void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_DevConnected(hpcd->pData);
}


void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_DevDisconnected(hpcd->pData);
}


USBD_StatusTypeDef  USBD_LL_Init (USBD_HandleTypeDef *pdev)
{
  hpcd_USB_FS.pData = pdev;
  pdev->pData = &hpcd_USB_FS;

  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.ep0_mps = DEP0CTL_MPS_8;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  HAL_PCD_Init(&hpcd_USB_FS);

    HAL_PCDEx_PMAConfig(pdev->pData , 0x00 , PCD_SNG_BUF, 0x18+0*USB_FS_MAX_PACKET_SIZE);
    HAL_PCDEx_PMAConfig(pdev->pData , 0x80 , PCD_SNG_BUF, 0x18+1*USB_FS_MAX_PACKET_SIZE);
    HAL_PCDEx_PMAConfig(pdev->pData , 0x01 , PCD_SNG_BUF, 0x18+2*USB_FS_MAX_PACKET_SIZE);
    HAL_PCDEx_PMAConfig(pdev->pData , 0x81 , PCD_SNG_BUF, 0x18+3*USB_FS_MAX_PACKET_SIZE);
  return USBD_OK;
}


USBD_StatusTypeDef  USBD_LL_DeInit (USBD_HandleTypeDef *pdev)
{
  HAL_PCD_DeInit(pdev->pData);
  return USBD_OK;
}


USBD_StatusTypeDef  USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
  HAL_PCD_Start(pdev->pData);
  return USBD_OK;
}


USBD_StatusTypeDef  USBD_LL_Stop (USBD_HandleTypeDef *pdev)
{
  HAL_PCD_Stop(pdev->pData);
  return USBD_OK;
}


USBD_StatusTypeDef  USBD_LL_OpenEP  (USBD_HandleTypeDef *pdev,
				      uint8_t  ep_addr,
				      uint8_t  ep_type,
				      uint16_t ep_mps)
{

  HAL_PCD_EP_Open(pdev->pData,
		  ep_addr,
		  ep_mps,
		  ep_type);

  return USBD_OK;
}


USBD_StatusTypeDef  USBD_LL_CloseEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{

  HAL_PCD_EP_Close(pdev->pData, ep_addr);
  return USBD_OK;
}


USBD_StatusTypeDef  USBD_LL_FlushEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{

  HAL_PCD_EP_Flush(pdev->pData, ep_addr);
  return USBD_OK;
}


USBD_StatusTypeDef  USBD_LL_StallEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{

  HAL_PCD_EP_SetStall(pdev->pData, ep_addr);
  return USBD_OK;
}


USBD_StatusTypeDef  USBD_LL_ClearStallEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{

  HAL_PCD_EP_ClrStall(pdev->pData, ep_addr);
  return USBD_OK;
}


uint8_t USBD_LL_IsStallEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
  PCD_HandleTypeDef *hpcd = pdev->pData;

  if((ep_addr & 0x80) == 0x80)
  {
    return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
  }
  else
  {
    return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
  }
}

USBD_StatusTypeDef  USBD_LL_SetUSBAddress (USBD_HandleTypeDef *pdev, uint8_t dev_addr)
{

  HAL_PCD_SetAddress(pdev->pData, dev_addr);
  return USBD_OK;
}

USBD_StatusTypeDef  USBD_LL_Transmit (USBD_HandleTypeDef *pdev,
				      uint8_t  ep_addr,
				      uint8_t  *pbuf,
				      uint16_t  size)
{

  HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size);
  return USBD_OK;
}


USBD_StatusTypeDef  USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev,
					   uint8_t  ep_addr,
					   uint8_t  *pbuf,
					   uint16_t  size)
{

  HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size);
  return USBD_OK;
}


uint32_t USBD_LL_GetRxDataSize  (USBD_HandleTypeDef *pdev, uint8_t  ep_addr)
{
  return HAL_PCD_EP_GetRxCount(pdev->pData, ep_addr);
}


void  USBD_LL_Delay (uint32_t Delay)
{
  HAL_Delay(Delay);
}


void *USBD_static_malloc(uint32_t size)
{
  static uint32_t mem[MAX_STATIC_ALLOC_SIZE];
  return mem;
}


void USBD_static_free(void *p)
{

}


static void SystemClockConfig_Resume(void)
{
	SystemClock_Config();
}

void HAL_PCDEx_SetConnectionState(PCD_HandleTypeDef *hpcd, uint8_t state)
{
  if (state == 1)
  {
   

  }
  else
  {
   
  }

}


