#ifndef __PROTOCOL_HAL_H__
#define __PROTOCOL_HAL_H__
#include "protocol.h"
typedef enum
{
    PROTOCOL_RVC;
    PROTOCOL_MAX;
} protocol_type_e;

typedef struct
{
    uint16_t protocol_type;
    uint16_t sender;
    uint16_t receiver;
    uint32_t seq_id;
    uint32_t session_id;
    void *data;
    uint32_t data_len;
} user_pack_t;

tyepdef struct
{
    protocol_type_e type;
    int32_t check_header(uint8_t *buf);
    int32_t check_pack(uint8_t *buf);
    int32_t get_header_len(uint8_t *buf);
    int32_t get_pack_len(uint8_t *buf);
    int32_t unpack(uint8_t *buf, user_pack_t * pack);
    int32_t pack(uint8_t *buf,user_pack_t *pack);
} protocol_t;


extern protocol_t *protocol[PROTOCOL_MAX];
int32_t protocol_register(protocol_t *p);
#endif
