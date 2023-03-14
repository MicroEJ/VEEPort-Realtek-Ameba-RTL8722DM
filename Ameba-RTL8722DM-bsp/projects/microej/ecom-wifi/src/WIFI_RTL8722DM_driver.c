/*
 * C
 *
 * Copyright 2020-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

/**
 * @file
 * @brief REALTEK AMEBAD specific CCO implementation file for ECOM-WIFI.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 21 September 2021
 */
 
/* Toolchain includes */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "LLECOM_WIFI_impl.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/inet.h"
#include "lwip/dns.h"

#include "wifi_constants.h"
#include "wifi_structures.h"
#include "wifi_conf.h"
#include "wifi_util.h"
#include "wifi_ind.h"

#include "lwip_netconf.h"

// ** Preprocessor definitions **

/**@brief Define this value to enable assert */
#define ENABLE_ASSERT

/**@brief Accessing WiFi funcionality is possible through direct API calls or AT commands.
 *        Direct API calls are more problematic and AT interface is tested and reliable.
 *        Therefore, unless otherwise is required, use AT command interface functions.
 * Warning!!!  Switch disabled as AT commands enable DHCP all the time and brake expected behavior in relation to DHCP
 * */
//#define USE_WIFI_AT_COMMANDS

/**@brief Assert macro */
#ifdef ENABLE_ASSERT
#define WIFI_RTL8722DM_ASSERT(x) do { if (!(x)) { WIFI_RTL8722DM_ASSERT_TRACE("%s, %d\n", __FILE__, __LINE__); while(1); } } while (0)
#else
#define WIFI_RTL8722DM_ASSERT(x) do { if (!(x)) { WIFI_RTL8722DM_ASSERT_TRACE("%s, %d\n", __FILE__, __LINE__); } } while (0)
#endif

/**@brief Log priority levels */
#define WIFI_RTL8722DM_LOG_DEBUG      0
#define WIFI_RTL8722DM_LOG_INFO       1
#define WIFI_RTL8722DM_LOG_WARNING    2
#define WIFI_RTL8722DM_LOG_ERROR      3
#define WIFI_RTL8722DM_LOG_ASSERT     4
#define WIFI_RTL8722DM_LOG_NONE       5

/**@brief Current log level */
//#define WIFI_RTL8722DM_LOG_LEVEL WIFI_RTL8722DM_LOG_WARNING
#define WIFI_RTL8722DM_LOG_LEVEL WIFI_RTL8722DM_LOG_DEBUG

#ifndef WIFI_RTL8722DM_LOG_LEVEL
    #error "WIFI_RTL8722DM_LOG_LEVEL must be defined"
#endif

/**@brief Debug logger */
#if (WIFI_RTL8722DM_LOG_DEBUG >= WIFI_RTL8722DM_LOG_LEVEL)
    #define WIFI_RTL8722DM_DEBUG_TRACE        printf("[rtl8722dm Wifi Driver][DEBUG] ");printf
#else
    #define WIFI_RTL8722DM_DEBUG_TRACE(...)   ((void) 0)
#endif

/**@brief Info logger */
#if (WIFI_RTL8722DM_LOG_INFO >= WIFI_RTL8722DM_LOG_LEVEL)
    #define WIFI_RTL8722DM_INFO_TRACE         printf("[rtl8722dm Wifi Driver][INFO] ");printf
#else
    #define WIFI_RTL8722DM_INFO_TRACE(...)    ((void) 0)
#endif

/**@brief Warning logger */
#if (WIFI_RTL8722DM_LOG_WARNING >= WIFI_RTL8722DM_LOG_LEVEL)
    #define WIFI_RTL8722DM_WARNING_TRACE      printf("[rtl8722dm Wifi Driver][WARNING] ");printf
#else
    #define WIFI_RTL8722DM_WARNING_TRACE(...) ((void) 0)
#endif

/**@brief Error logger */
#if (WIFI_RTL8722DM_LOG_ERROR >= WIFI_RTL8722DM_LOG_LEVEL)
    #define WIFI_RTL8722DM_ERROR_TRACE        printf("[rtl8722dm Wifi Driver][ERROR] ");printf
#else
    #define WIFI_RTL8722DM_ERROR_TRACE(...)   ((void) 0)
#endif

