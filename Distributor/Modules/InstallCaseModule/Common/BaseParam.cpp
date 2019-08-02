#include "stdafx.h"
#include "BaseParam.h"

INSTALLCASE_BEGIN
BaseParam::BaseParam()
{INTERFUN;
    _pParent = NULL;
    _pProgress = NULL;
    _nAction = 0;    

    _strParam = L"";
    _voidParam = NULL;
    _intParam = 0;
    _boolParam = true;

	_pObserver = NULL;

}

BaseParam::BaseParam( void* pParent, int nAction, void* pProgress, BaseParam* pInitParam )
{INTERFUN;
    _pParent    = pParent;
    _pProgress  = pProgress;
    _nAction    = nAction;
    
    if (pInitParam)
    {
        _voidList.insert(_voidList.end(), pInitParam->_voidList.begin(), pInitParam->_voidList.end());
        _strList.insert(_strList.end(), pInitParam->_strList.begin(), pInitParam->_strList.end());
        _intList.insert(_intList.end(), pInitParam->_intList.begin(), pInitParam->_intList.end());
        _strParam = pInitParam->_strParam;
        _voidParam = pInitParam->_voidParam;
        _intParam = pInitParam->_intParam;
        _boolParam = pInitParam->_boolParam;
    }
	_pObserver = NULL;
}

BaseParam::~BaseParam()
{INTERFUN;
}

void BaseParam::AddVoidParam( void* par )
{INTERFUN;
    _voidList.push_back(par);
}

void BaseParam::AddStrParam( wstring par )
{INTERFUN;
    _strList.push_back(par);
}

void BaseParam::AddIntParam( int par )
{INTERFUN;
    _intList.push_back(par);
}

void BaseParam::SetParent( void* pParent )
{INTERFUN;
    _pParent = pParent;
}

void BaseParam::SetProgress( void* pProgress )
{INTERFUN;
    _pProgress = pProgress;
}

void BaseParam::SetAction( int nAction )
{INTERFUN;
    _nAction = nAction;
}

const vector<void*>* BaseParam::GetVoidParam() const
{
    return &_voidList;
}

const vector<wstring>* BaseParam::GetStrParam() const
{
    return &_strList;
}

const vector<int>* BaseParam::GetIntParam() const
{
    return &_intList;
}

void* BaseParam::GetParent() const
{
    return _pParent;
}

void* BaseParam::GetProgress() const
{
    return _pProgress;
}

int BaseParam::GetAction() const
{
    return _nAction;
}
INSTALLCASE_END