#pragma once

#include "BaseIO.h"
#include "AFCFileSysemBase.h"
#include "IosWiFiFileSystem.h"

namespace ND91Assistant
{
    // ios 文件收发类
    class IosFileIO : public BaseIO
    {
    public:
        IosFileIO(const DeviceInfo* pDevInfo);
		IosFileIO(int nHandle);

		virtual ~IosFileIO();

		// 发送命令到ios设备
		// file: 命令文件（完整路径）
		// 返回：结果文件（完整路径）
        virtual std::string SendAndroidDatagram(std::string filename, bool bReceiveResult = true);

        // summary  : 发送ios命令并返回结果
        // return   : std::wstring						: 结果文件名(unicode)
        // parameter: std::wstring strCmdFileOnPhone	: 上传至设备上的命令文件名(全路径，unicode)
		// parameter: std::wstring strResultFileOnPhone	: 下载至PC上的结果文件名(全路径，unicode)
		// parameter: std::wstring strCmdFileContent	: 命令文件内容(unicode)
		// parameter: bool bReceiveResult				: true=接收返回值 false=不接收返回值
        virtual std::wstring SendIosFile(std::wstring strCmdFileOnPhone, std::wstring strResultFileOnPhone, 
			std::wstring strCmdFileContent, bool bReceiveResult = true);

		IIosFileSystem* GetIosFileStream(){return _pFileStream;}

		//删除IOS所有命令返回文件(即.done文件)
		void DeleteAllCmdDoneFile();

		void SetWaitTime(unsigned long nWaitTime){_nWaitTime = nWaitTime;}
		int GetWaitTime(){return _nWaitTime;}

	private:
		// Summary  : 创建IOS命令文件（PC端）
		// Returns  : std::wstring						：命令文件名（含路径）
		// Parameter: std::wstring strCmdFileName		：命令文件名
		// Parameter: std::wstring strCmdFileContent	：命令文件内容
		std::wstring CreateIosCommandFile(std::wstring strCmdFileName, std::wstring strCmdFileContent);

        // 创建目录
        void CreateDirectory( wstring path );

    protected:
		// 设备上的命令文件，每种命令可能不一样
		std::wstring _cmdFileOnPhone;

		//设备上的结果文件，每种命令可能不一样
		std::wstring _resultFileOnPhone;

	private:
		IIosFileSystem* _pFileStream;

		unsigned long _nWaitTime;	//等待返回的时间
    };
}
