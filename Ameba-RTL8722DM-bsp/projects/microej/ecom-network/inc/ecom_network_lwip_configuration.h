/*
 * C
 *
 * Copyright 2020-2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 *
 */

#ifndef  ECOM_NETWORK_LWIP_CONFIGURATION_H
#define  ECOM_NETWORK_LWIP_CONFIGURATION_H

/**
 * @file
 * @brief LLECOM_NETWORK over LWIP configuration.
 * @author MicroEJ Developer Team
 * @version 1.1.1
 * @date 21 April 2021
 */

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Compatibility sanity check value.
 * This define value is checked in the implementation to validate that the version of this configuration
 * is compatible with the implementation.
 *
 * This value must not be changed by the user of the CCO.
 * This value must be incremented by the implementor of the CCO when a configuration define is added, deleted or modified.
 */
#define ECOM_NETWORK_LWIP_CONFIGURATION_VERSION (1)

/**
 * @brief Provide here LLECOM_NETWORK_start and LLECOM_NETWORK_stop functions
 * from interface driver which will allow to start and stop the interface
 * Example:
 *          bool MY_WIFI_DRIVER_start_f(void);
 *          bool MY_WIFI_DRIVER_stop_f(void);
 * 			#define LLECOM_NETWORK_start MY_WIFI_DRIVER_start_f
 * 			#define LLECOM_NETWORK_stop MY_WIFI_DRIVER_stop_f
 * Remove below line after once you define LLECOM_NETWORK_start and LLECOM_NETWORK_stop functions.
 */
//#error "Define here the LLECOM_NETWORK_start and LLECOM_NETWORK_stop functions for the network interface used (Wifi, Modem, Eth...) "
#define LLECOM_NETWORK_start WIFI_RTL8722DM_start_f
#define LLECOM_NETWORK_stop WIFI_RTL8722DM_stop_f

/**
 * @brief Provide here LLECOM_NETWORK_apply_configuration function
 * from interface driver side which will handle additional needed steps after an IP configuration was defined.
 * An example can be to handle start/stop of DHCP client.
 *
 * Example:
 *          bool MY_WIFI_DRIVER_apply_configuration_f(struct netif* interface, bool is_static, ip_addr_t ip, ip_addr_t netmask, ip_addr_t gw);
 * 			#define LLECOM_NETWORK_apply_configuration  MY_WIFI_DRIVER_apply_configuration_f
 * 	Remove below line after once you define LLECOM_NETWORK_apply_configuration function.
 */
//#error "Define here LLECOM_NETWORK_apply_configuration function to complete IP configuration process"
#define LLECOM_NETWORK_apply_configuration WIFI_RTL8722DM_apply_configuration

/**
 * @brief Provide here LLECOM_NETWORK_interface_is_started function
 * from interface driver side which will return the running state of the network interface.
 *
 * Example:
 *          bool MY_WIFI_DRIVER_interface_is_started_f(struct netif * interface);
 * 			#define LLECOM_NETWORK_interface_is_started  MY_WIFI_DRIVER_interface_is_started_f
 * 	Remove below line after once you define LLECOM_NETWORK_interface_is_started function.
 */
//#error "Define here LLECOM_NETWORK_interface_is_started function to provide access to interface running status"
#define LLECOM_NETWORK_interface_is_started	WIFI_RTL8722DM_interface_is_started

/**
 * @brief Set this define to print debug traces.
 */
//#define	LLECOM_NETWORK_DEBUG

#ifdef LLECOM_NETWORK_DEBUG
#include <stdio.h>
#define LLECOM_NETWORK_DEBUG_TRACE printf("[DEBUG] ");printf
#else
#define LLECOM_NETWORK_DEBUG_TRACE(...) ((void) 0)
#endif

#ifdef __cplusplus
	}
#endif

#endif // ECOM_NETWORK_LWIP_CONFIGURATION_H