/**@brief Assert logger */
#if (WIFI_RTL8722DM_LOG_ASSERT >= WIFI_RTL8722DM_LOG_LEVEL)
    #define WIFI_RTL8722DM_ASSERT_TRACE       printf("[rtl8722dm Wifi Driver][ASSERT] ");printf
#else
    #define WIFI_RTL8722DM_ASSERT_TRACE(...)  ((void) 0)
#endif

/** @brief Max size of the AP scan buffers */
#define WIFI_RTL8722DM_MAX_SCAN_BUFFERS             32

/** @brief Timeouts */
#define RTL8722DM_DISCONNECT_TIMEOUT_MS             5000
#define RTL8722DM_CONNECT_TIMEOUT_MS                5000
#define RTL8722DM_START_TIMEOUT_MS                  5000
#define RTL8722DM_STOP_TIMEOUT_MS                   5000
#define RTL8722DM_SCAN_TIMEOUT_MS                  10000

/**@brief Security mode */
#define WFM_SECURITY_MODE_OPEN_BITMASK 			0x00
#define WFM_SECURITY_MODE_WEP_BITMASK			0x01
#define WFM_SECURITY_MODE_WPA_PSK_BITMASK		0x42
#define WFM_SECURITY_MODE_WPA_EAP_BITMASK		0x82
#define WFM_SECURITY_MODE_WPA2_PSK_BITMASK		0x44
#define WFM_SECURITY_MODE_WPA2_EAP_BITMASK		0x84
#define WFM_SECURITY_MODE_WPA2_WPA1_PSK_BITMASK	0x46
#define WFM_SECURITY_MODE_WPA2_WPA1_EAP_BITMASK	0x86

/** @brief Max connections that can be made to the Soft AP */
#define WIFI_SOFT_AP_MAX_CONNECTIONS         4

/** @brief Beacon interval for the Soft AP */
#define WIFI_SOFT_AP_BEACON_INTERVAL       100

// ** externs **
extern void fATW0( void *arg);
extern void fATW1( void *arg);
extern void fATW3( void *arg);
extern void fATW4( void *arg);
extern void fATWD( void *arg);
extern void fATWC( void *arg);
extern void fATWA( void *arg);

// ** typedefs **
typedef enum {
    RTL8722DM_MODE_NULL = 0,  /**< null mode */
    RTL8722DM_MODE_STA,       /**< WiFi station mode */
    RTL8722DM_MODE_AP,        /**< WiFi soft-AP mode */
    RTL8722DM_MODE_MAX
}WIFI_RTL8722DM_mode_t;


// ** statics **

/** @brief List of available access points */
static rtw_scan_result_t gpst_available_ap_list[WIFI_RTL8722DM_MAX_SCAN_BUFFERS]; // @TODO: Convert this to dynamic array, possibly to LinkedList.

/** RTL SPECIFIC SECTION **/
static uint8_t numOfAccessPoints = 0;

/* Local variable for DHCP state*/
static int dhcp_state = DHCP_START;

// ** Private funcs

/**
 * @brief this function translates the rtl8722dm security mode  to llnet security mode
 *
 * @param[out] llnet_security_mode output llnet security mode
 * @param[in]  security_mode input security mode  type
 */
