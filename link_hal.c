#include "protocol_hal.h"
#include "list.h"
#include "linebuf.h"
#include <pthread.h>
#include <stdlib.h>
#include "link.h"
#include "link_cfg.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void link_dec(link_t *p)
{
	static uint8_t dec_state = 0;
    uint8_t temp_buf[128];
    user_pack_t pack;
    linebuf_t *lb;
	int32_t pack_len = 0;
	int32_t real_read_len = 0;
    do {
        pack_len = 0;
		real_read_len = 0;
		int32_t need_read_len = (get_line_buf_available_size(lb) < 128) ? get_line_buf_available_size(lb) : 128;
		real_read_len = read(p->fd, temp_buf, need_read_len);
        lb = &(p->decbuf);
		memcpy(lb->buf + lb->w_index, temp_buf, real_read_len);
		lb->w_index += real_read_len;

		while (1)
		{
			if (p->dec_state == 0)
                {
				if (get_line_buf_len(lb) < p->prot->get_header_len(lb->buf+lb->r_index))
				{
					move_linebuf(lb, lb->r_index);
					break;
				}

				if (check_header(lb->buf + lb->r_index))
				{
					dec_state = 1;
				}
				else
				{
					lb->r_index += 1;
				}
			}
			else if (p->dec_state == 1)
			{
				if (get_line_buf_len(lb) < p->prot->get_pack_len(lb->buf + lb->r_index))
				{
					move_linebuf(lb, lb->r_index);
					break;
				}
				else
				{
					if (check_pack(lb->buf + lb->r_index))
					{
						dec_state = 0;
						p->prot->unpack(lb->buf + lb->r_index, &pack);
					    lb->r_index += p->prot->get_pack_len(lb->buf + lb->r_index);
					}
					else
					{
						p->dec_state = 0;
						lb->r_index += 1;
					}
				}
			}
	    }
    } while(real_read_len >0);


}

static list_t link_head;;

extern int32_t protocol_bno080_rvc_init(void);
static int32_t link_core_init(void)
{
	list_init(&link_head);
    protocol_bno080_rvc_init();
}

static int32_t link_cfg_init(void)
{
	static link_cfg_t cfg[] = LINK_CFG_TABLE;
	link_t *p;
	link_cfg_t * p_cfg;

	for(uint32_t i = 0; i < sizeof(cfg)/sizeof(cfg[0]); i++) {
		p_cfg = &cfg[i];
		p = (link_t *)malloc(sizeof(link_t));
		p->prot = protocol_find(p_cfg->prot_type);
		p->prio = p_cfg->prio;
		p->dev_name = p_cfg->dev_name;
		p->dec_buf_size = p_cfg->dec_buf_size;

		p->fd = open(p->dev_name, O_RDWR);
		if(p->fd < 0) {
			LINK_ERR("add link <%s>, error\n", p->dev_name);
			continue;
		}

		linebuf_init(&p->decbuf, p->dec_buf_size);

		list_add(&link_head, &(p->node));
	}
}

void *link_dec_task_default(void *args)
{
	link_t *p;
	list_t *head = &link_head;

    do{
        for_each_list_entry(p, head, link_t, node) {
            link_dec(p);
        }
        //TODO how to sleep ms?
        //msleep(5);
    }while(1);
}


static pthread_t link_dec_task_id;

static link_dec_task_init(void)
{
	int s;
	pthread_attr_t attr;
	s = pthread_attr_init(&attr);
	if(s) {
		LINK_ERR("link dec task attr init failed\n");
		return -1;
	}
	s = pthread_create(&link_dec_task_id, &attr, &link_dec_task_default, NULL);
	if(s) {
		LINK_ERR("link dec task init failed\n");
		return -2;
	}
    LINK_INFO("link dec task running...\n");

    pthread_join(&link_dec_task_id, NULL);
}

int32_t link_init(void)
{
	/*
	 * 1.protocol_init
	 * 2.link_cfg_init
	 * 3.link dec task_init
	 */
	link_core_init();
	link_cfg_init();
    link_dec_task_init();
}
