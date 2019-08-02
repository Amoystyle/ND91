#pragma once

#include "Core/DeviceInfo.h"
#include "Monitor/ProgressSubject.h"
#include "IIosFileSystem.h"

#include <map>

namespace ND91Assistant
{
    class LinkData;

	struct sLine{
		string strValue;
		sLine* pNext;
	};

    // 创建超链接模式
    enum LinkMode
    {
        Hard    = 1,    // 硬链接模式,即拷贝一份到链接所在目录
        Symbol  = 2,    // 符号...
    };

    // 调用iTunesMobileDevice.dll库函数，完成文件相关操作
    // 该库函数的方法，大多数是返回0表示成功
	class AFCFileSysemBase : public IIosFileSystem
	{
	public:
		// Summary  : 构造函数
		// Returns  : 
		// Parameter: void * pAfcHandle ：对应_ios_afcHandle
		// Parameter: void * pAfcHandleApp ：对应_ios_afcHandleApp
		// Parameter: bool bJailBreaked ：对应_ios_bJailBreaked
		AFCFileSysemBase(const DeviceInfo* pDevInfo);
        AFCFileSysemBase(void* pAfcHandle, const DeviceInfo* pDevInfo);// 应用文件系统调用（如应用共享文档,IPA安装/备份,video管理）
		~AFCFileSysemBase(void);

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
		virtual TransferResult Ios_CopyToPhone(wstring sourceOnComputer, wstring destinationOnPhone, ProgressSubject& subject = NullProgressSubject::Instance());

		//下载文件
		virtual TransferResult Ios_DownFromPhone(wstring sourceOnPhone, wstring destinationOnComputer, ProgressSubject& subject = NullProgressSubject::Instance());

        // 在USB连接状态下直接往设备上读写数据(目前是图片管理读写缩略图数据库文件在用)
        virtual int Ios_OpenFile(wstring strPath, string& buffer);

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

        // Opens a File for reading 
        int OpenRead(wstring strPath, string& buffer);
        // Opens a File for writing 
        bool OpenWrite(wstring strPath, string buffe);

		virtual TransferResult Ios_CopyToPhoneEx(wstring sourceOnComputer, wstring destinationOnPhone, 
			unsigned __int64 OffSetAppInExe, unsigned __int64 len,
			ProgressSubject& subject = NullProgressSubject::Instance());

        // Summary  : 建立链接
        // Returns  : bool
        // Parameter: wstring target 要建立链接的路径 target 路径不以/开始 才能成功,不知道是否是bug
        // Parameter: wstring link 链接
        // Parameter: LinkMode mode
        bool LinkPath(wstring target, wstring link, LinkMode mode);

        // Summary  : 获取最近一次AFC操作失败信息.类似WIN32 GetLastError方法TODO:有些异常是需要重启AFC服务才可以的
        // Returns  : string
        string GetAFCLastErrorMsg();

        // Summary  : 判断下AFC连接是否正常,如果不正常,本方法会尝试重启AFC服务.
        // <remarks>由于一些操作会导致AFC挂掉,因此后续需要afc的地方可能需要重启afc服务.
        // 比如安装压缩包格式不规范 ND591up0712.ipa后会导致AFC服务挂掉,因此后续的安装需要重启AFC服务
        // 又未越狱设备安装破解版软件</remarks>
        // Returns  : bool
        bool IsAFCConnectionValid();

        // Summary  : 启动AFC文件系统服务服务,如果未越狱设备,则启动AFC2服务会失败
        // Returns  : bool 是否成功
        bool StartAFCFileSystem();

        // 停止AFC文件系统服务
        void StopAFCFileSystem();

	private:

		TransferResult DoCopyToPhone(wstring local_path, wstring remote_path, ProgressSubject& subject,
			unsigned __int64 OffSetAppInExe=0, unsigned __int64 len=0);
		TransferResult DoDownFromPhone(wstring remote_path, wstring local_path, ProgressSubject& subject);

        // 生成目录下某节点，对旧版本
        NodeData* GetFileInfo(DirData* pDir, wstring name, eFileGetType eType);

        // 生成目录下某节点，对新版本
        NodeData* FileInfoOpen( DirData* pDir, wstring name, eFileGetType eType);

        // 根据文件信息创建节点
        NodeData* CreateNode(DirData* pDir, wstring name, std::map<string, string>& dict, eFileGetType eType);

        // 复制文件信息（调用者须删除返回的对象）
		NodeData* GetFileNode(wstring path);

	private:
		void*		_pAFCHandle;
		void*		_pAFCHandleApp;
		bool		_bJailBreaked; //是否越狱

        void*		_pAFCSocket;
        DeviceInfo*	_pDevInfo;
	};
}