static void convert_rtl8722dm_sec_mode_to_llnet_security_mode_f(
    rtw_security_t  security_mode,
	int32_t *           llnet_security_mode)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);

    // We need to map these enum values to the ones in LLECOM_WIFI_impl.h 
    //enum {

    //  RTW_SECURITY_OPEN           = 0,                                                /**< Open security                           */
    //  RTW_SECURITY_WEP_PSK        = WEP_ENABLED,                                      /**< WEP Security with open authentication   */
    //  RTW_SECURITY_WEP_SHARED     = ( WEP_ENABLED | SHARED_ENABLED ),                 /**< WEP Security with shared authentication */
    //  RTW_SECURITY_WPA_TKIP_PSK   = ( WPA_SECURITY  | TKIP_ENABLED ),                 /**< WPA Security with TKIP                  */
    //  RTW_SECURITY_WPA_AES_PSK    = ( WPA_SECURITY  | AES_ENABLED ),                  /**< WPA Security with AES                   */
    //  RTW_SECURITY_WPA2_AES_PSK   = ( WPA2_SECURITY | AES_ENABLED ),                  /**< WPA2 Security with AES                  */
    //  RTW_SECURITY_WPA2_TKIP_PSK  = ( WPA2_SECURITY | TKIP_ENABLED ),                 /**< WPA2 Security with TKIP                 */
    //  RTW_SECURITY_WPA2_MIXED_PSK = ( WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED ),   /**< WPA2 Security with AES & TKIP           */
    //  **deprecated** RTW_SECURITY_WPA_WPA2_MIXED = ( WPA_SECURITY  | WPA2_SECURITY ),                /**< WPA/WPA2 Security                       */
    //  RTW_SECURITY_WPA_WPA2_MIXED_PSK_PSK = ( WPA_SECURITY  | WPA2_SECURITY | TKIP_ENABLED | AES_ENABLED), /**< WPA/WPA2 Security with AES & TKIP      */
    //  RTW_SECURITY_WPA2_AES_CMAC = ( WPA2_SECURITY | AES_CMAC_ENABLED),                /**< WPA2 Security with AES and Management Frame Protection                 */

    // RTW_SECURITY_WPS_OPEN       = WPS_ENABLED,                                      /**< WPS with open security                  */
    // RTW_SECURITY_WPS_SECURE     = (WPS_ENABLED | AES_ENABLED),                      /**< WPS with AES security                   */

    // RTW_SECURITY_WPA3_AES_PSK 	= (WPA3_SECURITY | AES_ENABLED),						/**< WPA3-AES with AES security  */

    // RTW_SECURITY_UNKNOWN        = -1,                                               /**< May be returned by scan function if security is unknown. Do not pass this to the join function! */

    // RTW_SECURITY_FORCE_32_BIT   = 0x7fffffff                                        /**< Exists only to force rtw_security_t type to 32 bits */
    //};


    // #define SECURITY_MODE_ENTERPRISE_NO_SECURITY	    (0)
    // #define SECURITY_MODE_ENTERPRISE_WPA_MIXED		(1)
    // #define SECURITY_MODE_ENTERPRISE_WPA1			(2)
    // #define SECURITY_MODE_ENTERPRISE_WPA2			(3)
    // #define SECURITY_MODE_ENTERPRISE_WEP			    (4)
    // #define SECURITY_MODE_MIXED						(5)	/* MIXED (WPA1 / WPA2) */
    // #define SECURITY_MODE_OPEN						(6) /* Open (no authentification) */
    // #define SECURITY_MODE_WEP128					    (7)
    // #define SECURITY_MODE_WEP64						(8)
    // #define SECURITY_MODE_WPA1						(9)
    // #define SECURITY_MODE_WPA2						(10)
    // #define SECURITY_MODE_UNKNOWN						(255) /* Unknown (for unsupported protocols) */

    switch(security_mode)
	{
	case RTW_SECURITY_OPEN:
		*llnet_security_mode = SECURITY_MODE_OPEN;
		break;
	case RTW_SECURITY_WEP_PSK:
 	case RTW_SECURITY_WEP_SHARED:   
        // @TODO: Check Key size of PSK and SHARED
		*llnet_security_mode = SECURITY_MODE_WEP128;
		break;
	case RTW_SECURITY_WPA_TKIP_PSK:
		*llnet_security_mode = SECURITY_MODE_WPA1;
		break;
	case RTW_SECURITY_WPA_AES_PSK:
		*llnet_security_mode = SECURITY_MODE_WPA1;
		break;
	case RTW_SECURITY_WPA2_AES_PSK:
		*llnet_security_mode = SECURITY_MODE_WPA2;
		break;
	case RTW_SECURITY_WPA2_TKIP_PSK:
		*llnet_security_mode = SECURITY_MODE_WPA2;
		break;
	case RTW_SECURITY_WPA2_AES_CMAC:
		*llnet_security_mode = SECURITY_MODE_WPA2;
		break;
	case RTW_SECURITY_WPA2_MIXED_PSK:
		*llnet_security_mode = SECURITY_MODE_MIXED;
		break;
	case RTW_SECURITY_WPA_WPA2_MIXED_PSK:
		*llnet_security_mode = SECURITY_MODE_MIXED;
	case RTW_SECURITY_WPA3_AES_PSK:
		*llnet_security_mode = SECURITY_MODE_WPA3;
		break;
	case RTW_SECURITY_UNKNOWN:
        default:
          /* Not supported security mode */
          WIFI_RTL8722DM_ERROR_TRACE("Security mode  not supported (0x%x)\n", security_mode);
          *llnet_security_mode = SECURITY_MODE_UNKNOWN;
	}
}

