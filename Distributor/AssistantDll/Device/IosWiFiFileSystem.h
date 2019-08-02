#pragma once

#include "Monitor/ProgressSubject.h"
#include "IIosFileSystem.h"

namespace ND91Assistant
{
	class IosWiFiFileSystem : public IIosFileSystem
	{
	public:
		IosWiFiFileSystem(int nhandle);
		~IosWiFiFileSystem(void);

		/// 当前文件系统能访问到根目录
		virtual string Ios_GetRootDirectory(eFileSystemType eType);

		virtual bool Ios_Rename(wstring srcPath, wstring dstPath);

		virtual bool Ios_CreateDirectory(wstring strPath);

		virtual bool Ios_DeleteEmptyDirectory(wstring strPath);	

		virtual bool Ios_DeleteFile(wstring strPath);

		virtual bool Ios_Exists(wstring strPath);

		virtual bool Ios_IsDirectory(wstring strPath);

		virtual long Ios_FileSize(wstring strPath);

		//获取指定文件夹下的所有文件信息，但不包括子目录下的信息
		virtual bool Ios_GetFileInfos(DirData* pDir, eFileGetType eType);

		//上传文件
		virtual TransferResult Ios_CopyToPhone(wstring sourceOnComputer, wstring destinationOnPhone, ProgressSubject& pSubject = NullProgressSubject::Instance());

		//下载文件
		virtual TransferResult Ios_DownFromPhone(wstring sourceOnPhone, wstring destinationOnComputer, ProgressSubject& pSubject = NullProgressSubject::Instance());

         // 在USB连接状态下直接往设备上读写数据(目前是图片管理读写缩略图数据库文件在用)
        virtual int Ios_OpenFile(wstring strPath, string& buffer);

		virtual TransferResult Ios_CopyToPhoneEx(wstring sourceOnComputer, wstring destinationOnPhone, 
			unsigned __int64 OffSetAppInExe, unsigned __int64 len,
			ProgressSubject& subject = NullProgressSubject::Instance());

        // Summary  : 获取设备上文件SHA1值,该方法需要文件存在同时文件不能为空
        //大于500MB以上文件会耗时很长,直至超时.可以考虑用GetFileSHA1WithRange方法
        // Returns  : string 成功20字节SHA1值
        // Parameter: wstring strPath 文件路径
        virtual string Ios_GetFileSHA1(wstring strPath);

        // Summary  : 获取设备上文件 从文件头到指定结尾的SHA1值,该方法需要文件存在同时文件不能为空
        // Returns  : string 成功20字节SHA1值
        // Parameter: wstring strPath 文件路径
        // Parameter: long endPosition endPosition不能小于0或者大于文件大小
        virtual string Ios_GetFileSHA1WithRange(wstring strPath, long endPosition);

	private:
		bool Ios_GetFiles(DirData* pDir);
		bool Ios_GetDirectories(DirData* pDir);

		TransferResult DoCopyToPhone(wstring local_path, wstring remote_path, ProgressSubject& subject);
		TransferResult DoDownFromPhone(wstring remote_path, wstring local_path, ProgressSubject& subject);

        // 解析 WiFiGetFiles、WiFiGetDirectories 的返回结果，生成pDir的子节点
        void ParseResult(char* szBuff, int nLen, DirData* pDir, bool isFile);

	private:
		int _nHandle;
	};
}
