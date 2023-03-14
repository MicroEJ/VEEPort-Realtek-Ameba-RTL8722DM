/*
 * Copyright 2020-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
#include "ameba_soc.h"
#include "main.h"
#include "microjvm_main.h"
#include "cpuload_conf.h"

#include "core_armv8mml.h"

#ifdef CPULOAD_ENABLED
#include "cpuload.h"
#endif

#ifdef ENABLE_SYSTEM_VIEW
#include "SEGGER_RTT.h"
#include "SEGGER_SYSVIEW.h"
#endif

//Enable this define to enable the watchdog freertos scheduler checkpoint
//#define WATCHDOG_TIMER_FREERTOS_SCHEDULER_CHECKPOINT

#ifdef WATCHDOG_TIMER_FREERTOS_SCHEDULER_CHECKPOINT
#include "LLWATCHDOG_TIMER_impl.h"
#include "watchdog_timer_freertos.h"
#endif

//Enable this define to run the Platform Qualification Tools benchmark
//#define PLATFORM_QUALIFICATION_TOOLS

#ifdef PLATFORM_QUALIFICATION_TOOLS
#include "t_core_main.h"
#endif

//Enable this define to run the lwiperf benchmark
// #define ENABLE_IPERF

#ifdef ENABLE_IPERF
#include "iperf.h"

#define LWIPERF_STACK_SIZE      (12 * 1024)
#define LWIPERF_TASK_PRIORITY   (11)
#define LWIPERF_TASK_STACK_SIZE (LWIPERF_STACK_SIZE / 4)
#endif // ENABLE_IPERF


#define MICROJVM_STACK_SIZE      (24 * 1024)
#define JAVA_TASK_PRIORITY       (11)  /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define JAVA_TASK_STACK_SIZE     (MICROJVM_STACK_SIZE / 4)
#ifdef CPULOAD_ENABLED
#define CPULOAD_INIT_TASK_STACK_SIZE     (512 / 4)
static void cpuload_suspend_tasks(void);
static void cpuload_resume_tasks(void);
static void cpuload_init_task(void * pvParameters);
#endif //CPULOAD_ENABLED

TaskHandle_t microjvm_task;
TaskHandle_t wifi_on_task;

#ifdef ENABLE_IPERF
TaskHandle_t iperf_task;
#endif // ENABLE_IPERF

#if defined(CONFIG_FTL_ENABLED)
#include "ftl_int.h"

extern const u8 ftl_phy_page_num;
extern const u32 ftl_phy_page_start_addr;

void app_ftl_init(void)
{
	ftl_init(ftl_phy_page_start_addr, ftl_phy_page_num);
}
#endif

#if defined(CONFIG_WIFI_NORMAL) && defined(CONFIG_NETWORK)
extern VOID wlan_network(VOID);
extern u32 GlobalDebugEnable;
#endif
void app_captouch_init(void);
void app_keyscan_init(u8 reset_status);

void app_init_debug(void)
{
	u32 debug[4];

	debug[LEVEL_ERROR] = BIT(MODULE_BOOT);
	debug[LEVEL_WARN]  = 0x0;
	debug[LEVEL_INFO]  = BIT(MODULE_BOOT);
	debug[LEVEL_TRACE] = 0x0;

	debug[LEVEL_ERROR] = 0xFFFFFFFF;

	LOG_MASK(LEVEL_ERROR, debug[LEVEL_ERROR]);
	LOG_MASK(LEVEL_WARN, debug[LEVEL_WARN]);
	LOG_MASK(LEVEL_INFO, debug[LEVEL_INFO]);
	LOG_MASK(LEVEL_TRACE, debug[LEVEL_TRACE]);
}

static void* app_mbedtls_calloc_func(size_t nelements, size_t elementSize)
{
	size_t size;
	void *ptr = NULL;

	size = nelements * elementSize;
	ptr = pvPortMalloc(size);

	if(ptr)
		_memset(ptr, 0, size);

	return ptr;
}

void app_mbedtls_rom_init(void)
{
	mbedtls_platform_set_calloc_free(app_mbedtls_calloc_func, vPortFree);
	//rom_ssl_ram_map.use_hw_crypto_func = 1;
	rtl_cryptoEngine_init();
}

VOID app_start_autoicg(VOID)
{
	u32 temp = 0;

	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HS_PLATFORM_PARA);
	temp |= BIT_HSYS_PLFM_AUTO_ICG_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HS_PLATFORM_PARA, temp);
}

VOID app_pmu_init(VOID)
{
	if (BKUP_Read(BKUP_REG0) & BIT_SW_SIM_RSVD){
		return;
	}

	pmu_set_sleep_type(SLEEP_PG);

	/* if wake from deepsleep, that means we have released wakelock last time */
	//if (SOCPS_DsleepWakeStatusGet() == TRUE) {
	//	pmu_set_sysactive_time(2);
	//	pmu_release_wakelock(PMU_OS);
	//	pmu_tickless_debug(ENABLE);
	//}
}