/**
 * @brief this function translates the security modes
 *
 * @param[in] llnet_security_mode - llnet security mode
 * @param[out]  security_type - rtl security type
 */
static void convert_llnet_to_rtl8722dm_security_mode_f(
	int32_t llnet_security_mode,
	rtw_security_t * security_type
    )
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);

    switch (llnet_security_mode)
    {
        case SECURITY_MODE_OPEN:
            *security_type = RTW_SECURITY_OPEN;
        break;
        case SECURITY_MODE_WEP64:
            *security_type = RTW_SECURITY_WEP_PSK;
        break;
        case SECURITY_MODE_WEP128:
            *security_type = RTW_SECURITY_WEP_PSK;
        break;
        case SECURITY_MODE_WPA1:
            *security_type = RTW_SECURITY_WPA_AES_PSK; //Use WPA1 or WPA2
        break;
        case SECURITY_MODE_WPA2:
            *security_type = RTW_SECURITY_WPA2_AES_PSK;
        break;
        case SECURITY_MODE_MIXED:
            *security_type = RTW_SECURITY_WPA_WPA2_MIXED_PSK;
        break;
        case SECURITY_MODE_WPA3:
              *security_type = RTW_SECURITY_WPA3_AES_PSK;
        break;
        case SECURITY_MODE_UNKNOWN:
        default:
            /* Not supported security mode */
            WIFI_RTL8722DM_ERROR_TRACE("Security mode not supported (mode = %d)\n", llnet_security_mode);
            *security_type = RTW_SECURITY_UNKNOWN;
    }
}

/**
 * @brief this function translates the rtl8722dm wps mode to llnet wps mode
 *
 * @param[out] llnet_wps_mode output llnet wps mode
 * @param[in]  wps_mode input wps mode
 *
 * @return true if the wps mode was converted, false otherwise
 */
static bool convert_rtl8722dm_wps_mode_to_llnet_f(
    rtw_wps_type_t  wps_mode,
	int32_t *           llnet_wps_mode)
{
    // Converts from:
    // enum {
    // 	RTW_WPS_TYPE_DEFAULT 		    	= 0x0000,
    // 	RTW_WPS_TYPE_USER_SPECIFIED 		= 0x0001,
    // 	RTW_WPS_TYPE_MACHINE_SPECIFIED   	= 0x0002,
    // 	RTW_WPS_TYPE_REKEY 			        = 0x0003,
    // 	RTW_WPS_TYPE_PUSHBUTTON 		    = 0x0004,
    // 	RTW_WPS_TYPE_REGISTRAR_SPECIFIED 	= 0x0005,
    //  RTW_WPS_TYPE_NONE                   = 0x0006,
    //  RTW_WPS_TYPE_WSC                    = 0x0007
    // };
    
    // to:
    // #define WPS_MODE_NONE						(0) /* No method */
    // #define WPS_MODE_NFC							(1) /* Near Field Communication (NFC) method */
    // #define WPS_MODE_PIN							(2) /* Personal Identification Number (PIN) method */
    // #define WPS_MODE_PUSHBUTTON					(4) /* Push button method */
    // #define WPS_MODE_USB							(8) /* USB method */

   switch(wps_mode)
	{
	case RTW_WPS_TYPE_NONE:
		*llnet_wps_mode = WPS_MODE_NONE;
		break;
	case RTW_WPS_TYPE_PUSHBUTTON:  
		*llnet_wps_mode = WPS_MODE_PUSHBUTTON;
		break;
    default:
        // Some values of RTW could not be translated to llnet.  
        // Revisit these after final hardware design.
        WIFI_RTL8722DM_DEBUG_TRACE("STA WPS mode (0x%x) could not be converted to a valid llnet value, returning NONE!\n", wps_mode);
        *llnet_wps_mode = WPS_MODE_NONE;
        break;
	}
	return true;
}

