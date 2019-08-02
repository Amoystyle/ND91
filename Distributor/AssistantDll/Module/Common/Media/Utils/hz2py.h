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

#ifndef __HZ2PY_H__
#define __HZ2PY_H__
#include <string.h>
#include <memory.h>
#include <stdio.h>

#include "iTunesMP3Lib_Type.h"

typedef struct _HanziToPinyinTag
{
	ND_UINT16 h0;	//���ֽ���Сֵ
	ND_UINT16 h1;	//���ֽ����ֵ
	ND_UINT16 l0;	//���ֽ���Сֵ
	ND_UINT16 l1;	//���ֽ����ֵ
	ND_UCHAR spell[7000][8];	//ƴ��
}HanziToPinyinType;

extern const HanziToPinyinType hzToPyE2;//һ�����ֿ⺺��ƴ�����ձ�GB18030��������

#endif // __HZ2PY_H__
