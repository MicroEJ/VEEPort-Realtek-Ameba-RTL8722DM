/*
 * Automatically generated by make menuconfig: don't edit
 */
#ifndef _PLATFORM_AUTOCONF_H_
#define _PLATFORM_AUTOCONF_H_
#define AUTOCONF_INCLUDED

/*
 * < MENUCONFIG FOR CHIP CONFIG
 */

/*
 * < CONFIG CHIP
 */
#define CONFIG_RTL8721D 1
#define ARM_CORE_CM4 1
#undef  CONFIG_FPGA
#define CONFIG_CHIP_A_CUT 1
#undef  CONFIG_CHIP_B_CUT

/*
 * < CONFIG CPU CLK 
 */
#define CONFIG_CPU_CLK 1
#define CONFIG_CPU_200MHZ 1
#undef  CONFIG_CPU_100MHZ
#undef  CONFIG_CPU_50MHZ
#undef  CONFIG_CPU_25MHZ
#undef  CONFIG_CPU_XTAL
#undef  CONFIG_FPGA_CLK
#define PLATFORM_CLOCK (200000000)
#define CPU_CLOCK_SEL_VALUE (0)

/*
 * < CONFIG TEST MODE 
 */
#undef  CONFIG_MP
#undef  CONFIG_CP
#undef  CONFIG_FT
#undef  CONFIG_EQC

/*
 * < CONFIG TrustZone 
 */
#undef  CONFIG_TRUSTZONE

/*
 * < CONFIG SBootIMG2 
 */
#undef  CONFIG_SBootIMG2

/*
 * < CONFIG OS 
 */
#define CONFIG_KERNEL 1
#define PLATFORM_FREERTOS 1
#define TASK_SCHEDULER_DISABLED (0)

/*
 * < CONFIG SOC PS 
 */
#define CONFIG_SOC_PS_EN 1
#define CONFIG_SOC_PS_MODULE 1

/*
 * < CONFIG SDIO Device 
 */
#define CONFIG_SDIO_DEVICE_EN 1
#define CONFIG_SDIO_DEVICE_NORMAL 1
#define CONFIG_SDIO_DEVICE_MODULE 1

/*
 * < CONFIG USB 
 */
#undef  CONFIG_USB_OTG_EN

/*
 * < CONFIG PINMUX 
 */
#undef  CONFIG_PINMAP_ENABLE

/*
 * < MBED_API 
 */
#define CONFIG_MBED_API_EN 1

/*
 * < CONFIG FUNCTION TEST 
 */
#undef  CONFIG_PER_TEST

/*
 * < CONFIG SECURE TEST 
 */
#undef  CONFIG_SEC_VERIFY

/*
 * < CONFIG BT 
 */
#undef  CONFIG_BT_EN
#undef  CONFIG_BT
#undef  CONFIG_BT_PERIPHERAL
#undef  CONFIG_BT_CENTRAL
#undef  CONFIG_BT_SCATTERNET
#undef  CONFIG_BT_BEACON
#undef  CONFIG_BT_CONFIG
#undef  CONFIG_BT_AIRSYNC_CONFIG
#undef  CONFIG_BT_BREEZE
#undef  CONFIG_BT_TAG_SCANNER
#undef  CONFIG_BT_ANCS
#undef  CONFIG_BT_DISTANCE_DETECTOR
#undef  CONFIG_BT_MESH_PROVISIONER
#undef  CONFIG_BT_MESH_PROVISIONER_RTK_DEMO
#undef  CONFIG_BT_MESH_DEVICE
#undef  CONFIG_BT_MESH_DEVICE_RTK_DEMO
#undef  CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE
#undef  CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE
#undef  CONFIG_BT_MESH_CENTRAL
#undef  CONFIG_BT_MESH_PERIPHERAL
#undef  CONFIG_BT_MESH_SCATTERNET

/*
 * < CONFIG WIFI 
 */
#define CONFIG_WIFI_EN 1
#undef  CONFIG_HIGH_TP_TEST
#undef  WIFI_PERFORMANCE_MONITOR
#define CONFIG_WIFI_NORMAL 1
#undef  CONFIG_WIFI_TEST
#define CONFIG_WIFI_MODULE 1

/*
 * < CONFIG NETWORK 
 */
#define CONFIG_NETWORK 1

/*
 * < SSL Config 
 */
#define CONFIG_USE_MBEDTLS_ROM 1
#define CONFIG_MBED_TLS_ENABLED 1
#undef  CONFIG_SSL_ROM_TEST

/*
 * < DuerOS Config 
 */
#undef  CONFIG_BAIDU_DUER

/*
 * < LinkKit Config 
 */
#undef  CONFIG_LINKKIT_AWSS

/*
 * < MQTT Config 
 */
#undef  CONFIG_MQTT_EN

/*
 * < GUI Config 
 */
#undef  CONFIG_GUI_EN

/*
 * < Audio Config 
 */
#undef  CONFIG_AUDIO_EN

/*
 * To set debug msg flag
 */
#define CONFIG_DEBUG_LOG 1

/*
 * < Build Option 
 */
#define CONFIG_TOOLCHAIN_ASDK 1
#undef  CONFIG_TOOLCHAIN_ARM_GCC
#undef  CONFIG_LINK_ROM_LIB
#define CONFIG_LINK_ROM_SYMB 1


#endif /* #ifndef _PLATFORM_AUTOCONF_INCLUDED_ */
