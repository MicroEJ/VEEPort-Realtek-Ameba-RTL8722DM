/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#include "LLEXT_RES_impl.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "fs_helper_fatfs_configuration.h"

#include "sni.h"

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct {
	void* data;
	uint32_t size;
} SNIX_resource;

extern int32_t SNIX_get_resource(const char* name, SNIX_resource* resource);

static FATFS fs;
static char  logical_drv[4]; /* root diretor */

static bool llext_res_is_initialized = false;

SNIX_resource microej_default_font = {0};

SNIX_resource* microej_default_font_ptr = (void*)-1;

#define MICROEJ_DEFAULT_FONT_NAME "/resources/fonts/microej_system_font.ejf_raw"

/**
 * @brief initializes external resource module.
 *
 * @return true on success; false if an error occurs.
 */
static bool LLEXT_RES_init(void);

static bool LLEXT_RES_init(void) {

	//get the default font
	if(microej_default_font_ptr == (void*)-1 && SNI_OK == SNIX_get_resource(MICROEJ_DEFAULT_FONT_NAME, &microej_default_font)){
		microej_default_font_ptr = (SNIX_resource*)&microej_default_font;
	}

    int drv_num = 0;
	int Fatfs_ok = 0;
	FRESULT res; 

	if(!llext_res_is_initialized){
        //register disk driver to fatfs
        drv_num = FATFS_RegisterDiskDriver(&FLASH_disk_Driver);

    	if(drv_num < 0){
		    printf("Rigester disk driver to FATFS fail.\n");
    	}else{
    		Fatfs_ok = 1;
    	}
         //mount
         if(Fatfs_ok){       
             res = f_mount(&fs, logical_drv, 1);
             if(res) {
                 if(f_mkfs(logical_drv, 1, 4096)!= FR_OK){
                     printf("Create FAT volume on Flash fail.\n");
                 }
                 if(f_mount(&fs, logical_drv, 0)!= FR_OK){
                     printf("FATFS mount logical drive on Flash fail.\n");
                 }else{
                    llext_res_is_initialized = true;
                 }
             }
        }
	}
	return llext_res_is_initialized;
}

RES_ID LLEXT_RES_open(const char* path)
{
	if(!LLEXT_RES_init()){
		//initialization failed!
		//return the default font
		return (RES_ID)microej_default_font_ptr;
	}

    //compute the absolute path by adding "/" root directory to the beginning of the path name
    int32_t length = strlen(path);
    char* absolute_path = (char*)malloc((size_t)++length);
     if(absolute_path == NULL){
		//memory allocation failed
		//return the default font
		 return (RES_ID)microej_default_font_ptr;
    }
    strcpy(absolute_path, "/");
    strcat(absolute_path, path);

    FIL *fp;
    FRESULT res = f_open(fp, (TCHAR*)absolute_path, FA_READ | FA_OPEN_EXISTING);
    
    if (res != FR_OK){
    	//file cannot be opened
    	free(absolute_path); //free the allocated memory for the absolute path
		//return the default font
    	return  (RES_ID)microej_default_font_ptr;
    }
    free(absolute_path); //free the allocated memory for the absolute path
    return (RES_ID)fp;
}

int32_t LLEXT_RES_close(RES_ID resourceID)
{
	if(resourceID == (RES_ID)microej_default_font_ptr){
		return LLEXT_RES_OK;
	}
	FIL* fp = (FIL*)resourceID;
	int32_t res = f_close(fp);
	if (res != FR_OK) {
		return LLEXT_RES_OK;
	}
	return -1;
}

int32_t LLEXT_RES_getBaseAddress(RES_ID resourceID)
{
	if(resourceID == (RES_ID)microej_default_font_ptr){
		//default font
		return (int32_t)(((SNIX_resource*)microej_default_font_ptr)->data);
	}
	//external resources will be loaded from FS
	return -1;
}

int32_t LLEXT_RES_read(RES_ID resourceID, void* ptr, int32_t* size)
{
	if(resourceID == (RES_ID)microej_default_font_ptr){
		//should not occur
		return -2; //error
	}
	FIL* fp = (FIL*)resourceID;
    unsigned int bytesread;
    
    FRESULT res = f_read(fp, (void*)ptr, *size, &bytesread);
	if (res != FR_OK) {
		return -2; //error
	} else {
		if (bytesread == 0) {
			return LLEXT_RES_EOF; //end-of-file
		} else {
			return LLEXT_RES_OK; //success
		}
	}
}

int32_t LLEXT_RES_available(RES_ID resourceID)
{
	if(resourceID == (RES_ID)microej_default_font_ptr){
		//default font
		return (int32_t)(((SNIX_resource*)microej_default_font_ptr)->size);
	}
	// resource from FS
	FIL* fp = (FIL*)resourceID;
	DWORD currentPtr = f_tell(fp);
	DWORD size = fp->fsize;

	if (currentPtr > size) {
		return 0;
	} else {
		return (size - currentPtr);
	}
}

int32_t LLEXT_RES_seek(RES_ID resourceID, int64_t offset)
{
	if(resourceID == (RES_ID)microej_default_font_ptr){
		//should not occur
		return -1; //error
	}
	FIL* fp = (FIL*)resourceID;
	FRESULT res = f_lseek(fp, offset);
	if(res != FR_OK){
	   return -1; //error
	}
	return LLEXT_RES_OK;
}

int64_t LLEXT_RES_tell(RES_ID resourceID)
{
	if(resourceID == (RES_ID)microej_default_font_ptr){
		//should not occur
		return -1; //error
	}
	FIL* fp = (FIL*)resourceID;
	return f_tell(fp);
}


#ifdef __cplusplus
	}
#endif
