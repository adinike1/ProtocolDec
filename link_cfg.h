/*************************************************************************
	> File Name: link_cfg.h
	> Author: 
	> Mail: 
	> Created Time: Sun 27 Jun 2021 07:14:02 AM PDT
 ************************************************************************/

#ifndef _LINK_CFG_H
#define _LINK_CFG_H
#include "link.h"

#define LINK_BNO080 \
		{PROTOCOL_RVC, "/dev/ttyS0", -1, LINK_PRIO_L, 256}

#define LINK_CFG_TABLE \
		{ \
		 LINK_BNO080, \
		}

#endif
