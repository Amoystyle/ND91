#include "stdafx.h"
#include "Monitor/ProgressSubject.h"

NullProgressSubject NullProgressSubject::_instance;

/**
 * 设置当前进度
 */
void ProgressSubject::SendProgress()
{
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        ProgressObserver* pObserver = dynamic_cast<ProgressObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->GetProgress(_data);
    }
}

void ProgressSubject::SendPrompt(MESSAGE_CODE msgCode)
{
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        ProgressObserver* pObserver = dynamic_cast<ProgressObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->GetPrompt(msgCode);
    }
}

void ProgressSubject::Complete()
{
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        ProgressObserver* pObserver = dynamic_cast<ProgressObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->Complete();
    }
}

const ProgressData* ND91Assistant::ProgressSubject::GetLastProgress()
{
	return &_data;
}

void ND91Assistant::ProgressSubject::ResetProgressData()
{
    _data._allTaskProgresss = 0;
    _data._allTaskSum = 0;
    _data._currentTaskProgress = 0;
    _data._currentTaskSum = 0;
    _data._taskNumberCompleted = 0;
    _data._taskNumberSum = 0;
}

void ND91Assistant::ProgressSubject::AddTaskProgress( __int64 value )
{
    _data._currentTaskProgress += value;    
    _data._allTaskProgresss += value;
    if (_data._currentTaskProgress > _data._currentTaskSum)
        _data._currentTaskProgress = _data._currentTaskSum;
    if (_data._allTaskProgresss > _data._allTaskSum)
        _data._allTaskProgresss = _data._allTaskSum;
}

void ND91Assistant::ProgressSubject::SetCurrentTaskSum( __int64 value )
{
    _data._currentTaskSum = value;
    _data._currentTaskProgress = 0;
}

void ND91Assistant::ProgressSubject::SetCurrentTaskName( std::wstring name )
{
    _data._currentTaskName = name;
}

void ND91Assistant::ProgressSubject::AddAllTaskProgress( __int64 value )
{
    _data._allTaskProgresss += value;
}

void ND91Assistant::ProgressSubject::SetAllTaskSum( __int64 value )
{
    _data._allTaskSum = value;
    _data._allTaskProgresss = 0;
}

void ND91Assistant::ProgressSubject::IncreaseTaskNumberComplete(__int64 value)
{
    SetCurrentTaskSum(0);
    _data._taskNumberCompleted += value;
}

void ND91Assistant::ProgressSubject::SetTaskNumberSum( __int64 value )
{
    _data._taskNumberSum = value;
    _data._taskNumberCompleted = 0;
}

void ND91Assistant::ProgressSubject::SetCurrentTaskComplete()
{
    AddTaskProgress(_data._currentTaskSum - _data._currentTaskProgress);
}

