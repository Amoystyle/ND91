#pragma once

#include "../Monitor/ProgressSubject.h"
#include "../Data/AppData.h"
#include "IosAppOperation/IIosAppOperation.h"
#include <vector>
#include <map>

using namespace std;

namespace ND91Assistant
{
	
#define ApplicationArchivesFolder L"ApplicationArchives" //备份文件存储的位置

	/************************************************************************/
	/* IOS设备USB模式下应用程序的操作
	包括获取所有程序列表、安装、卸载和备份,由于进度暂时无法区分归属，故暂只做单例处理
	WIFI模式只能通过守护安装*/
	/************************************************************************/
	class IosUsbAppOperation
	{
	public:
		static IosUsbAppOperation* GetInstance();

	private:
		//pDevPtr	: USB模式下连接/断开时候返回的设备句柄
		IosUsbAppOperation(void);
		~IosUsbAppOperation(void);

	public:

		ProgressSubject& GetSubject() {return *_progressSubject;}

		//获取指定类型的所有程序的信息列表
		//pDevPtr	: 为DeviceInfo里的_ios_devptr
		//eType		: 应用程序类型
		bool GetAppInfos(void* pDevPtr, eApplicationType eType, vector<AppData*> &vtInfos);

		//安装IPA程序
		//pDevPtr		: 为DeviceInfo里的_ios_devptr
		//pAfcSocketApp	: 为DeviceInfo里的_ios_afcSocketApp
		//pAfcHandleApp	: 为DeviceInfo里的_ios_afcHandleApp
		//strAppFullName: 应用程序PC上的文件名（含路径），非IPA文件返回FALSE
		bool InstallApp(void* pDevPtr, void* pAfcSocketApp, void* pAfcHandleApp, wstring strAppFullName, ProgressSubject& subject = NullProgressSubject::Instance());

		//卸载IPA程序
		//pDevPtr		: 为DeviceInfo里的_ios_devptr
		//strIdentifier	: 为AppData里的_Identifier
		bool UnInstallApp(void* pDevPtr, wstring strIdentifier);

		//备份IPA程序
		//pDevPtr		: 为DeviceInfo里的_ios_devptr
		//pAfcHandleApp	: 为DeviceInfo里的_ios_afcHandleApp
		//strIdentifier	: 为AppData里的_Identifier
		//eMode			: 备份类型
		//strFileOnPc	: 备份文件存放在PC上的位置（路径+名），为扩展名为IPA
		bool ArchiveApp(void* pDevPtr, void* pAfcHandleApp, wstring strIdentifier, eArchiveMode eMode, wstring strFileOnPc, ProgressSubject& subject = NullProgressSubject::Instance());

	private:
		bool GetAppInfos(void* pDevPtr, eApplicationType eType, map<std::string, void*> &mapInfos);
		void PraseAppInfos(map<std::string, void*> mapInfos, vector<AppData*> &vtInfos);

		//将程序备份在设备上
		bool ArchiveAppOnPhone(void* pDevPtr, wstring strIdentifier, eArchiveMode eMode, bool bUnInstall = false);
		//删除备份
		bool RemoveAppArchiveOnPhone(void* pDevPtr, wstring strIdentifier);

		//安装APP进度回调
		friend void OnPackInstallCallBack(void* pSender, void* pEvent);
		//使用ITUNES默认上传APP包的进度回调
		friend void OnPackDataTransferCallBack(void* pEvent);
		//删除APP备份的进度回调
		friend void OnRemoveIPAArchiveCallback(void* pSender, void* pEvent);
		//备份APP的进度回调
		friend void OnArchiveIPACallback(void* pSender, void* pEvent);

		friend void PraseProgress(void* pCFData);

	private:
		static IosUsbAppOperation* _pInstance;
		ProgressSubject* _progressSubject;
	};
}
