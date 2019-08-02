#pragma once
#include "Monitor/ProgressObserver.h"

namespace ND91Assistant
{
	class DefaultProgressObserver : public ProgressObserver
	{
    public:
        DefaultProgressObserver(EXIST_ACTION nAction) {_nAction = nAction; };
		// 接收进度提示：具体提示语句内容（包括多语种支持）由界面实现
		virtual void GetPrompt(MESSAGE_CODE msgCode) {};

		// 接收进度数据
		virtual void GetProgress(ProgressData pData) {};

		// 接收进度完成消息
		virtual void Complete() {};

		// Summary  : 询问用户文件/目录已存在时如何处理
		// Returns  : ND91Assistant::EXIST_ACTION 用户的选择
		// Parameter: std::wstring name 文件/目录名		
		virtual EXIST_ACTION GetExistAction(std::wstring name) { return _nAction; };

        EXIST_ACTION _nAction;
	};

    class OverwriteObserver : public ProgressObserver
    {
        // 接收进度提示：具体提示语句内容（包括多语种支持）由界面实现
        virtual void GetPrompt(MESSAGE_CODE msgCode) {};

        // 接收进度数据
        virtual void GetProgress(ProgressData pData) {};

        // 接收进度完成消息
        virtual void Complete() {};

        // Summary  : 询问用户文件/目录已存在时如何处理
        // Returns  : ND91Assistant::EXIST_ACTION 用户的选择
        // Parameter: std::wstring name 文件/目录名
        virtual EXIST_ACTION GetExistAction(std::wstring name) { return OVERWRITE_ALL; };
    };
}
