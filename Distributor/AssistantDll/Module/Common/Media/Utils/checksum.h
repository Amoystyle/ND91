/**************************************************************************/
/*                                                                        */
/* Copyright (c) 1999-2011 NetDragon Websoft Inc.                         */
/*                    �����������޹�˾��Ȩ���� 1999-2011                  */
/*                                                                        */
/*  PROPRIETARY RIGHTS of NetDragon Websoft Inc. are involved in the      */
/*  subject matter of this material. All manufacturing, reproduction,     */
/*  use, and sales rights pertaining to this subject matter are governed  */
/*  by the license agreement.  The recipient of this software implicitly  */ 
/*  accepts the terms of the license.                                     */
/*  ������ĵ������������������޹�˾��Ȩ���еĺϷ��ʲ����κ���ʿ�Ķ���ʹ  */
/*  �ñ����ϱ�������Ӧ��������Ȩ,�е��������κͽ�����Ӧ�ķ���Լ����     */
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
//################################# SHA1���ݽṹ��ʼ ##################################
//#####################################################################################

//SHA1У��buffer��С
#define SHA1_DIGEST_LEN			20

//SHA1 block���С
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
//################################# SHA1���ݽṹ���� ##################################
//#####################################################################################

//#####################################################################################
//################################# MD5 ���ݽṹ��ʼ ##################################
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
//################################# MD5 ���ݽṹ���� ##################################
//#####################################################################################

//#####################################################################################
//################################# sha256���ݽṹ��ʼ ################################
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
//################################# sha256���ݽṹ���� ################################
//#####################################################################################

typedef struct __tag_checksum
{
	ENUM_CHECKSUM_TYPE m_iType;		//У������
	ND_CHAR * m_szDigest_str;		
	union
	{
		Sha1sum m_sha1;
		Md5sum m_md5;		
		Sha256sum m_sha256;
	}m_sum;
}ND_CheckSum;

//����checksum����
ND_CheckSum * CreateChecksumObject( ENUM_CHECKSUM_TYPE iType );

//��ȡУ�������ַ�������
ND_SIZE GetChecksumStringLen( ENUM_CHECKSUM_TYPE iType );

//�õ�У����
void GetChecksumUpdate( ND_CheckSum * pCS, const ND_UCHAR * pszData, ND_SIZE iLength );

//��ȡУ��DIGEST
void GetChecksumDigest( ND_CheckSum * pCS, ND_UINT8 * pBuffer, ND_SIZE * iDigestLen );

//�ͷ�Checksum����
void ReleaseChecksumObject( ND_CheckSum * pCS );

void sha1_sum_update( Sha1sum * sha1, const ND_UCHAR *buffer, ND_SIZE count );
void sha1_sum_close( Sha1sum *sha1 );
void sha1_sum_digest( Sha1sum *sha1, ND_UINT8  *digest );
ND_CHAR * sha1_sum_to_string( Sha1sum *sha1 );
void sha1_sum_init( Sha1sum *sha1 );

//����sha1ֵ pszSha1���20���ֽڣ�pszData��������ݣ�iLength���ݳ���
void getSha1(ND_UCHAR *pszSha1, const ND_UCHAR *pszData, ND_SIZE iLength);

#ifdef __cplusplus
}
#endif

#endif