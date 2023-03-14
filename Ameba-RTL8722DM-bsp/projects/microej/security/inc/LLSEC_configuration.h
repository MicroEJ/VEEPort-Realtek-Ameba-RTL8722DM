/*
 * C
 *
 * Copyright 2021-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
 
/**
 * @file
 * @brief Security natives configuration.
 * @author MicroEJ Developer Team
 * @version 1.0.1
 */

//#include "microej_allocator.h"
#include "platform_stdlib.h" 

#define LLSEC_calloc calloc
#define LLSEC_free free

/*
* Used for private and public key generation
*/
#define LLSEC_PRIVATE_KEY_LOCAL_BUFFER_SIZE 3072
#define LLSEC_PUBLIC_KEY_LOCAL_BUFFER_SIZE  3072
