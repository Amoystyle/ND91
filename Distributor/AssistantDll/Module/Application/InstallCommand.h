#pragma once

#include "Module/BaseCommand.h"
#include <vector>

#include "Monitor/AppProgressObserver.h"

using namespace std;
namespace ND91Assistant
{
    class DeviceData;
    class AppData;
	class App2ExeInfo;

    //安装程序到手机中
    class InstallCommand : public BaseCommand
    {
    public:
		//安装步骤
		enum APP_INSTALL_STEP
		{
			NONE,			//未开始
			PREPARE,		//准备数据， 如NPK需要先解压
			UPLOAD,			//成功
			INSTALL,		//失败
			CONFIRM,		//安装过程需要用户在手机上确认
			FINISH,			//完成
			FAIL,			//失败
		};
        // Summary  : 
        // Returns  : 
        // Parameter: const DeviceData * pDevice
        // Parameter: const AppData * appData 要安装的程序
        InstallCommand(const DeviceData* pDevice, const AppData* appData);
        virtual ~InstallCommand();

		void SetParam(App2ExeInfo* exeInfo)
		{
			_exeInfo = exeInfo;
		}

        // 执行命令
        virtual void Execute();

        // Summary  : 安装一个App 的简单调用方法，静态，用于简单的调用安装程序
        // Returns  : bool 是否安装成功
        // Parameter: const DeviceData * pDevice 设备
        // Parameter: wstring strAppPath 安装程序在PC中的位置
        // Parameter: wstring strPackageName 安装程序在手机中的包名(唯一标识)
        // Parameter: APP_TYPE appType 安装程序类型
        static bool SimpleInstallApp(const DeviceData* pDevice, wstring strAppPath, wstring strPackageName, APP_TYPE appType);

		// 生成Android数据体，
        virtual std::string GetAndroidBodyData();

		// 解析返回数据包
        virtual void SetAndroidReturnData(std::string data);

		// 解析特殊的返回包头
		bool parseAndroidPackageHead( BinaryBufferReader* pReader );

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // 生成ios文件。返回文件名，调用者负责删除文件
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // 解析返回文件，解析后删除文件
        virtual  void SetIosReturnFile(std::wstring filename) {}

 private:

	 //安装exe类型的安装包
	 bool InstallApkInExe(wstring strPCPath,unsigned __int64 offsetAppInExe,unsigned __int64 len);
		// Summary  : 安装PC中的Apk
		// Returns  : bool 是否安装成功
		// Parameter: wstring strPCPath 安装程序在PC中的位置
		// Parameter: wstring strPackageName 安装程序在手机中的包名(唯一标识)
		bool InstallApk(wstring strPCPath, wstring strPackageName);

        // Summary  : 安装PC中的Npk
        // Returns  : bool 是否安装成功
        // Parameter: wstring strPCPath 安装程序在PC中的位置
        bool InstallNpk(wstring strPCPath);

	private:
		// Summary  : 安装手机上的Apk
		// Returns  : bool 是否安装成功
		// Parameter: wstring strPhonePath 安装程序在手机中的位置
		// Parameter: wstring strPkgName 安装程序在手机中的包名(唯一标识)
		bool InstallRemoteApk(wstring strPhonePath, wstring strPkgName);

	private:
		const AppData* _appData;
		APP_INSTALL_STEP _step;	// 安装结果
		wstring _appRemotePath;	// 安装程序上传到手机中的位置

	private:
		bool _bLock;		// 安装时是否锁定
		bool _bRecover;		// 安装是否覆盖
		int  _bOnSDCard;	// 是否安装在SDCard上 2.2有效 1=SDCard 2:非SDCard 3未设置（默认）
        bool _isMove;
		//exe类型安装包的安装内容类型
		App2ExeInfo* _exeInfo;

        wstring _errorMsg; // 安装失败返回错误信息
	public:
		void SetLock(bool b) {_bLock = b;};
		void SetRecover(bool b) {_bRecover = b;};
        void SetOnSDCard(bool b) {_bOnSDCard = b ? 1 : 2;};
        void SetIsMove(bool b) { _isMove = b; }

        wstring GetErrorMsg() { return _errorMsg; }
    };
}