/* enable or disable BT shared memory */
/* if enable, KM4 can access it as SRAM */
/* if disable, just BT can access it */
/* 0x100E_0000	0x100E_FFFF	64K */
VOID app_shared_btmem(u32 NewStatus)
{
	u32 temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HS_PLATFORM_PARA);

	if (NewStatus == ENABLE) {
		temp |= BIT_HSYS_SHARE_BT_MEM;
	} else {
		temp &= ~BIT_HSYS_SHARE_BT_MEM;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HS_PLATFORM_PARA, temp);
}

static void app_dslp_wake(void)
{
	u32 aon_wake_event = SOCPS_AONWakeReason();

	DBG_8195A("hs app_dslp_wake %x \n", aon_wake_event);

	if(BIT_GPIO_WAKE_STS & aon_wake_event) {
		DBG_8195A("DSLP AonWakepin wakeup, wakepin %x\n", SOCPS_WakePinCheck());
	}

	if(BIT_AON_WAKE_TIM0_STS & aon_wake_event) {
		SOCPS_AONTimerCmd(DISABLE);
		DBG_8195A("DSLP Aontimer wakeup \n");
	}

	if(BIT_RTC_WAKE_STS & aon_wake_event) {
		DBG_8195A("DSLP RTC wakeup \n");
	}

	if(BIT_DLPS_TSF_WAKE_STS & aon_wake_event) {
		DBG_8195A("DSLP TSF wakeup \n");
	}

	if(BIT_KEYSCAN_WAKE_STS & aon_wake_event) {
		DBG_8195A("DSLP KS wakeup\n");
	}

	if(BIT_CAPTOUCH_WAKE_STS & aon_wake_event) {
		DBG_8195A("DSLP Touch wakeup\n");
	}

	SOCPS_AONWakeClear(BIT_ALL_WAKE_STS);
}

#ifdef ENABLE_IPERF
void xIperfTaskFunction(void * pvParameters)
{
	/* Wait lwip connection */
	vTaskDelay(10000 / portTICK_PERIOD_MS);

	/* Start the iperf  */
	iperf_main();

	/* Should reach this point only if the iperf exits */
	vTaskDelete(xTaskGetCurrentTaskHandle());
}
#endif // ENABLE_IPERF

/*!
 * @brief Java task function that runs in an OS task
 */
void xJavaTaskFunction(void * pvParameters)
{
#ifdef PLATFORM_QUALIFICATION_TOOLS
    T_CORE_main();
#endif
    microjvm_main();
    vTaskDelete(xTaskGetCurrentTaskHandle());
}

#ifdef CPULOAD_ENABLED

/*
 * This function will suspend all other tasks that can disturb reference time calculation for CPULoad feature.
 * Here need to be suspended any new created task that will run from start-up and perform heavy workload.
 * This is needed only if CPULoad feature is active and the values reported are suspiciously high.
 */
static void cpuload_suspend_tasks(void){
	// Suspend MicroJvm task
	vTaskSuspend(microjvm_task);
	// Search for "init" task handler and suspend the task if found.
	// "init" task is starting WIFI module.
	wifi_on_task = xTaskGetHandle("init");
	if (wifi_on_task != NULL) {
		vTaskSuspend(wifi_on_task);
	}
}

/*
 * In this function we need to resume all tasks suspended in the suspend_tasks() function.
 */
static void cpuload_resume_tasks(void){

	vTaskResume(microjvm_task);
	if (wifi_on_task != NULL) {
		vTaskResume(wifi_on_task);
	}
}

/*
 * Initializes the CPULoad feature.
 *
 */
static void cpuload_init_task(void * pvParameters)
{
	// Call below function in order to suspend other tasks that can be triggered while we calculate the reference time.
	cpuload_suspend_tasks();

	// Init cpuload
	cpuload_init();

	// Resume all previous suspended tasks.
	cpuload_resume_tasks();

	vTaskDelete(xTaskGetCurrentTaskHandle());
}

#endif //CPULOAD_ENABLED

