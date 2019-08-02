#pragma once
#include "DeviceInfoDefine.h"

class AblDeviceInfo
{
public:
	AblDeviceInfo()
	{
		bIsActived = true;
		bIsJailBreak = true;		//Ĭ������Խ����
		bIsRoot = true;

		nBattery = 0;
		nSignal = 0;
		nScreenWidth = 0;
		nScreenHeight = 0;
		bInstallOnPhone = false;

		bLoaded = false;

		nSDCardState = eSDCardState_MOUNTED;
	};

	AblDeviceInfo(AblDeviceInfo* pInfo)
	{
		if(pInfo == this)
			return;

		deviceID		= pInfo->deviceID;		
		deviceName		= pInfo->deviceName;	
		strMacAddress	= pInfo->strMacAddress;
						  
		nBattery		= pInfo->nBattery;
		nSignal			= pInfo->nSignal;
		nScreenWidth	= pInfo->nScreenWidth;
		nScreenHeight	= pInfo->nScreenHeight;
						  
		bIsActived		= pInfo->bIsActived;
		bIsJailBreak	= pInfo->bIsJailBreak;			
		bIsRoot			= pInfo->bIsRoot;
		nSDCardState	= pInfo->nSDCardState;
		bWifi			= pInfo->bWifi;
						  
		baseInfo     	= pInfo->baseInfo;     		
		spaceInfo_Phone	= pInfo->spaceInfo_Phone;
		spaceInfo_SDCard= pInfo->spaceInfo_SDCard;


		connectType		= pInfo->connectType;
		bInstallOnPhone = pInfo->bInstallOnPhone;


	};

public:
	wstring deviceID;           // �豸id 
	wstring deviceName;         // �豸����
	wstring strMacAddress;      // MAC��ַ

	int nBattery;                   //����
	int nSignal;                    //�ź�
	int nScreenWidth;				//��Ļ��
	int nScreenHeight;				//��Ļ��

	bool    bIsRoot;                 //�Ƿ�Root
	bool    bIsActived;              //�Ƿ񼤻�
	bool    bIsJailBreak;            //iOs�Ƿ�Խ��    
	bool    bWifi;					 //Wifi�Ƿ��


	Ablconnect_Type connectType;	 // ����ģʽ	
	bool bInstallOnPhone;

	bool bLoaded;

	AblBaseDeviceInfo baseInfo;     //������Ϣ
	AblSDCardState nSDCardState;    //SDCard״̬
	AblSpaceInfoItem spaceInfo_Phone;   // �ֻ��ռ���Ϣ
	AblSpaceInfoItem spaceInfo_SDCard;  // SDCard�ռ���Ϣ
};