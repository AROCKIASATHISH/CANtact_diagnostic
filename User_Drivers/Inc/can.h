#ifndef _CAN_H
#define _CAN_H

enum can_bitrate {
    CAN_BITRATE_10K,
    CAN_BITRATE_20K,
    CAN_BITRATE_50K,
    CAN_BITRATE_100K,
    CAN_BITRATE_125K,
    CAN_BITRATE_250K,
    CAN_BITRATE_500K,
    CAN_BITRATE_750K,
    CAN_BITRATE_1000K,
};

enum can_bus_state {
    OFF_BUS,
    ON_BUS
};

void can_init(void);
void can_enable(void);
void can_disable(void);
void can_set_bitrate(enum can_bitrate bitrate);
void can_set_silent(uint8_t silent);
uint32_t can_tx(CanTxMsgTypeDef *tx_msg, uint32_t timeout);
uint32_t can_rx(CanRxMsgTypeDef *rx_msg, uint32_t timeout);
uint8_t is_can_msg_pending(uint8_t fifo);
void can_set_filter(uint32_t id, uint32_t mask);
void can_tx_packet(uint8_t *txdata ,uint8_t length);
uint32_t can_rx_packet(uint8_t* rx_buf);
void can_set_packet_type(uint32_t ptype);
void can_set_packet_id(uint32_t id , uint32_t idtype);
void MX_CAN_INIT();
void CAN_Rx_Int_Enable();
uint8_t CAN_Rx_IT_on();

#endif // _CAN_H
