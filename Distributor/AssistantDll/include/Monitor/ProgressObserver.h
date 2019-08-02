#pragma once

#include "AssistantDll.h"
#include "IObserver.h"
#include "ProgressData.h"
#include "ICancel.h"
#include <string>
namespace ND91Assistant
{
    enum MESSAGE_CODE {
        MESSAGE_PREPARE,        // 准备阶段：搜集数据
		MESSAGE_ERROR,
		MESSAGE_NEED_SU_AUTH_IN_DEVICE,//需要客户到设备上确定助手客户端权限提升
		//程序安装相关						
		MESSAGE_INSTALL_PREPARE,			// 准备数据	
		MESSAGE_INSTALL_UPLOAD,				// 上传APP文件
		MESSAGE_INSTALL_INSTALL,			// 安装程序
        MESSAGE_INSTALL_AUTHORIZOR,			// 授权
        MESSAGE_INSTALL_UNINSTALL,          // 卸载程序
		MESSAGE_INSTALL_COPYDATA,			// 拷贝程序数据
		MESSAGE_INSTALL_NOTIFYSB,			// 修正IOS软件图标
        MESSAGE_INSTALL_PXLTOIPA,           // 转换
		//备份程序相关
		MESSAGE_ARCHIVE_DOWNLOAD,			//下载APP数据
		MESSAGE_ARCHIVE_ARRANGE,			//整理数据
		MESSAGE_ARCHIVE_PACKAGE,			//打包APP数据		
		MESSAGE_ARCHIVE_REMOVE,				//删除手机上的备份程序
		MESSAGE_ARCHIVE_APP_ONPHONE,	//将程序备份到手机的某个目录下	
        //文件相关
        MESSAGE_UPLOAD,                     // 上传文件
        MESSAGE_DOWNLOAD,                   // 下载文件
        MESSAGE_BACKUP,                     // 备份数据
        MESSAGE_RESTORE,                    // 恢复数据
        MESSAGE_EXPORT,                     // 导出数据
        MESSAGE_IMPORT,                     // 导入数据
        MESSAGE_PARSE,                      // 解析备份数据

		MESSAGE_PROCESS,					//回报进度
		MESSAGE_COMPLETE,					//结束

		//备份还原
		MESSAGE_NoSupportSynchronizer,		//不支持同步,联系人备份如果提示这个信息则无法备份联系人
		MESSAGE_EnableiCloud,				//云同步开启，联系人备份如果收到这信息则，使用云同步的联系人无法备份(要备份需取消云同步，或联系人不在云同步分类下)	
		MESSAGE_EnableSyncFail,				//同步功能被禁止，并且尝试打开失败，
		MESSAGE_DeamonDisConnect,			//与守护的连接断开，可能守护在重启.
    };

    // 已存在同名文件/文件夹时的操作（需要用户介入）
    enum EXIST_ACTION
    {
        OVERWRITE,              // 覆盖
        OVERWRITE_ALL,          // 覆盖所有
        RENAME,                 // 重命名
        RENAME_ALL,             // 重命名所有
        RENAME_COPY,            // 重命名(文件名前‘复件’)
        RENAME_COPY_ALL,        // 重命名所有(文件名前‘复件’)
        CANCEL,                 // 取消
        NONE,                   // 初值，无含义
    };

    // 进度提示接收类
    class CLASSINDLL_CLASS_DECL ProgressObserver : public IObserver, public ICancel
    {
    public:
        virtual ~ProgressObserver() {}

        // 接收进度提示：具体提示语句内容（包括多语种支持）由界面实现
        virtual void GetPrompt(MESSAGE_CODE msgCode) = 0;

        // 接收进度数据
        virtual void GetProgress(ProgressData pData) = 0;

        // 接收进度完成消息
        virtual void Complete() = 0;

        // Summary  : 询问用户文件/目录已存在时如何处理
        // Returns  : ND91Assistant::EXIST_ACTION 用户的选择
        // Parameter: std::wstring name 文件/目录名
        virtual EXIST_ACTION GetExistAction(std::wstring name) = 0;

        virtual bool IsCancel() { return ICancel::IsCancel(); }
    };

	class CLASSINDLL_CLASS_DECL NullProgressObserver : public ProgressObserver
	{
		virtual ~NullProgressObserver() {}

		// 接收进度提示：具体提示语句内容（包括多语种支持）由界面实现
		virtual void GetPrompt(MESSAGE_CODE msgCode) {}

		// 接收进度数据
		virtual void GetProgress(ProgressData pData) {}

		// 接收进度完成消息
		virtual void Complete() {}
	};

}
