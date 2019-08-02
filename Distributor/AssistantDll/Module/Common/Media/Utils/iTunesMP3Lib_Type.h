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

#ifndef __ITUNES_MP3_LIB_TYPE_H
#define __ITUNES_MP3_LIB_TYPE_H

#include <stdint.h>

#ifdef WIN32
    #include <malloc.h>
#else
#endif


typedef unsigned int			ND_UINT32;
typedef int						ND_INT32;
typedef unsigned short			ND_UINT16;
typedef short					ND_INT16;
typedef unsigned char			ND_UINT8;
typedef char					ND_INT8;
typedef unsigned char			ND_UINT8;
typedef int16_t					ND_INT64;
typedef uint64_t				ND_UINT64;

typedef	unsigned int			ND_UINT;
typedef int						ND_INT;
typedef ND_UINT					ND_SIZE;

typedef char					ND_CHAR;
typedef unsigned char			ND_UCHAR;

typedef unsigned short			ND_UWCHAR;

typedef long					ND_LONG;
typedef unsigned long			ND_ULONG;

typedef char					ND_BOOL;

typedef float					ND_FLOAT;
typedef double					ND_DOUBLE;

#define ND_PATH_MAXSIZE			256

#endif