#if (WIFI_RTL8722DM_LOG_DEBUG >= WIFI_RTL8722DM_LOG_LEVEL)  
static void WIFI_RTL8722DM_print_scan_result( rtw_scan_result_t* record )
{
    WIFI_RTL8722DM_DEBUG_TRACE( "%s\t ", ( record->bss_type == RTW_BSS_TYPE_ADHOC ) ? "Adhoc" : "Infra" );
    WIFI_RTL8722DM_DEBUG_TRACE( MAC_FMT, MAC_ARG(record->BSSID.octet) );
    WIFI_RTL8722DM_DEBUG_TRACE( " %d\t ", record->signal_strength );
    WIFI_RTL8722DM_DEBUG_TRACE( " %d\t  ", record->channel );
    WIFI_RTL8722DM_DEBUG_TRACE( " %d\t  ", record->wps_type );
    WIFI_RTL8722DM_DEBUG_TRACE( "%s(0x%x)\t\t ", ( record->security == RTW_SECURITY_OPEN ) ? "Open" :
                                 ( record->security == RTW_SECURITY_WEP_PSK ) ? "WEP" :
                                 ( record->security == RTW_SECURITY_WPA_TKIP_PSK ) ? "WPA TKIP" :
                                 ( record->security == RTW_SECURITY_WPA_AES_PSK ) ? "WPA AES" :
                                 ( record->security == RTW_SECURITY_WPA2_AES_PSK ) ? "WPA2 AES" :
                                 ( record->security == RTW_SECURITY_WPA2_TKIP_PSK ) ? "WPA2 TKIP" :
                                 ( record->security == RTW_SECURITY_WPA2_MIXED_PSK ) ? "WPA2 Mixed" :
                                 ( record->security == RTW_SECURITY_WPA_WPA2_MIXED_PSK ) ? "WPA/WPA2 AES" :
#ifdef CONFIG_SAE_SUPPORT
								 ( record->security == RTW_SECURITY_WPA3_AES_PSK) ? "WP3-SAE AES" :	
#endif								 
                                 "Unknown",
                                 record->security);

    WIFI_RTL8722DM_DEBUG_TRACE( " %s ", record->SSID.val );
    WIFI_RTL8722DM_DEBUG_TRACE( "\r\n" );
}
#endif

static rtw_result_t WIFI_RTL8722DM_app_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{

	if (malloced_scan_result->scan_complete != RTW_TRUE) 
    {
        // Copy result to stored list.
        if ( numOfAccessPoints < WIFI_RTL8722DM_MAX_SCAN_BUFFERS )
        {
            rtw_scan_result_t* record = &malloced_scan_result->ap_details;
            record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

            WIFI_RTL8722DM_DEBUG_TRACE( "%d\t ", numOfAccessPoints + 1);

            gpst_available_ap_list[numOfAccessPoints]  = *record;
            numOfAccessPoints++;
#if (WIFI_RTL8722DM_LOG_DEBUG >= WIFI_RTL8722DM_LOG_LEVEL)            
		    WIFI_RTL8722DM_print_scan_result(record);
#endif            
        }
        else
        {
           // Optional warning message.
           WIFI_RTL8722DM_DEBUG_TRACE("Ap scan list is full! more than %d wifi network detected\n", WIFI_RTL8722DM_MAX_SCAN_BUFFERS);
        }
	} 
  
	return RTW_SUCCESS;
}

//  ** Public Functions **
bool WIFI_RTL8722DM_initialize_f(void)
{
	WIFI_RTL8722DM_DEBUG_TRACE("(%s)  wifi init.\n", __func__);

	/* WIFI is initialized by SDK. Nothing to do here. */
	return true;
}

bool WIFI_RTL8722DM_start_f(void)
{
	WIFI_RTL8722DM_DEBUG_TRACE("(%s)  wifi start.\n", __func__);

	wifi_on(RTW_MODE_STA);

	return true;
}

bool WIFI_RTL8722DM_stop_f(void){
	WIFI_RTL8722DM_DEBUG_TRACE("(%s)  wifi stop.\n", __func__);

	wifi_off();

	return true;
}

bool WIFI_RTL8722DM_disable_softap_f(void)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s) turning wifi off.\n", __func__);
    wifi_off();

    //back to STA mode
    vTaskDelay(20);
    if (wifi_on(RTW_MODE_STA) < 0){
        WIFI_RTL8722DM_DEBUG_TRACE("\n\rERROR: Wifi on failed!");
        return false;
    }

    return true;
}

