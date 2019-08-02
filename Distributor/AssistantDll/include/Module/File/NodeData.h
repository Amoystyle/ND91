#pragma once

#include "AssistantDll.h"
#include <string>

using namespace std;

#pragma warning(disable:4251)

namespace ND91Assistant
{
	class DirData;
    class DeviceData;
    class CopyFileCommand;
    class DownloadFileCommand;
	class DirectoryOperationCommand;
	class ProgressObserver;
	// 文件系统节点
	// 子类有目录、文件、链接等类型
    class CLASSINDLL_CLASS_DECL NodeData
    {
        friend class DirData;
        friend class RootData;
        friend class FileHelper;

    public: // 成员变量

        // 节点类型
        enum NODE_TYPE {
            DIR,
            FILE,
            LINK,
        };

        const NODE_TYPE _type;          // 节点类型

        wstring		_name;              // 名称
        wstring     _displayName;       // 界面显示的名称
        long		_size;				// 大小

        // 3个数字，拆成2进制, 如 755 = 111 101 101   
        // 111 代表 可读 可写 可执行
        // 101 代表 可读 不可写 可执行
        // 101 代表 可读 不可写 可执行
        string		_mode;              // 权限
        string		_owner;             // 拥有者
        string		_group;             // 组

        time_t		_modifyTime;        // 修改时间
        time_t		_createTime;		// 创建时间

    protected:
        // 所在目录(父节点，对root此值为NULL)
        // 为了不让外面修改这个值，藏起来
        DirData*	_Parent;			
        wstring     _fileToPC;          // 下载到PC的路径，返回值

    public: // 方法

        NodeData(NODE_TYPE t);
        virtual ~NodeData(void);

        DirData* GetParent() const { return _Parent; }

        wstring GetFileToPC() { return _fileToPC; };

        // 节点是否是用户空间
        // return true:用户空间,  false:系统空间
        bool IsUserSpace();

        // 刷新节点信息
	    virtual bool Refresh() = 0;

        // 取所在目录
        wstring GetParentPath() const;

        // 取完整路径
		virtual std::wstring GetFullPath() const = 0;

		// 复制一个新类，并指定父节点（新对象指针由调用者负责）
		virtual NodeData* Clone(DirData* pParent) const = 0;

		// 重命名
		virtual bool Rename(wstring dest) = 0;

		// 改权限
		virtual bool ChangeMode(string mode) = 0;

        // 取权限
        virtual bool GetMode(string& mode, wstring& owner) = 0;

        // 是否目录（对Link则判断其指向的节点是否目录）
        virtual bool IsDir() = 0;

    protected:
		// 删除并析构自己（注意：调用后指针即失效！）
		virtual bool DoDelete(DirectoryOperationCommand* pCmd, ProgressObserver* pObserver=NULL) = 0;

        // 下载到指定目录
        virtual TransferResult DoDownload(wstring dest, DownloadFileCommand* pCmd) = 0;

    };

    // 权限字符串，777格式
    class CLASSINDLL_CLASS_DECL Mode
    {
    public:
        Mode(string rwxMode)
        {
            if ( rwxMode.size() != 9 )
            {
                _mode[0] = '7';
                _mode[1] = '7';
                _mode[2] = '7';
                return;
            }

            for(int i = 0; i < 3; ++i )
            {
                string rwx = rwxMode.substr(3*i, 3*i + 3);
                ConvertOne(rwx, i);
            }
        }

        string GetMode()
        {
            return string(_mode, 3);
        }

    private:
        void ConvertOne(string oneMode, int offset)
        {
            int mode = 0;
            for( int i = 0; i < 3; ++i )
            {
                if ( oneMode[i] != '-' )
                    mode += 1 << (2-i);
            }
            _mode[offset] = '0' + mode;
        }

        char _mode[3];	// 777格式

    };

}
