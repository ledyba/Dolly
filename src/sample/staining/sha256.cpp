/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */
#include "sha256.h"
/*
 *  FIPS-180-2 compliant SHA-256 implementation
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
/*
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

namespace clr {

/*
 * SHA-256 context setup
 */
void sha2_starts( sha2_context *ctx )
{
    ctx->total[0] = Field(0);
    ctx->total[1] = Field(0);

    /* SHA-256 */
    ctx->state[0] = Field(0x6A09E667);
    ctx->state[1] = Field(0xBB67AE85);
    ctx->state[2] = Field(0x3C6EF372);
    ctx->state[3] = Field(0xA54FF53A);
    ctx->state[4] = Field(0x510E527F);
    ctx->state[5] = Field(0x9B05688C);
    ctx->state[6] = Field(0x1F83D9AB);
    ctx->state[7] = Field(0x5BE0CD19);
}

void sha2_process( sha2_context *ctx, const Field data[16] )
{
    Field temp1, temp2, W[64];
    Field A, B, C, D, E, F, G, H;

    for( int i=0;i<16;++i ) {
    	W[i] = data[i];
    }

#define  SHR(x,n) ((x & Field(0xFFFFFFFF)) >> Field(n))
#define ROTR(x,n) (SHR(x,n) | (x << Field(32 - n)))

#define S0(x) (ROTR(x, 7) ^ ROTR(x,18) ^  SHR(x, 3))
#define S1(x) (ROTR(x,17) ^ ROTR(x,19) ^  SHR(x,10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25))

#define F0(x,y,z) ((x & y) | (z & (x | y)))
#define F1(x,y,z) (z ^ (x & (y ^ z)))

#define R(t)                                    \
(                                               \
    W[t] = S1(W[t -  2]) + W[t -  7] +          \
           S0(W[t - 15]) + W[t - 16]            \
)

#define P(a,b,c,d,e,f,g,h,x,K)                  \
{                                               \
    temp1 = h + S3(e) + F1(e,f,g) + K + x;      \
    temp2 = S2(a) + F0(a,b,c);                  \
    d += temp1; h = temp1 + temp2;              \
}

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];
    E = ctx->state[4];
    F = ctx->state[5];
    G = ctx->state[6];
    H = ctx->state[7];

    P( A, B, C, D, E, F, G, H, W[ 0], Field(0x428A2F98) );
    P( H, A, B, C, D, E, F, G, W[ 1], Field(0x71374491) );
    P( G, H, A, B, C, D, E, F, W[ 2], Field(0xB5C0FBCF) );
    P( F, G, H, A, B, C, D, E, W[ 3], Field(0xE9B5DBA5) );
    P( E, F, G, H, A, B, C, D, W[ 4], Field(0x3956C25B) );
    P( D, E, F, G, H, A, B, C, W[ 5], Field(0x59F111F1) );
    P( C, D, E, F, G, H, A, B, W[ 6], Field(0x923F82A4) );
    P( B, C, D, E, F, G, H, A, W[ 7], Field(0xAB1C5ED5) );
    P( A, B, C, D, E, F, G, H, W[ 8], Field(0xD807AA98) );
    P( H, A, B, C, D, E, F, G, W[ 9], Field(0x12835B01) );
    P( G, H, A, B, C, D, E, F, W[10], Field(0x243185BE) );
    P( F, G, H, A, B, C, D, E, W[11], Field(0x550C7DC3) );
    P( E, F, G, H, A, B, C, D, W[12], Field(0x72BE5D74) );
    P( D, E, F, G, H, A, B, C, W[13], Field(0x80DEB1FE) );
    P( C, D, E, F, G, H, A, B, W[14], Field(0x9BDC06A7) );
    P( B, C, D, E, F, G, H, A, W[15], Field(0xC19BF174) );
    P( A, B, C, D, E, F, G, H, R(16), Field(0xE49B69C1) );
    P( H, A, B, C, D, E, F, G, R(17), Field(0xEFBE4786) );
    P( G, H, A, B, C, D, E, F, R(18), Field(0x0FC19DC6) );
    P( F, G, H, A, B, C, D, E, R(19), Field(0x240CA1CC) );
    P( E, F, G, H, A, B, C, D, R(20), Field(0x2DE92C6F) );
    P( D, E, F, G, H, A, B, C, R(21), Field(0x4A7484AA) );
    P( C, D, E, F, G, H, A, B, R(22), Field(0x5CB0A9DC) );
    P( B, C, D, E, F, G, H, A, R(23), Field(0x76F988DA) );
    P( A, B, C, D, E, F, G, H, R(24), Field(0x983E5152) );
    P( H, A, B, C, D, E, F, G, R(25), Field(0xA831C66D) );
    P( G, H, A, B, C, D, E, F, R(26), Field(0xB00327C8) );
    P( F, G, H, A, B, C, D, E, R(27), Field(0xBF597FC7) );
    P( E, F, G, H, A, B, C, D, R(28), Field(0xC6E00BF3) );
    P( D, E, F, G, H, A, B, C, R(29), Field(0xD5A79147) );
    P( C, D, E, F, G, H, A, B, R(30), Field(0x06CA6351) );
    P( B, C, D, E, F, G, H, A, R(31), Field(0x14292967) );
    P( A, B, C, D, E, F, G, H, R(32), Field(0x27B70A85) );
    P( H, A, B, C, D, E, F, G, R(33), Field(0x2E1B2138) );
    P( G, H, A, B, C, D, E, F, R(34), Field(0x4D2C6DFC) );
    P( F, G, H, A, B, C, D, E, R(35), Field(0x53380D13) );
    P( E, F, G, H, A, B, C, D, R(36), Field(0x650A7354) );
    P( D, E, F, G, H, A, B, C, R(37), Field(0x766A0ABB) );
    P( C, D, E, F, G, H, A, B, R(38), Field(0x81C2C92E) );
    P( B, C, D, E, F, G, H, A, R(39), Field(0x92722C85) );
    P( A, B, C, D, E, F, G, H, R(40), Field(0xA2BFE8A1) );
    P( H, A, B, C, D, E, F, G, R(41), Field(0xA81A664B) );
    P( G, H, A, B, C, D, E, F, R(42), Field(0xC24B8B70) );
    P( F, G, H, A, B, C, D, E, R(43), Field(0xC76C51A3) );
    P( E, F, G, H, A, B, C, D, R(44), Field(0xD192E819) );
    P( D, E, F, G, H, A, B, C, R(45), Field(0xD6990624) );
    P( C, D, E, F, G, H, A, B, R(46), Field(0xF40E3585) );
    P( B, C, D, E, F, G, H, A, R(47), Field(0x106AA070) );
    P( A, B, C, D, E, F, G, H, R(48), Field(0x19A4C116) );
    P( H, A, B, C, D, E, F, G, R(49), Field(0x1E376C08) );
    P( G, H, A, B, C, D, E, F, R(50), Field(0x2748774C) );
    P( F, G, H, A, B, C, D, E, R(51), Field(0x34B0BCB5) );
    P( E, F, G, H, A, B, C, D, R(52), Field(0x391C0CB3) );
    P( D, E, F, G, H, A, B, C, R(53), Field(0x4ED8AA4A) );
    P( C, D, E, F, G, H, A, B, R(54), Field(0x5B9CCA4F) );
    P( B, C, D, E, F, G, H, A, R(55), Field(0x682E6FF3) );
    P( A, B, C, D, E, F, G, H, R(56), Field(0x748F82EE) );
    P( H, A, B, C, D, E, F, G, R(57), Field(0x78A5636F) );
    P( G, H, A, B, C, D, E, F, R(58), Field(0x84C87814) );
    P( F, G, H, A, B, C, D, E, R(59), Field(0x8CC70208) );
    P( E, F, G, H, A, B, C, D, R(60), Field(0x90BEFFFA) );
    P( D, E, F, G, H, A, B, C, R(61), Field(0xA4506CEB) );
    P( C, D, E, F, G, H, A, B, R(62), Field(0xBEF9A3F7) );
    P( B, C, D, E, F, G, H, A, R(63), Field(0xC67178F2) );

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
    ctx->state[4] += E;
    ctx->state[5] += F;
    ctx->state[6] += G;
    ctx->state[7] += H;
}

