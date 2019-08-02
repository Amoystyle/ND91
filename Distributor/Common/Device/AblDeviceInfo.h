#pragma once
#include "DeviceInfoDefine.h"

class AblDeviceInfo
{
public:
	AblDeviceInfo()
	{
		bIsActived = true;
		bIsJailBreak = true;		//默认是已越狱的
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
	wstring deviceID;           // 设备id 
	wstring deviceName;         // 设备名称
	wstring strMacAddress;      // MAC地址

	int nBattery;                   //电量
	int nSignal;                    //信号
	int nScreenWidth;				//屏幕宽
	int nScreenHeight;				//屏幕高

	bool    bIsRoot;                 //是否Root
	bool    bIsActived;              //是否激活
	bool    bIsJailBreak;            //iOs是否越狱    
	bool    bWifi;					 //Wifi是否打开


	Ablconnect_Type connectType;	 // 连接模式	
	bool bInstallOnPhone;

	bool bLoaded;

	AblBaseDeviceInfo baseInfo;     //基本信息
	AblSDCardState nSDCardState;    //SDCard状态
	AblSpaceInfoItem spaceInfo_Phone;   // 手机空间信息
	AblSpaceInfoItem spaceInfo_SDCard;  // SDCard空间信息
};