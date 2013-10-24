/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */
#pragma once
#include "Field.h"

/**
 * \file sha2.h
 *
 * \brief SHA-224 and SHA-256 cryptographic hash function
 *
 *  Copyright (C) 2006-2013, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

namespace clr {

#include <string.h>
#include <inttypes.h>

#define POLARSSL_ERR_SHA2_FILE_IO_ERROR                -0x0078  /**< Read/write error in file. */

/**
 * \brief          SHA-256 context structure
 */
typedef struct
{
    Field total[2];          /*!< number of bytes processed  */
    Field state[8];          /*!< intermediate digest state  */
    Field buffer[16];   /*!< data block being processed */
}
sha2_context;


/**
 * \brief          SHA-256 context setup
 *
 * \param ctx      context to be initialized
 * \param is224    0 = use SHA256, 1 = use SHA224
 */
void sha2_starts( sha2_context *ctx );

/**
 * \brief          SHA-256 process buffer
 *
 * \param ctx      SHA-256 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void sha2_update( sha2_context *ctx, const Field* input, size_t ilen );

/**
 * \brief          SHA-256 final digest
 *
 * \param ctx      SHA-256 context
 * \param output   SHA-224/256 checksum result
 */
void sha2_finish( sha2_context *ctx, Field (&output)[8] );

/* Internal use */
void sha2_process( sha2_context *ctx, const Field data[16] );

void sha2( const Field* input, size_t ilen, Field (&output)[8] );

template <size_t N>
void sha2( const Field (&input)[N], Field (&output)[8] )
{
	sha2(input, N, output);
}
}
