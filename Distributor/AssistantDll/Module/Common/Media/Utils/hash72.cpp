/**************************************************************************/
/*                                                                        */
/* Copyright (c) 1999-2011 NetDragon Websoft Inc.                         */
/*                    网龙网络有限公司版权所有 1999-2011                  */
/*                                                                        */
/*  PROPRIETARY RIGHTS of NetDragon Websoft Inc. are involved in the      */
/*  subject matter of this material. All manufacturing, reproduction,     */
/*  use, and sales rights pertaining to this subject matter are governed  */
/*  by the license agreement.  The recipient of this software implicitly  */ 
/*  accepts the terms of the license.                                     */
/*  本软件文档资料是网龙网络有限公司版权所有的合法资产，任何人士阅读和使  */
/*  用本资料必须获得相应的书面授权,承担保密责任和接受相应的法律约束。     */
/*                                                                        */
/**************************************************************************/

#include "hash.h"
#include "rijndael.h"

static const ND_UINT8 AES_KEY[16] = { 0x61, 0x8c, 0xa1, 0x0d, 0xc7, 0xf5, 0x7f, 0xd3, 0xb4, 0x72, 0x3e, 0x08, 0x15, 0x74, 0x63, 0xd7 };

/*
| hash_generate and hash_extract are :
| Copyright (c) 2009 Chris Lee <clee@mg8.org>
| licensed under the terms of the WTFPL <http://sam.zoy.org/wtfpl/>
*/
/* Generate a signature for an iTunesDB or a cbk file using the file SHA1
 * and a (IV, random bytes) for this device we want to sign for
 */
void ND_Hash72Generate( ND_UINT8 signature[46], const ND_UINT8 sha1[20], const ND_UINT8 iv[16], const ND_UINT8 random_bytes[12] )
{
	ND_UINT8 output[32] = { 0 }, plaintext[32] = { 0 };
	memcpy( plaintext, sha1, 20 );
	memcpy( &plaintext[20], random_bytes, 12 );

	signature[0] = 0x01;
	signature[1] = 0x00;
	memcpy( &signature[2], random_bytes, 12 );

	aes_set_key( (ND_UINT8*)AES_KEY );
	aes_encrypt( iv, plaintext, output, 32 );

	memcpy( &signature[14], output, 32 );
}

/* Given a valid signature and SHA1 for a file, extracts a valid (IV,
 * random bytes) couple which can be used to sign any iTunesDB or cbk file
 * for the device the signature was made for
 */
int ND_Hash72Extract( const ND_UINT8 signature[46], const ND_UINT8 sha1[20], ND_UINT8 iv[16], ND_UINT8 random_bytes[12] )
{
	ND_UINT8 plaintext[32] = { 0 }, output[32] = { 0 };

	if( signature[0] != 0x01 || signature[1] != 0x00 )
	{
		return -1;
	}

	memcpy( plaintext, sha1, 20 );
	memcpy( &plaintext[20], &signature[2], 12 );

	memcpy( output, plaintext, 32 );

	aes_set_key( (ND_UINT8*)AES_KEY );
	aes_decrypt( plaintext, (ND_UINT8*)&signature[14], output, 16 );

	if( memcmp( &plaintext[16], &output[16], 16 ) ) 
	{
		return -1;
	}

	memcpy( iv, output, 16 );
	memcpy( random_bytes, &signature[2], 12 );

	return 0;
}