bool WIFI_RTL8722DM_enable_softap_f(
    const int8_t      * ssid,
	int32_t             ssid_length,
    const int8_t      * passphrase,
	int32_t             passphrase_length)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s) ssid = %s(len=%d) passphrase=%s(len=%d)\n", __func__, ssid, ssid_length, passphrase, passphrase_length);
    WIFI_RTL8722DM_ASSERT(ssid != NULL);
    WIFI_RTL8722DM_ASSERT(ssid_length > 0);
    WIFI_RTL8722DM_ASSERT(passphrase != NULL);
    // passphrase_length should be 8-63 ASCII characters
    // Ignore passphrase_length, it always returns 64. Use strlen()
    WIFI_RTL8722DM_ASSERT ( (strlen((char *)passphrase) > 7) && ( strlen((char *)passphrase) < 64) );
 
    fATW3((void *)ssid);
    fATW4((void *)passphrase);
    fATWA(NULL);
    return true;
}


bool WIFI_RTL8722DM_set_name_softap_f(const int8_t * name, int32_t name_length)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s) ssid = %s\n", __func__, name);
    WIFI_RTL8722DM_ASSERT(name != NULL);

    // RTL8722DM does not support this function.  PS: "name" is not SSID.
    // Return true to avoid generating exception in the application.
    return true;
}

bool WIFI_RTL8722DM_get_bssid_f(uint8_t * bssid, int32_t bssid_length)
{

    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);
    WIFI_RTL8722DM_ASSERT(bssid != NULL);
    if(wext_get_bssid(WLAN0_NAME, bssid) < 0){
    	return false;
    }
    return true;
}

bool WIFI_RTL8722DM_get_channel_f(int16_t * channel)
{

    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);
    WIFI_RTL8722DM_ASSERT(channel != NULL);
    *channel = 0;
    if(wifi_get_channel((int32_t*)channel) < 0){
    	WIFI_RTL8722DM_ERROR_TRACE("Failed to get the current channel on STA interface\n");
    	return false;
    }
    return true;
}

bool WIFI_RTL8722DM_get_client_state_f(void)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);

    return true;
}


bool WIFI_RTL8722DM_get_rssi_f(float * rssi, int32_t rssi_length)
{

    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);
    WIFI_RTL8722DM_ASSERT(rssi != NULL);
    int32_t temp_rssi = 0;
    if(wifi_get_rssi(&temp_rssi) < 0){
    	WIFI_RTL8722DM_ERROR_TRACE("Failed to retrieve the RSSI value\n");
    	return false;
    }
    *rssi = (float)temp_rssi;
    return true;
}


bool WIFI_RTL8722DM_get_security_mode_f(int16_t * mode)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);
    WIFI_RTL8722DM_ASSERT(mode != NULL);

	rtw_wifi_setting_t setting;
	if(wifi_get_setting(WLAN0_NAME, &setting) < 0){
		WIFI_RTL8722DM_ERROR_TRACE("Failed to get Wi-Fi settings\n");
		return false;
	}
	convert_rtl8722dm_sec_mode_to_llnet_security_mode_f(setting.security_type, (int32_t*)mode);

    return true;
}

bool WIFI_RTL8722DM_get_ssid_f(int8_t * ssid, int32_t ssid_length)
{

    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);
    WIFI_RTL8722DM_ASSERT(ssid != NULL);

    rtw_wifi_setting_t setting;
	if(wifi_get_setting(WLAN0_NAME, &setting) < 0){
		WIFI_RTL8722DM_ERROR_TRACE("Failed to get Wi-Fi settings\n");
		return false;
	}

    /* Store ssid */
    memcpy((void *)ssid, (void *) setting.ssid, (size_t) ssid_length);

    return true;
}

bool WIFI_RTL8722DM_get_wps_modes_f(int16_t * wps_modes)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);
    WIFI_RTL8722DM_ASSERT(wps_modes != NULL);

    /* Not supported by the RTL8722DM APIs */
    *wps_modes = WPS_MODE_NONE;
    return true;
}

