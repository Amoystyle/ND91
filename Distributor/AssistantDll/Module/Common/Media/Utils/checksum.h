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

#ifndef __CHECKSUM_H
#define __CHECKSUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iTunesMP3Lib_Type.h"

typedef enum __enum_checksum_type
{
	ENUM_CHECKSUM_TYPE_SHA1 = 0,
	ENUM_CHECKSUM_TYPE_MD5 = 1,
	ENUM_CHECKSUM_TYPE_SHA256 = 2,
}ENUM_CHECKSUM_TYPE;

//#####################################################################################
//################################# SHA1数据结构开始 ##################################
//#####################################################################################

//SHA1校验buffer大小
#define SHA1_DIGEST_LEN			20

//SHA1 block块大小
#define SHA_BLOCK_SIZE			1024

#define SHA1_DATASIZE			64

typedef struct
{
	ND_UINT32 m_iBuf[5];
	ND_UINT32 m_iBits[2];

	/* we pack 64 unsigned chars into 16 32-bit unsigned integers */
	ND_UINT32 m_iData[16];

	ND_UCHAR m_szDigest[SHA1_DIGEST_LEN];
}Sha1sum;

//#####################################################################################
//################################# SHA1数据结构结束 ##################################
//#####################################################################################

//#####################################################################################
//################################# MD5 数据结构开始 ##################################
//#####################################################################################

#define MD5_DATASIZE    64
#define MD5_DIGEST_LEN  16

typedef struct
{
	ND_UINT32 m_iBuf[4];
	ND_UINT32 m_iBits[2];
  
	ND_UCHAR m_szData[MD5_DATASIZE];

	ND_UCHAR m_szDigest[MD5_DIGEST_LEN];
}Md5sum;

//#####################################################################################
//################################# MD5 数据结构结束 ##################################
//#####################################################################################

//#####################################################################################
//################################# sha256数据结构开始 ################################
//#####################################################################################

#define SHA256_DATASIZE         64
#define SHA256_DIGEST_LEN       32

typedef struct
{
	ND_UINT32 m_iBuf[8];
	ND_UINT32 m_iBits[2];

	ND_UINT8 m_szData[SHA256_DATASIZE];

	ND_UCHAR m_szDigest[SHA256_DIGEST_LEN];
}Sha256sum;

//#####################################################################################
//################################# sha256数据结构结束 ################################
//#####################################################################################

typedef struct __tag_checksum
{
	ENUM_CHECKSUM_TYPE m_iType;		//校验类型
	ND_CHAR * m_szDigest_str;		
	union
	{
		Sha1sum m_sha1;
		Md5sum m_md5;		
		Sha256sum m_sha256;
	}m_sum;
}ND_CheckSum;

//构建checksum对象
ND_CheckSum * CreateChecksumObject( ENUM_CHECKSUM_TYPE iType );

//获取校验类型字符串长度
ND_SIZE GetChecksumStringLen( ENUM_CHECKSUM_TYPE iType );

//得到校验结果
void GetChecksumUpdate( ND_CheckSum * pCS, const ND_UCHAR * pszData, ND_SIZE iLength );

//获取校验DIGEST
void GetChecksumDigest( ND_CheckSum * pCS, ND_UINT8 * pBuffer, ND_SIZE * iDigestLen );

//释放Checksum对象
void ReleaseChecksumObject( ND_CheckSum * pCS );

void sha1_sum_update( Sha1sum * sha1, const ND_UCHAR *buffer, ND_SIZE count );
void sha1_sum_close( Sha1sum *sha1 );
void sha1_sum_digest( Sha1sum *sha1, ND_UINT8  *digest );
ND_CHAR * sha1_sum_to_string( Sha1sum *sha1 );
void sha1_sum_init( Sha1sum *sha1 );

//计算sha1值 pszSha1结果20个字节，pszData输入的内容，iLength内容长度
void getSha1(ND_UCHAR *pszSha1, const ND_UCHAR *pszData, ND_SIZE iLength);

#ifdef __cplusplus
}
#endif

#endif