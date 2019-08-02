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

#ifndef __HZ2PY_H__
#define __HZ2PY_H__
#include <string.h>
#include <memory.h>
#include <stdio.h>

#include "iTunesMP3Lib_Type.h"

typedef struct _HanziToPinyinTag
{
	ND_UINT16 h0;	//高字节最小值
	ND_UINT16 h1;	//高字节最大值
	ND_UINT16 l0;	//低字节最小值
	ND_UINT16 l1;	//低字节最大值
	ND_UCHAR spell[7000][8];	//拼音
}HanziToPinyinType;

extern const HanziToPinyinType hzToPyE2;//一二级字库汉字拼音对照表，GB18030二区汉字

#endif // __HZ2PY_H__
