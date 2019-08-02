#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"

namespace ND91Assistant
{
#define PXL_NORMAL_PATH		L"/private/var/root/Media/PXL/Dropoff/ndapp.pxl"

	//PXL类型的APP的操作类
	class IosPXLAppOperation : public IIosAppOperation
	{
	public:
		// Summary  : 构造函数
		// Returns  : 
		// Parameter: const DeviceData* pDevice ：设备
		// Parameter: ProgressSubject & subject	：进度回调SUBJECT
		IosPXLAppOperation(const DeviceData* pDevice, ProgressSubject& subject);
		~IosPXLAppOperation(void);

		//获取指定类型的所有程序的信息列表
		//eType		: 应用程序类型
		virtual bool GetAppInfos(vector<AppData*> &vtInfos);

		//安装PXL程序
		//strAppFullName: 应用程序PC上的文件名（含路径），非PXL文件返回FALSE
        //string& errorMsg 安装失败返回错误信息
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo=NULL);

		//卸载PXL程序
		//strIdentifier	: 为AppData里的_PackageName
		virtual bool UnInstallApp(wstring strIdentifier);

		//备份PXL程序
		//strIdentifier	: 为AppData里的_PackageName
		//strAppPhonePath ：为AppData里的_strAppPhonePath
		//strAppName ：为AppData里的_AppName
		//strVersion ：为AppData里的_strVersion
		//eMode			: 备份类型
		//strFileOnPc	: 备份文件存放在PC上的位置（路径+名），为扩展名为PXL
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc);

		// Summary  : 单纯获取应用的数量
		// Returns  : int 返回应用数量，无或失败均返回0
		virtual int GetAppCount(APP_OWNER appOwner);

	private:		

		// Summary  : 检测安装包是否符合要求
		// Returns  : bool 可以安装返回TRUE,否则为FALSE
		// Parameter: wstring strAppFullName ：包名（含路径）
		// Parameter: wstring & strIdentifier ：应用程序的唯一标识
		// Parameter: wstring & strUnZipPath  ：解压后的路径
		// Parameter: bool b7z  ：是否为7z文件
		bool CheckInstallPkg(wstring strAppFullName, wstring &strIdentifier, wstring &strUnZipPath);

		// Summary  : 上传普通压缩格式的程序包
		// Returns  : bool 成功返回TRUE
		// Parameter: wstring strAppFullName ：程序包名（含路径）
		bool UpLoadNormalApp(wstring strAppFullName,__int64 AppOffsetInExe=0,unsigned __int64 AppSize=0);		
	};
}