bool WIFI_RTL8722DM_join_f(
const int8_t      * ssid,
int32_t             ssid_length,
const int8_t      *passphrase,
int32_t             passphrase_length,
int32_t             security_mode)
{
	int32_t wifi_connect_Result = 0;
	rtw_security_t	security_type;

	WIFI_RTL8722DM_DEBUG_TRACE("(%s) ssid = %s\n", __func__, ssid);
	WIFI_RTL8722DM_ASSERT(ssid != NULL);
	WIFI_RTL8722DM_ASSERT(ssid_length > 0);
	WIFI_RTL8722DM_ASSERT(passphrase != NULL);
	WIFI_RTL8722DM_ASSERT(passphrase_length > 0);
	WIFI_RTL8722DM_ASSERT(security_mode >= 0);

	convert_llnet_to_rtl8722dm_security_mode_f(security_mode, &security_type);

#ifdef USE_WIFI_AT_COMMANDS
	// Disconnect from existing connection first.
	fATWD(NULL);

#if CONFIG_AUTO_RECONNECT
	// And, disable autoreconnect, otherwise it will try to reconnect to the values stored in flash.
	wifi_set_autoreconnect(0);  
#endif

	fATW0((void *)ssid);
	fATW1((void *)passphrase);
	fATWC(NULL);

	wifi_connect_Result = wifi_is_connected_to_ap();

#if CONFIG_AUTO_RECONNECT
	//Re-enable autoreconnect.
	wifi_set_autoreconnect(1);
#endif 
#else

#ifdef CONFIG_AUTO_RECONNECT       
	// And, disable autoreconnect, otherwise it will try to reconnect to the values stored in flash.
	wifi_set_autoreconnect(0);  
#endif

	// Disconnect from pre-existing connection.
	wifi_disconnect();
	vTaskDelay(2000);

	wifi_connect_Result = wifi_connect( (char *)ssid,
			security_type,
			(char *)passphrase,
			strlen((char const *)ssid),
			strlen((char const *)passphrase),
			-1,  // key_id: The index of the wep key (0, 1, 2, or 3). If not using it, leave it with value -1.
			NULL);
#if CONFIG_AUTO_RECONNECT
	//Re-enable autoreconnect.
	wifi_set_autoreconnect(1);
#endif 

	LwIP_DHCP(0, dhcp_state);


#endif

	return ( wifi_connect_Result == RTW_SUCCESS ); 

}

bool WIFI_RTL8722DM_leave_f(void)
{
    bool returnStatus = true;

    WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);

#ifdef USE_WIFI_AT_COMMANDS
    fATWD(NULL);  // Process disconnect AT command.
    returnStatus = ( wifi_is_connected_to_ap() != RTW_SUCCESS );
#else
    wifi_disconnect();
#endif

#if CONFIG_AUTO_RECONNECT
    //Disable autoreconnect, otherwise it will try to reconnect to the values stored in flash.
    wifi_set_autoreconnect(0);  
#endif

    return ( returnStatus );
}


bool WIFI_RTL8722DM_get_ap_count_f(int16_t * const ap_count, int8_t active)
{
	WIFI_RTL8722DM_DEBUG_TRACE("(%s) ENTER\n", __func__);
    WIFI_RTL8722DM_ASSERT(ap_count != NULL);

    /* Clear buffers */
    memset((void *) &gpst_available_ap_list, 0, sizeof(gpst_available_ap_list));
    numOfAccessPoints = 0;
    /*
     * Default Scan type is Active
     * In case of Passive Scan request we return error
     * */
    if (active == 0){
    	return false;
    }
	if(wifi_scan_networks(WIFI_RTL8722DM_app_scan_result_handler, NULL ) != RTW_SUCCESS){
		WIFI_RTL8722DM_DEBUG_TRACE("\n\r(%s) ERROR: wifi scan failed", __func__);
	}
    else
    {
        /*  This info is taken from sec 20.2.2 Scan APIs of AN0400 AmebaD Applilcation Node 

            The scan progressively accumulates results over time, and may take 1 ~ 6 seconds to complete. The results of the scan will be individually provided to the callback function.
            The callback function will be executed in the context of the RTW thread.
            When scanning specific channels, devices with a strong signal strength on nearby channels may be detected.

            Therefore, wait for 8 secs till all scan callbacks are completed. */
            
        // @TODO: Convert this wait mechanism to Semaphore.
        vTaskDelay(10000);

        *ap_count = (int16_t) (numOfAccessPoints);
    }
    
	WIFI_RTL8722DM_DEBUG_TRACE("(%s) EXIT numOfAccessPoints=%d\n", __func__, numOfAccessPoints);
 
    return true;
}

