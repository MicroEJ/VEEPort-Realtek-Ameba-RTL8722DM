/* 
 * C
 * 
 * Copyright 2012-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
/* 
 * AUTOGENERATED by COMP-ICETEA-CIAR-ARM - 1.0.0 - DO NOT EDIT IT
 */
/* 
 * IceTea to C mapping support. Shall not be directly included by clients.
 */
#ifndef _LLMJVM_intern
#define _LLMJVM_intern 
typedef struct LLMJVM LLMJVM;
#include <stdint.h>
#ifdef __cplusplus
	extern "C" {
#endif
#define LLMJVM_initialize __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___initialize
#define LLMJVM_start __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___start__ILiceTea_lang_Ram_2
#define LLMJVM_getExitCode __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___getExitCode
#define LLMJVM_dump __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___dump
#define LLMJVM_checkIntegrity __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___checkIntegrity
#define LLMJVM_isImmortalArray __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___isImmortalArray__LiceTea_lang_Ram_2
#define LLMJVM_throwNativeException __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___throwNativeException__ILiceTea_lang_Ram_2Z
#define LLMJVM_isExceptionPending __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___isExceptionPending
#define LLMJVM_clearPendingException __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___clearPendingException
#define LLMJVM_registerResource __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___registerResource__LiceTea_lang_Ram_2LiceTea_lang_Method_2LiceTea_lang_Method_2
#define LLMJVM_unregisterResource __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___unregisterResource__LiceTea_lang_Ram_2LiceTea_lang_Method_2
#define LLMJVM_registerScopedResource __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___registerScopedResource__LiceTea_lang_Ram_2LiceTea_lang_Method_2LiceTea_lang_Method_2
#define LLMJVM_unregisterScopedResource __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___unregisterScopedResource
#define LLMJVM_getScopedResource __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___getScopedResource__LiceTea_lang_Ram_2LiceTea_lang_Ram_2LiceTea_lang_Ram_2
#define LLMJVM_getJavaResource __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___getJavaResource__LiceTea_lang_Ram_2ILiceTea_lang_Ram_2
#define LLMJVM_getCurrentJavaThreadID __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___getCurrentJavaThreadID
#define LLMJVM_suspendCurrentJavaThread __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___suspendCurrentJavaThread__JLiceTea_lang_Method_2LiceTea_lang_Ram_2Z
#define LLMJVM_javaThreadYield __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___javaThreadYield__LiceTea_lang_Method_2LiceTea_lang_Ram_2
#define LLMJVM_getCallbackArgs __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___getCallbackArgs__LiceTea_lang_Ram_2LiceTea_lang_Ram_2
#define LLMJVM_resumeJavaThread __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___resumeJavaThread__ILiceTea_lang_Ram_2
#define LLMJVM_isResumePending __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___isResumePending__I
#define LLMJVM_clearCurrentJavaThreadPendingResumeFlag __icetea__virtual__com_is2t_microjvm_IGreenThreadMicroJvm___clearCurrentJavaThreadPendingResumeFlag
struct LLMJVM
{
	int32_t __icetea__id__;
};
#ifdef __cplusplus
	}
#endif
#endif
