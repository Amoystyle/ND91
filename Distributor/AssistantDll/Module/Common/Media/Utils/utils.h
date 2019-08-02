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

#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iTunesMP3Lib_Type.h"
//产生一个64bitID
ND_UINT64 ND_GenerateID64( void );

//转化为MAC时间
ND_UINT32 ND_TimeConv( ND_UINT64 iTimeData );

//根据汉字的asc编码获取汉字拼音
extern ND_UCHAR * ND_GetHanziSpell( ND_UINT16 asc );

#ifdef __cplusplus
}
#endif

#endif