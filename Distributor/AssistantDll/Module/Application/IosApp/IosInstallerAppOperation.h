#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"

namespace ND91Assistant
{
	//Installer类型的APP的操作类
	class IosInstallerAppOperation : public IIosAppOperation
	{
	public:
		// Summary  : 构造函数
		// Returns  : 
		// Parameter: const DeviceData* pDevice ：设备
		// Parameter: ProgressSubject & subject	：进度回调SUBJECT
		IosInstallerAppOperation(const DeviceData* pDevice, ProgressSubject& subject);
		~IosInstallerAppOperation(void);

		//获取指定类型的所有程序的信息列表
		//eType		: 应用程序类型
		virtual bool GetAppInfos(vector<AppData*> &vtInfos);

		//安装Installer程序
		//strAppFullName: 应用程序PC上的文件名（含路径），非Installer文件返回FALSE
        //string& errorMsg 安装失败返回错误信息
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo=NULL);

		//卸载Installer程序
		//strIdentifier	: 为AppData里的_PackageName
		virtual bool UnInstallApp(wstring strIdentifier);

		//备份Installer程序
		//strIdentifier	: 为AppData里的_PackageName
		//strAppPhonePath ：为AppData里的_strAppPhonePath
		//strAppName ：为AppData里的_AppName
		//strVersion ：为AppData里的_strVersion
		//eMode			: 备份类型
		//strFileOnPc	: 备份文件存放在PC上的位置（路径+名），为扩展名为Installer
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc);

		// Summary  : 单纯获取应用的数量
		// Returns  : int 返回应用数量，无或失败均返回0
		virtual int GetAppCount(APP_OWNER appOwner);

	private:
		// Summary  : 解析全部程序信息
		// Returns  : void
		// Parameter: wstring destinationOnComputer ：信息存放的文件名（含路径，在PC上）
		// Parameter: vector<AppData * > & vtInfos ：用于存放程序信息
		void AnalysisInstallerFile(wstring destinationOnComputer, vector<AppData*> &vtInfos);
		// Summary  : 解析单个程序信息
		// Returns  : void
		// Parameter: wstring strXML ：程序信息XML内容
		// Parameter: AppData * pAppData ：用于存储程序信息
		void SetInstallerAppData(wstring strXML, AppData* pAppData);

	};
}
