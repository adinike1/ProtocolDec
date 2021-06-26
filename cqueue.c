#include "cqueue.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int32_t cqueue_init(cqueue_t *q, uint8_t *buf, uint32_t len)
{
	q->buf = buf;
	q->len = len;
	q->w_index = 0;
	q->r_index = 0;

	return 0;
}

int32_t cqueue_in(cqueue_t *q, uint8_t *buf, uint32_t len)
{
	uint32_t write_size = (q->len - 1 - cqueue_used(q)) > len ? len : (q->len - 1 - cqueue_used(q));

	if (cqueue_full(q))
	{
		return 0;
	}

	uint32_t cpy_len = 0;
	cpy_len = write_size < (q->len - q->w_index) ? write_size : (q->len - q->w_index);
	memcpy(q->buf + q->w_index, buf, cpy_len);
	memcpy(q->buf, buf+ cpy_len,  write_size- cpy_len);
	q->w_index += write_size;
	q->w_index %= q->len;

	return write_size;
}

int32_t cqueue_out(cqueue_t *q, uint8_t *buf, uint32_t len)
{
	uint32_t read_size = (cqueue_used(q) > len) ? len : (cqueue_used(q));

	if (cqueue_empty(q))
	{
		return 0;
	}

	uint32_t cpy_len = 0;
	cpy_len = (q->len - q->r_index) > read_size ? read_size : (q->len - q->r_index);
	memcpy(buf, q->buf + q->r_index, cpy_len);
	memcpy(buf + cpy_len, q->buf, read_size - cpy_len);

	q->r_index += read_size;
	q->r_index %= q->len;

	return read_size;
}


int32_t cqueue_empty(cqueue_t *q)
{
	if (q->w_index == q->r_index)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int32_t cqueue_full(cqueue_t *q)
{
	if (((q->w_index + 1) % q->len) == q->r_index)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


uint32_t cqueue_used(cqueue_t *q)
{
	if (q->w_index > q->r_index)
	{
		return q->w_index - q->r_index;
	}
	else
	{
		return q->w_index + q->len - q->r_index;
	}
}