/*
 * SHA-256 process buffer
 */
void sha2_update( sha2_context *ctx, const Field* input, size_t ilen )
{
    size_t fill;
    uint32_t left;

    if( ilen <= 0 )
        return;

    left = (ctx->total[0].value() & 0x3f)/4;
    fill = 16 - left;

    ctx->total[0] += Field((uint32_t) ilen * sizeof(uint32_t));
    ctx->total[0] &= Field(0xFFFFFFFF);

    if( ctx->total[0] < ((uint32_t) ilen) * sizeof(uint32_t) )
        ctx->total[1] += Field( sizeof(uint32_t) );

    if( left && ilen >= fill )
    {
    	for(size_t i=0;i<fill;++i){
    		ctx->buffer[i+left] = input[i];
    	}
        sha2_process( ctx, ctx->buffer );
        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while( ilen >= 64/4 )
    {
        sha2_process( ctx, input );
        input += 64/4;
        ilen  -= 64/4;
    }

    if( ilen > 0 ) {
    	for( size_t i=0;i<ilen;++i ) {
    		ctx->buffer[i+left] = input[i];
    	}
    }
}

static const Field sha2_padding[16] =
{
	Field(0x80000000),
	Field(),
	Field(),
	Field(),

	Field(),
	Field(),
	Field(),
	Field(),

	Field(),
	Field(),
	Field(),
	Field(),

	Field(),
	Field(),
	Field(),
	Field()
};

/*
 * SHA-256 final digest
 */
void sha2_finish( sha2_context *ctx, Field (&output)[8] )
{
    uint32_t last, padn;
    Field high, low;
    Field msglen[2];

    high = ( ctx->total[0] >> Field(29) )
         | ( ctx->total[1] << Field( 3) );
    low  = ( ctx->total[0] << Field( 3) );

    msglen[0] = high;
    msglen[1] = low;

    last = ctx->total[0].value() & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

    sha2_update( ctx, sha2_padding, padn/4 );
    sha2_update( ctx, msglen, 8/2 );

    for( int i=0; i<8; ++i ) {
    	output[i] = ctx->state[i];
    }

}

/*
 * output = SHA-256( input buffer )
 */
void sha2( const Field* input, size_t ilen, Field (&output)[8] )
{
    sha2_context ctx;

    sha2_starts( &ctx );
    sha2_update( &ctx, input, ilen );
    sha2_finish( &ctx, output );

    memset( &ctx, 0, sizeof( sha2_context ) );
}



}
