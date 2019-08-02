#pragma once
#include "Module/File/RootData.h"
#include "Module/File/NodeData.h"
#include "Monitor/ProgressObserver.h"
#include <deque>
#include <string>
#include <vector>

using namespace std;
#ifndef WIN32
#include "Common/winPort.h"
#else
#include <windows.h>
#endif

namespace ND91Assistant
{
    class DeviceData;
	class FileData;
	class LinkData;
    class ListDirectoryCommand;
    class RegexprHelper;
    class UploadFileCommand;
    class BaseCommand;

	class CLASSINDLL_CLASS_DECL DirData : public NodeData
	{
        friend class AFCFileSysemBase;
        friend class IosWiFiFileSystem;
        friend class ListDirectoryCommand;
        friend class RegexprHelper;
		friend class RootData;
	public:
		DirData(const DeviceData* pDevice);
		virtual ~DirData();

        virtual bool IsDir() { return true; }

#pragma region 对设备上的文件执行操作(并修改内存数据)

        // 判断节点名是否存在
        bool IsExist(wstring subName);

        // 重命名
        virtual bool Rename(wstring dest);

        // 改权限
		virtual bool ChangeMode(string mode);

        // 取权限
        virtual bool GetMode(string& mode, wstring& owner);

        // 创建子目录
        bool MakeSubDir(wstring subDirName);

        // 刷新数据
        virtual bool Refresh();

        // Summary  : 移动：即Cut & Paste，将已存在节点移到本目录下
        // Returns  : bool 是否成功
        // Parameter: vector<NodeData*>* pNodes 待移动的节点（移动后就失效）
        // Parameter: ProgressObserver * pObserver 仅使用其GetExistAction方法，无进度数据
        bool Move(vector<NodeData*>* pNodes, ProgressObserver* pObserver);

        // 复制：即Copy & Paste，将文件/目录复制到本目录下
		bool Copy(vector<NodeData*>* pNodes, ProgressObserver* pObserver);

        // 设备上是否存在此目录
        bool ExistInDevice() const;

		// 上传文件
		TransferResult UploadFile(vector<wstring> paths, ProgressObserver* pObserver, bool bSetAttr = true);

        // 上传目录
        TransferResult UploadDir(wstring path, ProgressObserver* pObserver, bool bSetAttr = true);

		TransferResult UploadDir(vector<wstring> paths, ProgressObserver* pObserver,bool bSetAttr);

        // 递归获取所有子目录数据, size=总文件大小，totalCounts=子项总数 
        bool RecursiveLoad(int& totalSize, int& totalCounts, wstring ExtName = L"");

#pragma  endregion

#pragma region 对内存中数据结构执行操作

        // 复制一个新类，并指定父节点（新对象指针由调用者负责）
        virtual NodeData* Clone(DirData* pParent) const;

		// 取完整路径，以"/"结尾（所以根目录的_name应该是空的）
		virtual std::wstring GetFullPath() const;

        // 输入节点是否本目录的上层目录
        bool IsParentDir(const NodeData* pNode) const;

        // 是否根目录
        bool IsRoot() const;

        // 按名字查找子节点
		NodeData* findChildrenbyName(wstring name, bool bFindLinkTarget = true, bool bExactMatch = true);

        // Summary  : 添加一个节点
        // Returns  : 新增的节点指针。若有同名节点添加失败、返回NULL
        // Parameter: const NodeData * pNode 要添加的节点信息
        NodeData* Insert(const NodeData* pNode, bool replace = true);

        // 删除一个子节点
		void RemoveFromChildrens(NodeData* pNode);

        // 是否有子目录
        bool HasSubDir();

        // 取得所有子节点（复制数组，请勿放在循环中）
		virtual deque<NodeData*> GetChildrens();

		// 取得所有子节点（复制数组，请勿放在循环中）
		deque<NodeData*> GetChildrensNoReload(){return _childNodes;}

        // 重命名文件，并返回
        static wstring PhoneFileRename(const wstring name, DirectoryOperationCommand* pCmd);

