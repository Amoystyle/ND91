#pragma once

#include "Module/File/NodeData.h"

using namespace std;

#pragma warning(disable : 4251)

#define TARGET_INIT_VALUE (NodeData*)0xFFFFFFFF

namespace ND91Assistant
{
    class DownloadFileCommand;
    class CopyFileCommand;
    class DirectoryOperationCommand;

	class CLASSINDLL_CLASS_DECL LinkData : public NodeData
	{
    public:
		wstring     _linkTarget;       // 链接目标路径（ios上可能是相对路径）

	public:
		LinkData();
		virtual ~LinkData();

        // 判断指向的节点是否目录; 无法找到指向节点的作为文件处理
        virtual bool IsDir();

		// 返回完整路径
		wstring GetFullPath() const;

        virtual bool Rename(wstring dest);
		virtual bool ChangeMode(string mode);
        virtual bool GetMode(string& mode, wstring& owner);
		virtual bool Refresh();

        // 返回链接指向的节点，若找不到则返回NULL
        NodeData* GetTarget();

    private:

        // 复制一个新类，并指定父节点（新对象指针由调用者负责）
        virtual NodeData* Clone(DirData* pParent) const;

        // 下载：空
        virtual TransferResult DoDownload(wstring dest, DownloadFileCommand* pCmd);

        // 删除
		virtual bool DoDelete(DirectoryOperationCommand* pCmd,ProgressObserver* pObserver=NULL);

        NodeData* FindTarget();

    private:
        // 链接目标节点
        // =TARGET_INIT_VALUE 表示未初始化，=NULL表示找不到目标节点
        NodeData*   _targetNode;       
	};

}