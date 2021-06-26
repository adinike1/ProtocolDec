/*************************************************************************
	> File Name: linebuf.c
	> Author: 
	> Mail: 
	> Created Time: Sat 26 Jun 2021 09:20:45 AM PDT
 ************************************************************************/

#include "linebuf.h"

void LineBufInit(LineBuf_t*l)
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
