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

#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iTunesMP3Lib_Type.h"
//����һ��64bitID
ND_UINT64 ND_GenerateID64( void );

//ת��ΪMACʱ��
ND_UINT32 ND_TimeConv( ND_UINT64 iTimeData );

//���ݺ��ֵ�asc�����ȡ����ƴ��
extern ND_UCHAR * ND_GetHanziSpell( ND_UINT16 asc );

#ifdef __cplusplus
}
#endif

#endif