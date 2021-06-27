#include "protocol_hal.h"
#include "list.h"
#include "linebuf.h"
#include <pthread.h>
#include <stdlib.h>

static protocol_t *protocol[PROTOCOL_MAX];

int32_t protocol_register(protocol_t *p)
{
	protocol[p->type] = p;
}
protocol_t *protocol_find(protocol_type_e type)
{
    for(uint32_t i = 0;i<sizeof(protocol)/sizeof(protocol[0]); i++) {
        protocol_t *p = protocol[i];
        if(p->type == type)
            return p;
    }

    return NULL;
}
