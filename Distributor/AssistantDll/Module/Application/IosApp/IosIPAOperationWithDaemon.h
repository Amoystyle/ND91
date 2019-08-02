#pragma once

#include "Device/IIosFileSystem.h"
#include "Monitor/ProgressSubject.h"
#include "Module/Application/IosApp/IIosAppOperation.h"
#include <vector>

using namespace std;

namespace ND91Assistant
{
	#define NDIPAINSTALLPATH		L"/private/var/mobile/Library/NetDragon/ipa/"
	#define NDIPAINSTALLFULLNAME	L"/private/var/mobile/Library/NetDragon/ipa/ndapp.ipa"

	class  App2ExeInfo;
	//IPA类型的APP的操作类，通过守护实现
	class IosIPAOperationWithDaemon : public IIosAppOperation
	{
	public:
		// Summary  : 构造函数
		// Returns  : 
		// Parameter: const DeviceData* pDevice ：设备
		// Parameter: ProgressSubject & subject	：进度回调SUBJECT
		IosIPAOperationWithDaemon(const DeviceData* pDevice, ProgressSubject& subject);
		~IosIPAOperationWithDaemon(void);

		//获取指定类型的所有程序的信息列表
		//eType		: 应用程序类型
		virtual bool GetAppInfos(vector<AppData*> &vtInfos);

		//安装IPA程序
		//strAppFullName: 应用程序PC上的文件名（含路径），非IPA文件返回FALSE
        //string& errorMsg 安装失败返回错误信息
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo=NULL);

		//卸载IPA程序
		//strIdentifier	: 为AppData里的_PackageName
		virtual bool UnInstallApp(wstring strIdentifier);

		//备份IPA程序
		//strIdentifier	: 为AppData里的_PackageName
		//strAppPhonePath ：为AppData里的_strAppPhonePath
		//strAppName ：为AppData里的_AppName
		//strVersion ：为AppData里的_strVersion
		//eMode			: 备份类型
		//strFileOnPc	: 备份文件存放在PC上的位置（路径+名），为扩展名为IPA
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc);	

		// Summary  : 单纯获取应用的数量
		// Returns  : int 返回应用数量，无或失败均返回0
		virtual int GetAppCount(APP_OWNER appOwner);
	};
}
