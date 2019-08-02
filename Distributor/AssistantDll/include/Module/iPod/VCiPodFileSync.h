#pragma once

#ifdef _ASSISTANT_DLL  // 在iPodManagerDll中不需要此类

#include "Core/DeviceData.h"
#include "Device/IIosFileSystem.h"

namespace ND91Assistant
{
	//ipod管理文件同步操作类
	class CLASSINDLL_CLASS_DECL iPodFileSync
	{
	public:
		//初始化文件同步
		static void InitFileSync(DeviceData* Device,wstring sqlDbPath);
		//上传cdb文件
		static void UpLoadCDB();
		//上传封面文件跟lib数据库文件
		static void UpdateDevItunesDB();
		//上传播放列表相关的文件
		static void UploadPlayListDB(wstring SrcFile);
		//下载音乐
		static void DownLodMusic(wstring localfile,wstring remotfile);
		//上传新建音乐相关文件
		static void UpLoadFile(wstring localfile,wstring remotfile);
		//删除音乐
		static void DeleteMusicFile( wstring filename);
		//获得ios连接afc句柄
		static int GetAFCHandle();
		//获得设备uuid
		static wstring GetUUID();
		//获得固件版本号
		static int GetDBVer();
		//获得设备文件目录的路径
		static wstring GetDevicePath(wstring FileName);
	private:
		//设备句柄
		static DeviceData* _pDevice;	
		//本地itunes_control目录路径
		static wstring _sqlDbPath;
		//设备上itunes_control目录的路径
		static wstring DeviceiTunsPath;

		static wstring _UUID;
	};
}

#endif