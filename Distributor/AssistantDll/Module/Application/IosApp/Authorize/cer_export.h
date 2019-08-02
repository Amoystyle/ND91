/**************************************************************************/
/*                                                                        */
/* Copyright (c) 1999-2012 NetDragon Websoft Inc.                         */
/*                    网龙网络有限公司版权所有 1999-2012                  */
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

/**************************************************************************/
/*  作者		—— 罗王毅                                               */
/*  创建日期	—— 2012.05.14                                           */
/**************************************************************************/

#ifndef __CER_EXPORT_H
#define __CER_EXPORT_H

#include "windows.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned long long    ND_UInt64;
typedef unsigned long         ND_UInt32;
typedef unsigned char         ND_UCHAR;

#define ADNCI_MSG_CONNECTED     1
#define ADNCI_MSG_DISCONNECTED  2
#define ADNCI_MSG_UNKNOWN       3

typedef enum { IPOD_STATE_UNCONNECTED = 0, IPOD_STATE_CONNECTED, IPOD_STATE_READY } t_iPodState;

struct am_device {
    unsigned char unknown0[16]; /* 0 - zero */
    unsigned int device_id;     /* 16 */
    unsigned int product_id;    /* 20 - set to AMD_IPHONE_PRODUCT_ID */
    char *serial;               /* 24 - set to AMD_IPHONE_SERIAL */
    unsigned int unknown1;      /* 28 */
    unsigned char unknown2[4];  /* 32 */
    unsigned int lockdown_conn; /* 36 */
    unsigned char unknown3[8];  /* 40 */
};

// struct am_device_notification_callback_info {
// 	struct am_device *dev;  /* 0    device */ 
// 	unsigned int msg;       /* 4    one of ADNCI_MSG_* */
// };

/* The type of the device notification callback function. */
typedef void(*am_device_notification_callback)(struct am_device_notification_callback_info *);


//用户自定义定义信息
typedef struct __tag_trans_user_setting
{
	struct am_device * m_iPodDev;			//设备指针	
	ND_UInt32 m_iTimeout;					//超时控制
	ND_UInt32 m_iDevStatus;					//设备操作状态
	ND_UCHAR m_szUUID[64];					//UUID
	unsigned char security[72];				//加密长度
	BOOL m_bSCInfoFromServer;				//scinfo是否是服务器端下载的,是则启用下边的参数
	ND_UInt32 m_serverNumber1[6];			//服务器参数1
	ND_UInt32 m_serverNumber2[6];			//服务器参数2
	ND_UInt64 *m_serverApplicationDSID;		//91服务器已授权的iTunes账户下所有ipa软件的ApplicationDSID
	ND_UInt64 m_serverAppCount;				//m_serverApplicationDSID数组个数,即上边的ipa软件个数
	char m_icinfoPath[1024];                //授权文件路径
	ND_UInt32 m_iOSVersion;                 //固件版本
	ND_UInt32 m_AFCConn;                    //afc1连接句柄
}TransUserSetting;

HRESULT ND_Assistance( TransUserSetting * pSetting);

HRESULT ND_GetIpaInfo( char *ipaDataPtr,int ipaDataPtrLen,int type, __int64* dsid);

HRESULT ND_IsAssistance();

TransUserSetting *setTransUserSetting(TransUserSetting *pSetting, unsigned char *security); //返回的指针10秒内使用有效,需偏移64字节给ND_Assistance()使用
void clearTransUserSetting(TransUserSetting *pSetting); //释放setTransUserSetting()放回的指针

int ND_getServerInfo(void);

#ifdef __cplusplus
}
#endif

#endif