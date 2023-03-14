/*
 * C
 *
 * Copyright 2018-2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#ifndef WIFI_RTL8722DM_DRIVER_H
#define WIFI_RTL8722DM_DRIVER_H

/**
 * @file
 * @brief REALTEK AMEBAD specific CCO header file for ECOM-WIFI.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 21 September 2021
 */

#include <stdbool.h>
#include <stdint.h>
#include "lwip/inet.h"
#include "lwip/ip_addr.h"

/**
 * @brief function to initialize the wifi module
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_initialize_f(void);

/**
 * @brief this function start the wifi module
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_start_f(void);

/**
 * @brief this function stop the wifi module
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_stop_f(void);

/**
 * @brief this function joins a WiFi network
 *
 * @param[in] ssid ssid of the network
 * @param[in] ssid_length ssid length
 * @param[in] passphrase password needed for network connection
 * @param[in] passphrase_length password length
 * @param[in] security_mode security mode
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_join_f(const int8_t * ssid, int32_t ssid_length, const int8_t * passphrase, int32_t passphrase_length, int32_t security_mode);

/**
 * @brief this function leaves a WiFi network
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_leave_f(void);

/**
 * @brief this function returns the bssid of the WiFi network
 *
 * @param[out] bssid the buffer to be filled with the bssid
 * @param[in]  bssid_length length to be filled in the buffer
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_bssid_f(uint8_t * bssid, int32_t bssid_length);

/**
 * @brief this function returns the channel of the WiFi network
 *
 * @param[out] channel the buffer to be filled
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_channel_f(int16_t * channel);

/**
 * @brief this function returns the rssi of the WiFi network
 *
 * @param[out] rssi the buffer to be filled
 * @param[in]  rssi_length length to be filled in the buffer
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_rssi_f(float * rssi, int32_t rssi_length);

/**
 * @brief this function returns the security mode of the WiFi network
 *
 * @param[out] mode the buffer to be filled
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_security_mode_f(int16_t * mode);

/**
 * @brief this function returns the ssid of the WiFi network
 *
 * @param[out] ssid the buffer to be filled
 * @param[in]  ssid_length length to be filled in the buffer
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_ssid_f(int8_t * ssid, int32_t ssid_length);

/**
 * @brief this function returns the WPS modes of the WiFi network
 *
 * @param[out] wps_modes the buffer to be filled
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_wps_modes_f(int16_t * wps_modes);

/**
 * @brief this function enables the SoftAP for the module
 *
 * @param[in] ssid the ssid of the SoftAP
 * @param[in] ssid_length the ssid length
 * @param[in] passphrase the password of the SoftAP
 * @param[in] passphrase_length the password length
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_enable_softap_f(const int8_t * ssid, int32_t ssid_length, const int8_t * passphrase, int32_t passphrase_length);

/**
 * @brief this function changes the name of the SoftAP
 *
 * @param[in] name the ssid of the SoftAP
 * @param[in] name_length the ssid length
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_set_name_softap_f(const int8_t * name, int32_t name_length);

/**
 * @brief this function disables the SoftAP
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_disable_softap_f(void);

/**
 * @brief this function returns the access point count discovered
 *
 * @param[out] ap_count the access point count to be returned
 *
 * @param[in] scan_type the scan type, active or passive
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_ap_count_f(int16_t * const ap_count, int8_t active);

/**
 * @brief this function returns the ssid from the saved list
 *
 * @param[in]  index the index from the saved list
 * @param[out] ssid the buffer to be returned
 * @param[in]  ssid_length the length to copy in the return buffer
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_ap_ssid_f(uint32_t index, uint8_t * ssid, int32_t ssid_length);

/**
 * @brief this function returns the bssid from the saved list
 *
 * @param[in]  index the index from the saved list
 * @param[out] bssid the buffer to be returned
 * @param[in]  bssid_length the length to copy in the return buffer
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_ap_bssid_f(uint32_t index, uint8_t * bssid, int32_t bssid_length);

/**
 * @brief this function returns the channel from the saved list
 *
 * @param[in]  index the index from the saved list
 * @param[out] channel the buffer to be returned
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_ap_channel_f(uint32_t index, int32_t * channel);

/**
 * @brief this function returns the rssi from the saved list
 *
 * @param[in]  index the index from the saved list
 * @param[out] rssi the buffer to be returned
 * @param[in]  rssi_length the length to copy in the return buffer
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_ap_rssi_f(uint32_t index, float * rssi, int32_t rssi_length);

/**
 * @brief this function returns the security mode from the saved list
 *
 * @param[in]  index the index from the saved list
 * @param[out] mode the buffer to be returned
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_ap_security_mode_f(uint32_t index, int32_t * mode);

/**
 * @brief this function returns the wps mode from the saved list
 *
 * @param[in]  index the index from the saved list
 * @param[out] wps_cap the buffer to be returned
 *
 * @return true if success else false
 */
bool WIFI_RTL8722DM_get_ap_wps_modes_f(uint32_t index, int32_t * wps_cap);

/**
 * @brief this function returns the client state
 *
 * @return true if the client is started else false
 */
bool WIFI_RTL8722DM_get_client_state_f(void);

/**
 * @brief this function will return the running state of the network interface
 * @return true in case interface is running, false if not
 */
bool WIFI_RTL8722DM_interface_is_started(struct netif * interface);

/**
 * @brief this function allow WIFI driver to perform additional tasks needed after an IP configuration
 * was define din ECOM_NETWORK. DHCP need to be started and stopped in our case.
 * @return No return
 */
void WIFI_RTL8722DM_apply_configuration(struct netif* interface, bool is_static, ip_addr_t ip, ip_addr_t netmask, ip_addr_t gw);

#endif /* WIFI_RTL8722DM_DRIVER_H */
