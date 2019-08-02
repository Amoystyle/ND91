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
#pragma once
#ifndef __MP3DECODER_H
#define __MP3DECODER_H

#ifdef WIN32
	#include <Windows.h>
#else
	#include "Common/winPort.h"
  #include <string>
	#include <fcntl.h>
	#include <errno.h>
	#include <sys/times.h>
	#include <iconv.h>
	#define CODEC_GB2312  "GB2312"
	#define CODEC_UTF8    "UTF-8"
  #define CODEC_UNICODE "UCS-4-INTERNAL"
  //typedef void* 				HANDLE;
  typedef long 					HRESULT;
  #define S_OK					((HRESULT)0L)
  #define SUCCEEDED(hr)	(((HRESULT)(hr)) >= 0)
  #define FAILED(hr) 		(((HRESULT)(hr)) < 0)
  //#define min(a, b)   (((a) < (b)) ? (a) : (b))
  
  #ifdef RC_INVOKED
		#define _HRESULT_TYPEDEF_(_sc) _sc
	#else // RC_INVOKED
		#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)
	#endif // RC_INVOKED

  //#if defined(_WIN32) && !defined(_MAC)
    #define E_FAIL					_HRESULT_TYPEDEF_(0x80004005L)
    #define E_NOTIMPL				_HRESULT_TYPEDEF_(0x80004001L)
    #define E_OUTOFMEMORY 	_HRESULT_TYPEDEF_(0x8007000EL)
    #define E_INVALIDARG    _HRESULT_TYPEDEF_(0x80070057L)
//   #else
//     #define E_FAIL					_HRESULT_TYPEDEF_(0x80000008L)
//     #define E_NOTIMPL				_HRESULT_TYPEDEF_(0x80000001L)
//     #define E_OUTOFMEMORY 	_HRESULT_TYPEDEF_(0x80000002L)
//     #define E_INVALIDARG    _HRESULT_TYPEDEF_(0x80000003L)
//   #endif
#endif
#include "../Utils/iTunesMP3Lib_Type.h"

//��ģ�鵱ǰ֧��ID3��2.4.0,2.3.0Э��

#ifdef __cplusplus
extern "C" {
#endif

typedef enum __enum_ID3_ver
{
	ENUM_ID3VER_2_4_0 = 0x02004000,
	ENUM_ID3VER_2_3_0 = 0x02003000,
}ENUM_ID3VER;

typedef enum __enum_audio_layer
{
	ENUM_AUDIO_LAYER1 = 0,
	ENUM_AUDIO_LAYER2,
	ENUM_AUDIO_LAYER3,

}ENUM_AUDIO_LAYER;

typedef enum __enum_mp3_info_para
{
	ENUM_MP3_INFO_PARA_PATH = 0,					//0 �ļ�·��
	ENUM_MP3_INFO_PARA_BITRATE,						//1 λ��
	ENUM_MP3_INFO_PARA_SAMPLERATE,					//2 ��������
	ENUM_MP3_INFO_PARA_FORMAT,						//3 ��Ƶ��ʽ
	ENUM_MP3_INFO_PARA_CHANNEL,						//4 ͨ��
	ENUM_MP3_INFO_PARA_ID3VER,						//5 ID3���
	ENUM_MP3_INFO_PARA_CODES,						//6 ��Ƶ���뷽ʽ
	ENUM_MP3_INFO_PARA_TIT2,						//7 ����
	ENUM_MP3_INFO_PARA_TPE1,						//8 ������
	ENUM_MP3_INFO_PARA_TPE2,						//9 ר��������
	ENUM_MP3_INFO_PARA_TCOM,						//10 ������
	ENUM_MP3_INFO_PARA_TALB,						//11 ר��
	ENUM_MP3_INFO_PARA_USLT,						//12 ���
	ENUM_MP3_INFO_PARA_APIC,						//13 ����
	ENUM_MP3_INFO_PARA_TRCK,						//14 �������
	ENUM_MP3_INFO_PARA_TPOS,						//15 ���̺���
	ENUM_MP3_INFO_PARA_TYER,						//16 ���
	ENUM_MP3_INFO_PARA_COMM,						//17 ע��
	ENUM_MP3_INFO_PARA_TIT1,						//18 ����
	ENUM_MP3_INFO_PARA_TCON,						//19 ���
	ENUM_MP3_INFO_PARA_TIT3,						//20 ����
	ENUM_MP3_INFO_PARA_TBPM,						//21 BPM
	ENUM_MP3_INFO_PARA_TCMP,						//22 ����ѡ��
	ENUM_MP3_INFO_PARA_TSOT,						//23 ��������
	ENUM_MP3_INFO_PARA_TSOA,						//24 ר������
	ENUM_MP3_INFO_PARA_TSOP,						//25 ����������
	ENUM_MP3_INFO_PARA_TSO2,						//26 ר������������
	ENUM_MP3_INFO_PARA_TSOC,						//27 ����������
	ENUM_MP3_INFO_PARA_RVAD,						//28 ��������
	ENUM_MP3_INFO_PARA_PCST,						//29 ý������podcast
	ENUM_MP3_INFO_PARA_ITNU,						//30 ý������iTunes U
	ENUM_MP3_INFO_PARA_PLAYTIME,					//31 ����ʱ��ms

	ENUM_MP3_INFO_PARA_NUM
}ENUM_MP3_INFO_PARA;

//�ж��Ƿ�֧��MP3�ļ�����
ND_BOOL ND_ValidMP3Cap( const ND_CHAR * pszMP3FilePath );

//�����ļ�
/*HANDLE*/void* ND_ParseMP3Info( const ND_CHAR * pszMP3FilePath );

//��ȡMP3��Ϣ
HRESULT ND_GetMP3Info( /*HANDLE*/void* hMP3, ND_UINT32 iType, void * pData, ND_INT32 iDataSize );

//����MP3��Ϣ
HRESULT ND_SetMP3Info( /*HANDLE*/void* hMP3, ND_UINT32 iType, void * pData, ND_INT32 iDataSize );

//�ͷ�MP3��Ϣ
void ND_ReleaseMP3Info( /*HANDLE*/void* hMP3 );

//��ȡeng��Ϣ: ppStrָ��UTF8�ַ���
HRESULT ND_GetEngString( void * pData, ND_UINT32 iDataSize, ND_UCHAR ** ppStr, ND_UINT32 * pStrLen );

#ifdef __cplusplus
}
#endif

#endif
