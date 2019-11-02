#include "stm32f0xx_hal.h"
#include "can.h"


CAN_HandleTypeDef hcan;
CAN_FilterConfTypeDef filter;
uint32_t prescaler;
enum can_bus_state bus_state;
CanTxMsgTypeDef txframe;
CanRxMsgTypeDef rx_msg;

void MX_CAN_INIT()
{
    /**CAN GPIO Configuration
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX
    */
    GPIO_InitTypeDef GPIO_InitStruct;
    __CAN_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_CAN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    bus_state = OFF_BUS;
    hcan.Instance = CAN;
    can_set_bitrate(CAN_BITRATE_500K);
    can_set_silent(0);
    can_enable();
    bus_state = ON_BUS;
    txframe.RTR = CAN_RTR_DATA;
    txframe.IDE = CAN_ID_STD;
    txframe.StdId = 0x123;
    txframe.ExtId = 0x01;
}

void can_set_filter(uint32_t id, uint32_t mask) {
    // see page 825 of RM0091 for details on filters
    // set the standard ID part
    filter.FilterIdHigh = (id & 0x7FF) << 5;
    // add the top 5 bits of the extended ID
    filter.FilterIdHigh += (id >> 24) & 0xFFFF;
    // set the low part to the remaining extended ID bits
    filter.FilterIdLow += ((id & 0x1FFFF800) << 3);

    // set the standard ID part
    filter.FilterMaskIdHigh = (mask & 0x7FF) << 5;
    // add the top 5 bits of the extended ID
    filter.FilterMaskIdHigh += (mask >> 24) & 0xFFFF;
    // set the low part to the remaining extended ID bits
    filter.FilterMaskIdLow += ((mask & 0x1FFFF800) << 3);

    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterNumber = 0;
    filter.FilterFIFOAssignment = CAN_FIFO0;
    filter.BankNumber = 0;
    filter.FilterActivation = ENABLE;

    if (bus_state == ON_BUS) {
	HAL_CAN_ConfigFilter(&hcan, &filter);
    }
}

void can_enable(void) {
    if (bus_state == OFF_BUS) {
	hcan.Init.Prescaler = prescaler;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SJW = CAN_SJW_1TQ;
	hcan.Init.BS1 = CAN_BS1_4TQ;
	hcan.Init.BS2 = CAN_BS2_3TQ;
	hcan.Init.TTCM = DISABLE;
	hcan.Init.ABOM = ENABLE;
	hcan.Init.AWUM = DISABLE;
	hcan.Init.NART = DISABLE;
	hcan.Init.RFLM = DISABLE;
	hcan.Init.TXFP = ENABLE;
        hcan.pTxMsg = NULL;
        HAL_CAN_Init(&hcan);
        bus_state = ON_BUS;
	can_set_filter(0, 0);
    }
}


void can_set_packet_type(uint32_t ptype)
{
 txframe.RTR = ptype ;   /* CAN_RTR_DATA      Data frame */
                         /* CAN_RTR_REMOTE    Remote frame */
}


/* CAN_ID_STD   Standard Id */
/* CAN_ID_EXT  Extended Id */
void can_set_packet_id(uint32_t id , uint32_t idtype)
{
    if (idtype == CAN_ID_STD) {
        txframe.IDE = CAN_ID_STD;
        txframe.StdId = id;
    } else if (idtype == CAN_ID_EXT ) {
        txframe.IDE = CAN_ID_EXT;
        txframe.ExtId = id;
    }
}


void can_disable(void) {
    if (bus_state == ON_BUS) {
        // do a bxCAN reset (set RESET bit to 1)
        hcan.Instance->MCR |= CAN_MCR_RESET;
        bus_state = OFF_BUS;
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

void can_set_bitrate(enum can_bitrate bitrate) {
    if (bus_state == ON_BUS) {
        // cannot set bitrate while on bus
        return;
    }

    switch (bitrate) {
    case CAN_BITRATE_10K:
	prescaler = 600;
        break;
    case CAN_BITRATE_20K:
	prescaler = 300;
        break;
    case CAN_BITRATE_50K:
	prescaler = 120;
        break;
    case CAN_BITRATE_100K:
        prescaler = 60;
        break;
    case CAN_BITRATE_125K:
        prescaler = 48;
        break;
    case CAN_BITRATE_250K:
        prescaler = 24;
        break;
    case CAN_BITRATE_500K:
        prescaler = 12;
        break;
    case CAN_BITRATE_750K:
        prescaler = 8;
        break;
    case CAN_BITRATE_1000K:
        prescaler = 6;
        break;
    }
}

void can_set_silent(uint8_t silent) {
    if (bus_state == ON_BUS) {
        // cannot set silent mode while on bus
        return;
    }
    if (silent) {
        hcan.Init.Mode = CAN_MODE_SILENT;
    } else {
        hcan.Init.Mode = CAN_MODE_NORMAL;
    }
}

uint32_t can_tx(CanTxMsgTypeDef *tx_msg, uint32_t timeout) {
    uint32_t status;
    // transmit can frame
    hcan.pTxMsg = tx_msg;
    status = HAL_CAN_Transmit(&hcan, timeout);
    return status;
}

uint32_t can_rx(CanRxMsgTypeDef *rx_msg, uint32_t timeout) {
    uint32_t status;
    hcan.pRxMsg = rx_msg;
    status = HAL_CAN_Receive(&hcan, CAN_FIFO0, timeout);
    return status;
}

uint8_t is_can_msg_pending(uint8_t fifo) {
    if (bus_state == OFF_BUS) {
        return 0;
    }
    return (__HAL_CAN_MSG_PENDING(&hcan, fifo) > 0);
}



void can_tx_packet(uint8_t *txdata ,uint8_t length )
{
    uint8_t j;
    txframe.DLC = length;
    for (j = 0; j < txframe.DLC; j++) {
        txframe.Data[j] = txdata[j];
    }
    can_tx(&txframe, 10);
}


uint32_t can_rx_packet(uint8_t* rx_buf)
{
    uint8_t j;
    uint32_t status;
    status = can_rx(&rx_msg, 10);
    if (status == HAL_OK) {
        for (j = 0; j < rx_msg.DLC; j++) {
            rx_buf[j] = rx_msg.Data[j] ;
        }
    }
    return status;
}



void CAN_Rx_Int_Enable()
{
    HAL_NVIC_SetPriority(CEC_CAN_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CEC_CAN_IRQn);
    __HAL_CAN_ENABLE_IT(&hcan ,CAN_IT_FMP0);
    //__HAL_CAN_ENABLE_IT(&hcan ,CAN_IT_FF0);
    // __HAL_CAN_ENABLE_IT(&hcan ,CAN_IT_FOV0); 
    CAN_Rx_IT_on();
}

uint8_t CAN_Rx_IT_on()
{
    HAL_CAN_Receive_IT(&hcan, CAN_FIFO0);
}


