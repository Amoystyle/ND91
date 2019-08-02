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

#ifndef __HASH_H
#define __HASH_H

#include "iTunesMP3Lib_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum __enum_hash_type
{
	ENUM_HASH_TYPE_UNKNOWN	= -1,
    ENUM_HASH_TYPE_NONE	= 0,
	ENUM_HASH_TYPE_HASH58 = 1,
	ENUM_HASH_TYPE_HASH72 = 2,
	ENUM_HASH_TYPE_HASHAB = 3,
}ENUM_HASH_TYPE;

//Hash72�ֶδ�С
#define ND_HASH72_BUFFER_SIZE			46
//HashAB�ֶδ�С
#define ND_HASHAB_BUFFER_SIZE			57

void ND_Hash72Generate( ND_UINT8 szHash72[46], const ND_UINT8 szCheckSum[20], const ND_UINT8 szParam1[16], const ND_UINT8 szParam2[12] );
int ND_Hash72Extract( const ND_UINT8 szHash72[46], const ND_UINT8 szCheckSum[20], ND_UINT8 szParam1[16], ND_UINT8 szParam2[12] );

#ifdef __cplusplus
}
#endif

#endif