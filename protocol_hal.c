#include "protocol_hal.h"

int32_t protocol_register(protocol_t *p)
{
    protocol[p->type] = p; 
}

protocol_t *protocol_get(protocol_type_e type)
{
    return protocol[i];
}

void link_dec(cqueue_t *q)
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


