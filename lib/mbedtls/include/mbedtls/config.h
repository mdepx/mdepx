/*
 *  Copyright (C) 2016, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include <sys/cdefs.h>

#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

#define	MBEDTLS_MD_C
#define	MBEDTLS_ASN1_PARSE_C

#define	MBEDTLS_X509_USE_C
#define	MBEDTLS_BIGNUM_C
#define	MBEDTLS_OID_C
#define	MBEDTLS_PK_PARSE_C
#define	MBEDTLS_PK_C
#define	MBEDTLS_RSA_C
#define	MBEDTLS_PKCS1_V21
#define	MBEDTLS_X509_CRT_PARSE_C
#define	MBEDTLS_SSL_TLS_C
#define	MBEDTLS_CIPHER_C
#define	MBEDTLS_SSL_PROTO_TLS1_2
#define	MBEDTLS_SHA256_C

#include "check_config.h"

#endif /* MBEDTLS_CONFIG_H */