//default main 
int main(void)
{
	if (rtk_wifi_config.wifi_ultra_low_power &&
		rtk_wifi_config.wifi_app_ctrl_tdma == FALSE) {
		SystemSetCpuClk(CLK_KM4_100M);
	}
	InterruptRegister(IPC_INTHandler, IPC_IRQ, (u32)IPCM0_DEV, 5);
	InterruptEn(IPC_IRQ, 5);

	// Disable Wi-Fi fast connect.
	Erase_Fastconnect_data();

#ifdef CONFIG_MBED_TLS_ENABLED
	app_mbedtls_rom_init();
#endif
	//app_init_debug();

	/* init console */
	shell_recv_all_data_onetime = 1;
	shell_init_rom(0, 0);
	shell_init_ram();
	ipc_table_init();

	/* Register Log Uart Callback function */
	InterruptRegister((IRQ_FUN) shell_uart_irq_rom, UART_LOG_IRQ, (u32)NULL, 5);
	InterruptEn(UART_LOG_IRQ,5);

	if(TRUE == SOCPS_DsleepWakeStatusGet()) {
		app_dslp_wake();
	}

#ifdef CONFIG_FTL_ENABLED
	app_ftl_init();
#endif

#if defined(CONFIG_WIFI_NORMAL) && defined(CONFIG_NETWORK)
	rtw_efuse_boot_write();

	/* pre-processor of application example */
	pre_example_entry();

	wlan_network();

	/* Execute application example */
	example_entry();
#endif

#if defined(CONFIG_EQC) && CONFIG_EQC
	//EQC_test_entry();
#endif
	app_start_autoicg();
	//app_shared_btmem(ENABLE);

	app_pmu_init();

	if ((BKUP_Read(0) & BIT_KEY_ENABLE))
		app_keyscan_init(FALSE); /* 5uA */
	if ((BKUP_Read(0) & BIT_CAPTOUCH_ENABLE))
		app_captouch_init(); /* 1uA */
	//if ((BKUP_Read(0) & BIT_GPIO_ENABLE))
	//	app_hp_jack_init();

	app_init_debug();
#ifdef ENABLE_SYSTEM_VIEW
	printf("Enable Segger system view\n");
	SEGGER_SYSVIEW_Conf();
	SEGGER_RTT_WriteString(0, "Hello World from SEGGER RTT!\r\n");
	printf("SEGGER_RTT block address: %p\n", &(_SEGGER_RTT));
#endif

#ifdef WATCHDOG_TIMER_FREERTOS_SCHEDULER_CHECKPOINT
	printf("Enable watchdog FreeRTOS scheduler checkpoint\n");
	printf("Watchdog initialization\n");
	if(WATCHDOG_TIMER_ERROR == LLWATCHDOG_TIMER_IMPL_init()){
		printf("Failed to init watchdog timer in main. \n");
	} else{
		printf("Start watchdog FreeRTOS scheduler checkpoint\n");
		if(CHECKPOINT_FREERTOS_ERROR == watchdog_timer_freertos_scheduler_checkpoint_start()){
			printf("Failed to start watchdog FreeRTOS scheduler checkpoint in main.\n");
		}
	}
#endif

#ifdef ENABLE_IPERF
	/* Start lwiperf task */
	xTaskCreate( xIperfTaskFunction, "lwiperf", LWIPERF_TASK_STACK_SIZE, NULL, LWIPERF_TASK_PRIORITY, NULL );
#endif // ENABLE_IPERF

	/* Create the MicroJvm task. */
	xTaskCreate(xJavaTaskFunction, "MicroJvm", JAVA_TASK_STACK_SIZE, NULL, JAVA_TASK_PRIORITY, &microjvm_task);
#ifdef CPULOAD_ENABLED
	// Create CPULoad init task with higher priority to ensure the task is executed before other tasks
	xTaskCreate(cpuload_init_task, "CpuloadInit", CPULOAD_INIT_TASK_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
#endif

	//DBG_8195A("M4U:%d \n", RTIM_GetCount(TIMM05));
	/* Enable Schedule, Start Kernel */
	
	SCB_Type *cmsis_cpu_reg_info = SCB;
	uint32_t cpuid_reg = cmsis_cpu_reg_info->CPUID;
	printf("\n =================================== \n");
	printf("CPUID = 0x%x\n",cpuid_reg);
	printf("Implementer = 0x%x\n",(cpuid_reg & SCB_CPUID_IMPLEMENTER_Msk) >> SCB_CPUID_IMPLEMENTER_Pos);
	printf("Variant = 0x%x\n",(cpuid_reg & SCB_CPUID_VARIANT_Msk) >> SCB_CPUID_VARIANT_Pos);
	printf("Constant = 0x%x\n",(cpuid_reg & SCB_CPUID_ARCHITECTURE_Msk) >> SCB_CPUID_ARCHITECTURE_Pos);
	printf("Part number = 0x%x\n",(cpuid_reg & SCB_CPUID_PARTNO_Msk) >> SCB_CPUID_PARTNO_Pos);
	printf("Revision = 0x%x\n",cpuid_reg & SCB_CPUID_REVISION_Msk);
	printf(" =================================== \n");

	vTaskStartScheduler();
}

