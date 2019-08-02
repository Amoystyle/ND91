#include "stdafx.h"
#include "ChildProgressObserver.h"
#include "Monitor/ProgressSubject.h"

using namespace ND91Assistant;

ND91Assistant::ChildProgressObserver::ChildProgressObserver( void* pParentSubject, __int64 nMax, EXIST_ACTION defaultAction )
{
    _pSubject = pParentSubject; 
    _defaultAction = defaultAction;
    ProgressSubject* pSubject = (ProgressSubject*)pParentSubject;
    if (pSubject && nMax == 0)
        _nMax = pSubject->GetLastProgress()->_currentTaskSum; 
    else
        _nMax = nMax;
    _nPos = 0; 
    _nLastPos = 0;
}

void ND91Assistant::ChildProgressObserver::GetPrompt( MESSAGE_CODE msgCode )
{
    if (!_pSubject) return;
    ProgressSubject* pSubject = (ProgressSubject*)_pSubject;
    if (pSubject) 
        pSubject->SendPrompt(msgCode);
}

void ND91Assistant::ChildProgressObserver::GetProgress( ProgressData data )
{
    if (!_pSubject) return;
    // 转换进度条
    ProgressSubject* pSubject = (ProgressSubject*)_pSubject;
    if (pSubject && data._allTaskSum > 0)
    {
        _nPos = data._allTaskProgresss * _nMax / data._allTaskSum;
        if (_nPos > _nLastPos)
        {
            pSubject->AddTaskProgress(_nPos - _nLastPos);            
            pSubject->SendProgress();
            _nLastPos = _nPos;
        }
    }
}

ND91Assistant::EXIST_ACTION ND91Assistant::ChildProgressObserver::GetExistAction( std::wstring name )
{
    if (!_pSubject) return _defaultAction;
    ProgressSubject* pSubject = (ProgressSubject*)_pSubject;
    if (pSubject)
    {
        ProgressObserver* pObserver = dynamic_cast<ProgressObserver*>(pSubject->GetFirstObserver());
        if (pObserver)
        {
            return pObserver->GetExistAction(name);
        }
    }
    return _defaultAction;
}

bool ND91Assistant::ChildProgressObserver::IsCancel()
{
    ProgressSubject* pSubject = (ProgressSubject*)_pSubject;
    if (pSubject)
    {
        ProgressObserver* pObserver = dynamic_cast<ProgressObserver*>(pSubject->GetFirstObserver());
        if (pObserver)
        {
            return pObserver->IsCancel();
        }
    }
    return false;
 
}
