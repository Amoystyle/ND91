#pragma once

#include "AFCFileSysemBase.h"

namespace ND91Assistant
{
	class DeviceData;

	class IosDaemonHelper
	{
	public:
		IosDaemonHelper(const DeviceData* pDevice);
		~IosDaemonHelper(void);

		//strDaemonZipFile:守护的zip压缩包
		bool InstallDaemon(wstring strDaemonZipFile, wstring daemonVersionOnService);
		
		//调用之前需先执行停止守护进程"TQServer"操作
        bool UnInstallDaemon();

    private:
		//strDeamonFolder守护zip包解压后的路径
        bool InstallDaemonByFolder(wstring strDeamonFolder, bool isNew);

        void ClearOldDeamonFile();
		void InitRun(int nVerNum);
		void BackFile(wstring strDeamonFolder, bool isNew);
		void UpdateFile(wstring strDeamonFolder);		
        // 更新 守护的PList文件
        bool UpdateInfoPList(wstring strDeamonFolder);

		bool StartIosServer(wstring strDeamonFolder);



        // 上传文件列表
        void UploadFile(bool isNew);

        // 4.x以后固件使用
        bool InstallDaemo_New();// 新守护安装方式
        // 3.x及以前固件使用
        bool InstallDaemo_Old();// 旧守护安装方式

        // 将守护版本号写入到手机
        bool WriteTQServerVersion();

        // 启动守护
        bool StartDaemon();

	private:
		AFCFileSysemBase* _pFileSteam;
		wstring _strRunFile;
		wstring _strRunPlist;
		wstring _strSerialNum;
        wstring _strDaemonFolderTemp;// 守护解压在本地的临时目录
        wstring _daemonVersionOnService;// 守护在服务器上的版本号

		const DeviceData* _pDevice;
	};
}
