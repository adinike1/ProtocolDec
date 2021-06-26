#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
	uint8_t *buf;
	uint32_t len;
	uint32_t r_index;
	uint32_t w_index;
}cqueue_t;

uint32_t cqueue_used(cqueue_t *q);
int32_t cqueue_full(cqueue_t *q);
int32_t cqueue_empty(cqueue_t *q);
int32_t cqueue_out(cqueue_t *q, uint8_t *buf, uint32_t len);
int32_t Protocol_pack(uint8_t *p_buf, uint8_t *pdata, uint8_t data_len);
