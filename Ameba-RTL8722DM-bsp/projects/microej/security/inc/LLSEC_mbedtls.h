/*
 * C
 *
 * Copyright 2021-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief Security natives mbedtls structs.
 * @author MicroEJ Developer Team
 * @version 1.0.1
 */

#ifndef LLSEC_MBEDTLS
#define LLSEC_MBEDTLS

#include "mbedtls/pk.h"

typedef enum {
    TYPE_RSA = 6,     //EVP_PKEY_RSA,
    TYPE_ECDSA = 408, //EVP_PKEY_EC,
} LLSEC_pub_key_type;

/*key must be mbedtls_rsa_context or mbedtls_ecdsa_context TYPE*/
typedef struct {
    LLSEC_pub_key_type type;
    char *key; /*mbedtls_rsa_context or mbedtls_ecdsa_context*/
} LLSEC_priv_key;

typedef struct {
    LLSEC_pub_key_type type;
    char *key; /*mbedtls_rsa_context or mbedtls_ecdsa_context*/
} LLSEC_pub_key;

extern char *llsec_gen_random_str_internal(int length);
extern int llsec_customer_entropy_source(void *data, uint8_t *output, size_t len, size_t *olen);


#endif /* LLSEC_MBEDTLS */
