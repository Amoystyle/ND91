#pragma once

#include "AssistantDll.h"
#include <string>

#pragma warning (disable:4251)

namespace ND91Assistant
{
    // 进度数据
    struct CLASSINDLL_CLASS_DECL ProgressData
    {
        __int64 _currentTaskProgress;       // 当前任务的完成量
        __int64 _currentTaskSum;            // 当前任务的总量
        std::wstring _currentTaskName;  // 当前任务的相关值，比如文件名

		__int64 _allTaskProgresss;          // 当前任务的完成量
		__int64 _allTaskSum;		        // 所有任务的总量
        
        __int64 _taskNumberCompleted;       // 当前完成的任务数
        __int64 _taskNumberSum;             // 所有任务的总数

        // 构造函数：全部设零
		ProgressData();

        // 转为文本
        std::wstring ToString() const;
    };
}
