/**************************************************************************/
/*                                                                        */
/* Copyright (c) 1999-2012 NetDragon Websoft Inc.                         */
/*                    �����������޹�˾��Ȩ���� 1999-2012                  */
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

/**************************************************************************/
/*  ����		���� ������                                               */
/*  ��������	���� 2012.05.14                                           */
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


//�û��Զ��嶨����Ϣ
typedef struct __tag_trans_user_setting
{
	struct am_device * m_iPodDev;			//�豸ָ��	
	ND_UInt32 m_iTimeout;					//��ʱ����
	ND_UInt32 m_iDevStatus;					//�豸����״̬
	ND_UCHAR m_szUUID[64];					//UUID
	unsigned char security[72];				//���ܳ���
	BOOL m_bSCInfoFromServer;				//scinfo�Ƿ��Ƿ����������ص�,���������±ߵĲ���
	ND_UInt32 m_serverNumber1[6];			//����������1
	ND_UInt32 m_serverNumber2[6];			//����������2
	ND_UInt64 *m_serverApplicationDSID;		//91����������Ȩ��iTunes�˻�������ipa�����ApplicationDSID
	ND_UInt64 m_serverAppCount;				//m_serverApplicationDSID�������,���ϱߵ�ipa�������
	char m_icinfoPath[1024];                //��Ȩ�ļ�·��
	ND_UInt32 m_iOSVersion;                 //�̼��汾
	ND_UInt32 m_AFCConn;                    //afc1���Ӿ��
}TransUserSetting;

HRESULT ND_Assistance( TransUserSetting * pSetting);

HRESULT ND_GetIpaInfo( char *ipaDataPtr,int ipaDataPtrLen,int type, __int64* dsid);

HRESULT ND_IsAssistance();

TransUserSetting *setTransUserSetting(TransUserSetting *pSetting, unsigned char *security); //���ص�ָ��10����ʹ����Ч,��ƫ��64�ֽڸ�ND_Assistance()ʹ��
void clearTransUserSetting(TransUserSetting *pSetting); //�ͷ�setTransUserSetting()�Żص�ָ��

int ND_getServerInfo(void);

#ifdef __cplusplus
}
#endif

#endif