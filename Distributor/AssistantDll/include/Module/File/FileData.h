#pragma once

#include "NodeData.h"

namespace ND91Assistant
{
    class ProgressObserver;
    class DownloadFileCommand;
    class DirectoryOperationCommand;

	// 文件系统节点：文件
	class CLASSINDLL_CLASS_DECL FileData : public NodeData
    {
        friend class FileHelper;
        friend class DirData;

        virtual bool IsDir() { return false; }

    public:
        FileData();
        virtual ~FileData();

        // 返回完整路径
        virtual wstring GetFullPath() const;

        // 重命名
		virtual bool Rename(wstring dest);

        // 改权限
		virtual bool ChangeMode(string mode);

        // 取权限
        virtual bool GetMode(string& mode, wstring& owner);

        // 刷新数据
		virtual bool Refresh();

        // 复制一个新类，并指定父节点（新对象指针由调用者负责）
        virtual NodeData* Clone(DirData* pParent) const;

    private:
		
        // 删除(递归时用）
		virtual bool DoDelete(DirectoryOperationCommand* pCmd,ProgressObserver* pObserver=NULL);

        virtual TransferResult DoDownload(wstring dest, DownloadFileCommand* pCmd);
    };

}
