#pragma once

#include "Module/File/FileData.h"
#include <vector>
#include "Common/StrOperation.h"
#include "Monitor/ProgressSubject.h"

using namespace std;

namespace ND91Assistant
{
    class FileData;
    class DirData;

	typedef enum eFileSystemType
	{
		//普通文件系统
		eFileSystemType_Normal,
		//媒体文件系统
		eFileSystemType_Media,
		//应用程序系统
		eFileSystemType_Application,
	};


	typedef enum eFileGetType
	{
		eFileGetType_Files,
		eFileGetType_Dirs,
		eFileGetType_All
	};

	class IIosFileSystem
	{
	public:
		/// 当前文件系统能访问到根目录
		virtual string Ios_GetRootDirectory(eFileSystemType eType) = 0;

		virtual bool Ios_Rename(wstring srcPath, wstring dstPath) = 0;

		virtual bool Ios_CreateDirectory(wstring strPath) = 0;

		virtual bool Ios_DeleteEmptyDirectory(wstring strPath) = 0;	

		virtual bool Ios_DeleteFile(wstring strPath) = 0;

		virtual bool Ios_Exists(wstring strPath) = 0;

		virtual bool Ios_IsDirectory(wstring strPath) = 0;

		virtual long Ios_FileSize(wstring strPath) = 0;

		//获取指定文件夹下的所有文件信息，但不包括子目录下的信息
		virtual bool Ios_GetFileInfos(DirData* pDir, eFileGetType eType) = 0;

		//上传文件或文件夹
		virtual TransferResult Ios_CopyToPhone(wstring sourceOnComputer, wstring destinationOnPhone, ProgressSubject& pSubject = NullProgressSubject::Instance()) = 0;

		//下载文件或文件夹
        virtual TransferResult Ios_DownFromPhone(wstring sourceOnPhone, wstring destinationOnComputer,ProgressSubject& pSubject = NullProgressSubject::Instance()) = 0;

        // 在USB连接状态下直接往设备上读写数据(目前是图片管理读写缩略图数据库文件在用)
        virtual int Ios_OpenFile(wstring strPath, string& buffer) = 0;

		virtual TransferResult Ios_CopyToPhoneEx(wstring sourceOnComputer, wstring destinationOnPhone, 
			unsigned __int64 OffSetAppInExe, unsigned __int64 len,
			ProgressSubject& subject = NullProgressSubject::Instance()) = 0;

        // Summary  : 获取设备上文件SHA1值,该方法需要文件存在同时文件不能为空
        //大于500MB以上文件会耗时很长,直至超时.可以考虑用GetFileSHA1WithRange方法
        // Returns  : string 成功20字节SHA1值
        // Parameter: wstring strPath 文件路径
        virtual string Ios_GetFileSHA1(wstring strPath) = 0;

        // Summary  : 获取设备上文件 从文件头到指定结尾的SHA1值,该方法需要文件存在同时文件不能为空
        // Returns  : string 成功20字节SHA1值
        // Parameter: wstring strPath 文件路径
        // Parameter: long endPosition endPosition不能小于0或者大于文件大小
        virtual string Ios_GetFileSHA1WithRange(wstring strPath, long endPosition) = 0;

    public:

        bool SkipFile(wstring file);

        // 获取节点在界面显示的名称
        void GetDisplayName(NodeData* pNode);

	};

}
