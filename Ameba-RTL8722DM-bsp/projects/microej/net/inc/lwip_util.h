/*
 * C
 *
 * Copyright 2017-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#ifndef __LWIP_UTIL_H
#define __LWIP_UTIL_H

/**
 * @file
 * @brief LLNET utility functions for LWIP.
 * @author MicroEJ Developer Team
 * @version 0.2.0
 * @date 19 June 2020
 */

#include <stdint.h>
#include <lwip/netif.h>

extern void ethernetif_update_config(struct netif *netif);
extern void ethernetif_set_link(void const *argument);

void llnet_lwip_notify_conn_changed(struct netif *netif);
int32_t llnet_lwip_init(void);

#endif // __LWIP_UTIL_H
