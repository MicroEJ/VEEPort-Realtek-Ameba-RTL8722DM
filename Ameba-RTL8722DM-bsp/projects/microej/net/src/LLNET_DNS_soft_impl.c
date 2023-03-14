/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

/**
 * @file
 * @brief LLNET_DNS soft implementation over LWIP.
 * @author MicroEJ Developer Team
 * @version 0.1.2
 * @date 7 July 2020
 */
#include <LLNET_DNS_impl.h>

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include "LLNET_CONSTANTS.h"
#include "LLNET_ERRORS.h"
#include "lwip/dns.h"

extern uint8_t dns_servers_list_updated;

uint8_t LLNET_DNS_IMPL_dnsServersListHasChanged()
{
	uint8_t hasChanged = dns_servers_list_updated;
	if(hasChanged)
	{
		dns_servers_list_updated = 0;
	}
	return hasChanged;
}

int32_t LLNET_DNS_IMPL_getIPv4DNSServersList(int8_t* serversAddr, int32_t offset, int32_t length, uint8_t retry)
{
	int32_t offsetIndex = offset;
	int32_t i = -1;
	int32_t count = 0;
	for(i = -1; ++i < DNS_MAX_SERVERS;)
	{
#if LWIP_VERSION_MAJOR >= 2
        ip_addr_t  dns_addr = *((ip_addr_t *)dns_getserver(i));
#else
        ip_addr_t dns_addr =  dns_getserver(i);
#endif
		// check if the DNS server IP address is right
		if(dns_addr.addr != IPADDR_ANY)
		{
			// copy DNS server IP address
			memcpy(serversAddr + offsetIndex, &dns_addr, sizeof(ip_addr_t));
			offsetIndex += sizeof(ip_addr_t);
			++count;
		}
	}
	return count;
}