bool WIFI_RTL8722DM_get_ap_ssid_f(uint32_t index, uint8_t * ssid, int32_t ssid_length)
{
	WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);
    bool b_return = true;

    WIFI_RTL8722DM_ASSERT(ssid != NULL);

    if ( ( index < numOfAccessPoints ) && (ssid_length > 0) )
    {
        if ( gpst_available_ap_list[index].SSID.len > 0 )
        {
          strncpy ( (char *)ssid, (const char *) gpst_available_ap_list[index].SSID.val, ssid_length );
        }
    }
    else
    {
        b_return = false;
    }

    return b_return;
}

bool WIFI_RTL8722DM_get_ap_bssid_f(uint32_t index, uint8_t * bssid, int32_t bssid_length)
{

    bool b_return = true;

    WIFI_RTL8722DM_ASSERT(bssid != NULL);

    if ( (index < numOfAccessPoints ) && (bssid_length > 0) )
    {
    	int32_t copy_length;

        if ((uint32_t) bssid_length < sizeof(gpst_available_ap_list[index].BSSID.octet))
        {
            copy_length = bssid_length;
        }
        else
        {
            copy_length = sizeof ( gpst_available_ap_list[index].BSSID.octet );
        }

	    WIFI_RTL8722DM_DEBUG_TRACE("(%s) index=%d bssid_len=%d copy_len=%d\n", __func__, index, bssid_length, copy_length);

        memcpy((int8_t *) bssid,
                (int8_t *) &gpst_available_ap_list[index].BSSID.octet[0],
                (size_t) copy_length);
    }
    else
    {
        b_return = false;
    }

    return b_return;
}

bool WIFI_RTL8722DM_get_ap_channel_f(uint32_t index, int32_t * channel)
{
	WIFI_RTL8722DM_DEBUG_TRACE("(%s) index = %d\n", __func__, index);
	bool b_return = true;

    WIFI_RTL8722DM_ASSERT(channel != NULL);

    if ( index < numOfAccessPoints )
    {
        *channel = (int32_t) gpst_available_ap_list[index].channel;
    }
    else
    {
        b_return = false;
    }

    return b_return;
}

bool WIFI_RTL8722DM_get_ap_rssi_f(uint32_t index, float * rssi, int32_t rssi_length)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s) index = %d\n", __func__, index);
    bool b_return = true;

    WIFI_RTL8722DM_ASSERT(rssi != NULL);

    if ( ( index < numOfAccessPoints ) && (rssi_length > 0) )
    {
        *rssi = ( float) gpst_available_ap_list[index].signal_strength;
    }
    else
    {
        b_return = false;
    }

    return b_return;
}

bool WIFI_RTL8722DM_get_ap_security_mode_f(uint32_t index, int32_t * mode)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s) index = %d\n", __func__, index);
    bool b_return = true;

    WIFI_RTL8722DM_ASSERT(mode != NULL);

    if ( index < numOfAccessPoints )
    {
		convert_rtl8722dm_sec_mode_to_llnet_security_mode_f(gpst_available_ap_list[index].security, mode);
    }
    else
    {
        b_return = false;
    }

    return b_return;
}

bool WIFI_RTL8722DM_get_ap_wps_modes_f(uint32_t index, int32_t * wps_cap)
{
    WIFI_RTL8722DM_DEBUG_TRACE("(%s) index = %d\n", __func__, index);
    bool b_return = true;

    WIFI_RTL8722DM_ASSERT(wps_cap != NULL);

    if ( index < numOfAccessPoints )
    {
		if( convert_rtl8722dm_wps_mode_to_llnet_f( gpst_available_ap_list[index].wps_type,
                                                   wps_cap) == false )
		{
			b_return = false;
		}
    }
    else
    {
        b_return = false;
    }

    return b_return;
}

bool WIFI_RTL8722DM_interface_is_started(struct netif * interface)
{
	WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);
	(void)interface;

	return (bool)wifi_is_up(RTW_STA_INTERFACE);
}

bool WIFI_RTL8722DM_apply_configuration(struct netif* interface, bool is_static, ip_addr_t ip, ip_addr_t netmask, ip_addr_t gw)
{
	WIFI_RTL8722DM_DEBUG_TRACE("(%s)\n", __func__);
	(void)interface;
	(void)ip;
	(void)netmask;
	(void)gw;

	if (is_static == true ) {
		dhcp_state = DHCP_STOP;
	} else {
		dhcp_state = DHCP_START;
	}
        return 1;
}