        // 重命名文件(前加‘复件’)，并返回
        static wstring PhoneFileRenameCopy(const wstring name, DirectoryOperationCommand* pCmd);

		RootData::ROOT_TYPE	GetRootType() { return _eRootType;};

		void SetHasLoad(bool hasLoad) { _hasLoad = hasLoad; }

#pragma endregion

    private:

        // 若未读取目录数据则读取：仅一层，不递归
        void Load();

        // 将指定文件或目录拷贝到本目录下
        bool DoCopy(NodeData* pNode,
            DirectoryOperationCommand* pDirCmd,
            CopyFileCommand* pFileCmd);

        // 将指定目录拷贝到本目录下
        bool DoCopy(DirData* pSourceDir,
            DirectoryOperationCommand* pDirCmd,
            CopyFileCommand* pFileCmd);

        // 将指定文件拷贝到本目录下
        bool DoCopy(FileData* pSourceFile,
            CopyFileCommand* pFileCmd);

        //删除
        virtual bool DoDelete(DirectoryOperationCommand* pCmd,ProgressObserver* pObserver=NULL);

        // 获取目录数据：仅一层，不递归
        bool DoLoad(ListDirectoryCommand* pCmd);

        // 递归获取所有子目录数据, size=总文件大小，totalCounts=子项总数 
        bool DoRecursiveLoad(int& totalSize, int& totalCounts, ListDirectoryCommand* pCmd, wstring ExtName = L"");

        // 下载到指定目录
        virtual TransferResult DoDownload(wstring destDir, DownloadFileCommand* pCmd);

        // 上传文件
        TransferResult DoUploadFile(wstring path, UploadFileCommand* pFileCmd, DirectoryOperationCommand* pDirCmd);

        // 上传目录
        TransferResult DoUploadDir(wstring path, UploadFileCommand* pFileCmd, DirectoryOperationCommand* pDirCmd);

        // 设备上存在此目录时重命名
        wstring PhoneDirRename(const wstring name);

        // 设备上存在此目录时重命名(前加‘复件’)
        wstring PhoneDirRenameCopy(const wstring name);

        // 新增链接子节点（读取目录内容时用）
        LinkData* AddLink();

        // 新增文件子节点（读取目录内容时用）
        FileData* AddFile();

        // 新增目录子节点（读取目录内容时用）
        DirData* AddDir();

        // 清除子节点
        void ClearChilds();

        // 检查是否存在同名目录，无则创建（DoCopy中使用）
        DirData* CheckSubDir(DirectoryOperationCommand* pDirCmd,
            BaseCommand* pCommand, wstring name);

        // 用给定的命令创建子目录
        bool DoMakeSubDir(wstring subDirName, DirectoryOperationCommand* pCmd);

        // Summary  : 给新建的节点设置属性
        // Parameter: NodeData * pNode 节点指针
        // Parameter: wstring name 节点名
        void SetNodeAttribute( NodeData* pNode, wstring name);

        // Summary  : 执行Move操作
        // Returns  : 是否成功
        // Parameter: NodeData * pNode 待移动的节点
        // Parameter: DirectoryOperationCommand * pCmd 命令指针
        bool DoMove(NodeData* pNode, DirectoryOperationCommand* pCmd);

        // 上传文件名/文件夹名字数限制
        void FileNameCheck( wstring &filename );

    public:
        const DeviceData*   _pDeviceData;

    private:
		// 内部类：方便加锁用
		class MyLock
		{
		public:
			MyLock(LPCRITICAL_SECTION	pLock) : _pLock(pLock) {
				EnterCriticalSection(pLock);
			}
			~MyLock() {
				LeaveCriticalSection(_pLock);
			}
		private:
			LPCRITICAL_SECTION	_pLock;
		};
		CRITICAL_SECTION	_lock;
		bool			    _hasLoad;
		RootData::ROOT_TYPE	_eRootType;

#pragma warning(disable : 4251)     // 禁用此警告
        deque<NodeData*> _childNodes;	// 子节点
#pragma warning(default : 4251)     // 恢复

	protected:
		void SetRootType(RootData::ROOT_TYPE eRootType);
	};

}
