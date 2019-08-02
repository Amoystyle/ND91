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
    woss << CCodeOperation::StringToWstring(" ��ǰ������ȣ�") << _currentTaskProgress << CCodeOperation::StringToWstring("/") << _currentTaskSum;
    woss << CCodeOperation::StringToWstring(" �ܽ��ȣ�") << _allTaskProgresss << CCodeOperation::StringToWstring("/") << _allTaskSum;
    woss << CCodeOperation::StringToWstring(" �����������") << _taskNumberCompleted  << CCodeOperation::StringToWstring("/") << _taskNumberSum;
    woss << CCodeOperation::StringToWstring(" ��ǰ��������") << _currentTaskName;
    return woss.str();
}
