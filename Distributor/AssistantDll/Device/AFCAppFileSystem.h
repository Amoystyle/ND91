#pragma once

#include "AFCFileSysemBase.h"

namespace ND91Assistant
{
	// 实现与com.apple.mobile.house_arrest服务通讯 固件3.x开始才有这个功能，
	// 低版本的应用程序中_bShare均为FALSE，调用着可根据_FileSharingEnabled的值判断是否开启此服务(为TURE时才有必要开启)
	// 暂只有USB模式有实现
	class AFCAppFileSystem : public AFCFileSysemBase
	{
	public:

		// 启动某个应用程序的共享文档目录服务
		// strIdentifier	: 该应用程序标识符
		static AFCAppFileSystem* StartAPPHouseArrestService(wstring strIdentifier, DeviceInfo* devInfo);

		/// 当前文件系统能访问到根目录，程序的共享文件均在此目录下
		//eType	: 传任意值即可
		//返回	：程序共享文件根目录，做上传下载等操作的时候均需要在此根目录下
		virtual string Ios_GetRootDirectory(eFileSystemType eType);		

	private:
		AFCAppFileSystem(void* pAfcHandle, const DeviceInfo* pDevInfo);
		~AFCAppFileSystem();
	};
}
