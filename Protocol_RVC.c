#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "crc16.h"
#include "protocol_hal.h"
#pragma pack(1)

#define RVC_SOF     0xAAAA
typedef struct
{
    uint8_t index;
    int16_t yaw;
    int16_t pitch;
    int16_t roll;
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
    uint8_t rsv[3];
    uint8_t crc_sum;        /* from index to rsv*/
} bno080_data_t;


typedef struct
{
	uint16_t sof;
} RVC_ProtocolHeader_t;

typedef  struct
{
	uint16_t sof;
	uint8_t data[0];
}RVC_Protocol_t;

#define PACKET_LEN      (sizeof(bno080_data_t)+sizeof(RVC_ProtocolHeader_t))

#define HEADER_LEN      2

static int32_t unpack(uint8_t *buf, user_pack_t * pack)
{
    RVC_Protocol_t p = (RVC_Protocol_t *)buf;
    pack->sender = 0;
    pack->receiver = 0;
    pack->data = p->data;
    pack->data_len = sizeof(bno080_data_t);
    
    return 0;
}

int32_t get_header_len(uint8_t *buf)
{
    return 2;
}

static int32_t pack(uint8_t *buf,user_pack_t *pack)
{
    RVC_ProtocolHeader_t header;
    header.sof = RVC_SOF;
    memcpy(buf, &header, sizeof(header));
    memcpy(buf+sizeof(RVC_ProtocolHeader_t), pack->data, sizeof(bno080_data_t));
    
    return 0;
}

const protocol_t bno080_rvc = 
{
    PROTOCOL_RVC,
    check_header,
    check_pack,
    get_header_len,
    get_pack_len,
    unpack,
    pack
}

protocol_bno080_rvc_init(void)
{
    protocol_register(&bno080_rvc);
}

