#include "stdafx.h"
#include "Monitor/ProgressData.h"

ND91Assistant::ProgressData::ProgressData()
{
    _currentTaskProgress = 0;
    _currentTaskSum = 0;
    _currentTaskName.clear();

    _allTaskProgresss = 0;
    _allTaskSum = 0;

    _taskNumberCompleted = 0;
    _taskNumberSum = 0;
}

std::wstring ND91Assistant::ProgressData::ToString() const
{
    std::wostringstream woss;   
    woss << CCodeOperation::StringToWstring(" 当前任务进度：") << _currentTaskProgress << CCodeOperation::StringToWstring("/") << _currentTaskSum;
    woss << CCodeOperation::StringToWstring(" 总进度：") << _allTaskProgresss << CCodeOperation::StringToWstring("/") << _allTaskSum;
    woss << CCodeOperation::StringToWstring(" 完成任务数：") << _taskNumberCompleted  << CCodeOperation::StringToWstring("/") << _taskNumberSum;
    woss << CCodeOperation::StringToWstring(" 当前任务名：") << _currentTaskName;
    return woss.str();
}
