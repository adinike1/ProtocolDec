#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "crc16.h"
#pragma pack(1)

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




#define LINE_BUF_SIZE	300

typedef struct LineBuf
{
	int32_t r_index;
	int32_t w_index;
	int32_t size;
	uint8_t buf[LINE_BUF_SIZE];
}LineBuf_t;

void LineBufInit(LineBuf_t*l, uint32_t LINE_BUF_SIZE)
{
	l->size = LINE_BUF_SIZE;
}

int32_t get_line_buf_available_size(LineBuf_t * l)
{
	return l->size - l->w_index;
}

int32_t get_line_buf_len(LineBuf_t * l)
{
	return (l->w_index - l->r_index);
}

int32_t Movelinebuf(LineBuf_t * l, int32_t offset)
{
	int32_t buf_size = l->w_index - l->r_index;
	// 移动缓冲区 memcpy 如果与地址重叠，会有什么问题
	while (buf_size > 0)
	{
		buf_size--;
		memcpy(l->buf, l->buf + l->r_index, 1);
	}
	l->r_index = 0;
	l->w_index = buf_size;

	return 0;
}

LineBuf_t linebuf;
static uint8_t temp_buf[300];

typedef struct
{
	uint32_t num;
}demo_data_t;

void data_push(cqueue_t *q)
{
	uint32_t pack_len = 0;
	static uint32_t i = 0;
	uint8_t buf[300] = { 0 };
	demo_data_t data;
	data.num = 0;


		pack_len = Protocol_pack(buf, (uint8_t *)&data, sizeof(data));
		cqueue_in(q, buf, pack_len);
}

int32_t Protocol_pack(uint8_t *p_buf, uint8_t *pdata, uint8_t data_len)
{
	uint32_t offset = 0;
	uint16_t header_crc16 = 0;
	uint16_t pack_crc16 = 0;
	static uint32_t seq_num = 0;
	Protocol_t protocol;
	protocol.sof = 0x55;
	protocol.version = 0x01;
	protocol.len = sizeof(protocol) + data_len+ sizeof(pack_crc16);
	protocol.crc16 = crc16((uint8_t *)&protocol, PROTOCOL_HEADR_LEN);
	printf("pack header crc :0x%0x\r\n", protocol.crc16);
	protocol.cmd_set = 0x20;
	protocol.cmd_id = 0x21;
	protocol.seq_num = seq_num++;
	memcpy(p_buf, &protocol, sizeof(protocol));
	offset += sizeof(protocol);
	memcpy(p_buf + offset, pdata, data_len);
	offset += data_len;
	pack_crc16 = crc16(p_buf, offset);
	printf("pack crc:0x%0x\r\n", pack_crc16);
	memcpy(p_buf + offset, &pack_crc16, sizeof(pack_crc16));
	offset += sizeof(pack_crc16);
	for (uint32_t i = 0; i < offset; i++)
	{
		printf("data[%d]:0x%0x\r\n", i, p_buf[i]);
	}
	return offset;
}
#define SOF 0x55
static int32_t pack_len = 0;
int32_t check_header(uint8_t *buf)
{
	uint16_t crc_val = 0;
	ProtocolHeader_t *header = (ProtocolHeader_t *)buf;
	pack_len = header->len;
	memcpy(&crc_val, buf+ PROTOCOL_HEADR_LEN, sizeof(uint16_t));
	printf("dec header crc :0x%0x\r\n", crc_val); 
	if ((header->sof == SOF) && (crc_val == crc16(buf, PROTOCOL_HEADR_LEN)))  //TODO add 头校验
	{
		printf("check header succeed\r\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
int32_t get_pack_len(uint8_t *buf)
{
	return pack_len;
}

int32_t check_pack(uint8_t *buf)
{
	uint16_t crc_val = 0;
	ProtocolHeader_t *header = (ProtocolHeader_t *)buf;
	pack_len = header->len;
	memcpy(&crc_val, buf + get_pack_len(buf) - 2, sizeof(uint16_t));
	printf("dec pack crc :0x%0x\r\n", crc_val);
	if ((crc_val == crc16(buf, get_pack_len(buf) - 2 )))
	{
		printf("check_pack succeed\r\n");
		return 1;
	}
	else
	{
		printf("check_pack failed\r\n");
		return 0;
	}
}
int32_t pack_dec(uint8_t *buf)
{
	Protocol_t * pack = (Protocol_t *)buf;
	printf("pack dec succed,seq_num:%d\r\n", pack->seq_num);

	return 0;
}


void ProtocolDepack(cqueue_t *q)
{
	static uint8_t dec_state = 0;
	while (!cqueue_empty(q))
	{
		int32_t pack_len = 0;
		int32_t real_read_len = 0;
		int32_t need_read_len = (get_line_buf_available_size(&linebuf) < 128) ? get_line_buf_available_size(&linebuf) : 128;
		real_read_len = cqueue_out(q, temp_buf, need_read_len);
		memcpy(linebuf.buf + linebuf.w_index, temp_buf, real_read_len);
		linebuf.w_index += real_read_len;

		while (1)
		{
			if (dec_state == 0)
			{
				if (get_line_buf_len(&linebuf) < PROTOCOL_HEADR_LEN)
				{
					Movelinebuf(&linebuf, linebuf.r_index);
                    break;
				}

				if (check_header(linebuf.buf + linebuf.r_index))
				{
					dec_state = 1;
				}
				else
				{
					linebuf.r_index += 1;
				}
			}
			else if (dec_state == 1)
			{
				if (get_line_buf_len(&linebuf) < get_pack_len(linebuf.buf+linebuf.r_index))
				{
					Movelinebuf(&linebuf, linebuf.r_index);
                    break;
				}
				else
				{
					if (check_pack(linebuf.buf + linebuf.r_index))
					{
						dec_state = 0;
						pack_dec(linebuf.buf + linebuf.r_index);
						linebuf.r_index += get_pack_len(linebuf.buf + linebuf.r_index);
					}
					else
					{
						dec_state = 0;
						linebuf.r_index += 1;
					}
				}
			}
	}
}


#define QUEUE_LEN	1024
uint8_t queue_buf[QUEUE_LEN] = { 0 };
cqueue_t queue;
int main(void)
{
	static uint32_t cnt = 0;
	cqueue_init(&queue, queue_buf, QUEUE_LEN);
	LineBufInit(&linebuf);
	printf("Helo world");
	while (1)
	{
		data_push(&queue);
		data_push(&queue);
		ProtocolDepack(&queue);
	}
	
	return 0;
}

/**
*边界条件:解包到最后只剩下一段，不能完全解析（头没解析完或者数据段没有解析完）
*在数据包中间从插入一些异常数据 如错误对校验码 错误的包头等，包长度等，看是否会存在错解 漏解的情况
*
*/
