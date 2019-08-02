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

//本模块当前支持ID3的2.4.0,2.3.0协议

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
	ENUM_MP3_INFO_PARA_PATH = 0,					//0 文件路径
	ENUM_MP3_INFO_PARA_BITRATE,						//1 位速
	ENUM_MP3_INFO_PARA_SAMPLERATE,					//2 采样速率
	ENUM_MP3_INFO_PARA_FORMAT,						//3 音频格式
	ENUM_MP3_INFO_PARA_CHANNEL,						//4 通道
	ENUM_MP3_INFO_PARA_ID3VER,						//5 ID3标记
	ENUM_MP3_INFO_PARA_CODES,						//6 音频编码方式
	ENUM_MP3_INFO_PARA_TIT2,						//7 名称
	ENUM_MP3_INFO_PARA_TPE1,						//8 表演者
	ENUM_MP3_INFO_PARA_TPE2,						//9 专辑表演者
	ENUM_MP3_INFO_PARA_TCOM,						//10 作曲者
	ENUM_MP3_INFO_PARA_TALB,						//11 专辑
	ENUM_MP3_INFO_PARA_USLT,						//12 歌词
	ENUM_MP3_INFO_PARA_APIC,						//13 封面
	ENUM_MP3_INFO_PARA_TRCK,						//14 轨道号码
	ENUM_MP3_INFO_PARA_TPOS,						//15 光盘号码
	ENUM_MP3_INFO_PARA_TYER,						//16 年份
	ENUM_MP3_INFO_PARA_COMM,						//17 注释
	ENUM_MP3_INFO_PARA_TIT1,						//18 归类
	ENUM_MP3_INFO_PARA_TCON,						//19 风格
	ENUM_MP3_INFO_PARA_TIT3,						//20 描述
	ENUM_MP3_INFO_PARA_TBPM,						//21 BPM
	ENUM_MP3_INFO_PARA_TCMP,						//22 加入选集
	ENUM_MP3_INFO_PARA_TSOT,						//23 名称排序
	ENUM_MP3_INFO_PARA_TSOA,						//24 专辑排序
	ENUM_MP3_INFO_PARA_TSOP,						//25 表演者排序
	ENUM_MP3_INFO_PARA_TSO2,						//26 专辑表演者排序
	ENUM_MP3_INFO_PARA_TSOC,						//27 作曲者排序
	ENUM_MP3_INFO_PARA_RVAD,						//28 音量调整
	ENUM_MP3_INFO_PARA_PCST,						//29 媒体种类podcast
	ENUM_MP3_INFO_PARA_ITNU,						//30 媒体种类iTunes U
	ENUM_MP3_INFO_PARA_PLAYTIME,					//31 播放时长ms

	ENUM_MP3_INFO_PARA_NUM
}ENUM_MP3_INFO_PARA;

//判断是否支持MP3文件解码
ND_BOOL ND_ValidMP3Cap( const ND_CHAR * pszMP3FilePath );

//解析文件
/*HANDLE*/void* ND_ParseMP3Info( const ND_CHAR * pszMP3FilePath );

//获取MP3信息
HRESULT ND_GetMP3Info( /*HANDLE*/void* hMP3, ND_UINT32 iType, void * pData, ND_INT32 iDataSize );

//设置MP3信息
HRESULT ND_SetMP3Info( /*HANDLE*/void* hMP3, ND_UINT32 iType, void * pData, ND_INT32 iDataSize );

//释放MP3信息
void ND_ReleaseMP3Info( /*HANDLE*/void* hMP3 );

//获取eng信息: ppStr指向UTF8字符串
HRESULT ND_GetEngString( void * pData, ND_UINT32 iDataSize, ND_UCHAR ** ppStr, ND_UINT32 * pStrLen );

#ifdef __cplusplus
}
#endif

#endif
