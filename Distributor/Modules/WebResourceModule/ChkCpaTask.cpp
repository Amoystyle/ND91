#include "StdAfx.h"
#include "ChkCpaTask.h"
#include "Common/WebResourceCommonFunc.h"
#include "CpaDataHelper.h"

CChkCpaTask::CChkCpaTask(void)
{
	_itemHashValues = new set<wstring>();
	_status = TaskStatusWaiting;
	setCancel(false);
}

CChkCpaTask::~CChkCpaTask(void)
{
	if (NULL != _itemHashValues)
	{
		delete _itemHashValues;
		_itemHashValues = NULL;
	}
}

bool CChkCpaTask::Run()
{
	if (this->_strHashValue == L"")
	{
		return false;
	}
	_itemHashValues->clear();
	if (getCancel())
	{
		return false;
	}

	initItemHashValues();

	if (getCancel())
	{
		return false;
	}
	

	Compare();

	if (getCancel())
	{
		return false;
	}
	return true;
}


void CChkCpaTask::setCancel(bool var)
{
	this->_cancel = var;
}

void CChkCpaTask::setStatus(TaskStatus var)
{
	CpaTaskStatueMsg msg;
	msg.oldStatus = this->_status;
	msg.newStatus = var;
	msg.task = this;

	this->_status = var;

	msg.Send();
}

TaskStatus CChkCpaTask::getStatus()
{
	return this->_status;
}

set<wstring>* CChkCpaTask::getItemHashValues()
{
	return _itemHashValues;
}


void CChkCpaTask::setStrHashValue(wstring var)
{
	this->_strHashValue = var;
}

void CChkCpaTask::initItemHashValues()
{
	string strHashValueTmp = CCodeOperation::ws2s(_strHashValue);

	vector<string> vecHashValue;

	WebResourceCommonFunc::Split(strHashValueTmp, ',', vecHashValue);

	for (vector<string>::iterator iter = vecHashValue.begin(); iter != vecHashValue.end(); ++iter)
	{
		string tmp = *iter;
		this->_itemHashValues->insert(CCodeOperation::s2ws(tmp));
	}
}

bool CChkCpaTask::getCancel()
{
	return _cancel;
}


ENUM_PLATFORM CChkCpaTask::getPlatfrom()
{
	return _platform;
}

void CChkCpaTask::setPlatfrom(ENUM_PLATFORM var)
{
	_platform = var;
}

void CChkCpaTask::Compare()
{
	bool isExistChk = false;
	if (getPlatfrom() == PLATFORM_Android)
	{
		isExistChk = CCpaDataHelper::GetInstance()->getAndroidInitFinished();
	}
	else
	{
		isExistChk = CCpaDataHelper::GetInstance()->getIOSInitFinished();
	}
	for (set<wstring>::iterator iter = _itemHashValues->begin(); iter != _itemHashValues->end(); ++iter)
	{
		wstring hashValueTmp = *iter;
		CpaWebIsCancelMsg cpaIsCancelMsg;
		if (getPlatfrom() == PLATFORM_Android)
		{
			cpaIsCancelMsg.bIsAndroidPlatform = true;
		}
		else
		{
			cpaIsCancelMsg.bIsAndroidPlatform = false;
		}
		cpaIsCancelMsg.strParam = _id;
		cpaIsCancelMsg.SendAndGetReturn();
		setCancel(cpaIsCancelMsg.bIsCancel);//检测任务可否继续
		if (getCancel())
		{
			return;
		}

		CpaWebButtonStatusChangedMsg msg;
		msg.bIsExist = false;
		msg.strParam = _id;
		msg.strHashValue = hashValueTmp;
		msg.platform = getPlatfrom();
		if (isExistChk)
		{
			wstring path = L"";
			if (getPlatfrom() == PLATFORM_Android)
			{
				path = CCpaDataHelper::GetInstance()->GetAndroidItemPath(CCodeOperation::ws2s(hashValueTmp));
			}
			else
			{
				path = CCpaDataHelper::GetInstance()->GetIOSItemPath(CCodeOperation::ws2s(hashValueTmp));
			}
			if(L"" != path)
			{

				msg.bIsExist = true;
				msg.strHashValue = hashValueTmp;
			}
		}
		
		msg.PostToUI();
	}
}

void CChkCpaTask::setId(wstring var)
{
	_id = var;
}

void CChkCpaTask::setUserData(UINT var)
{
	_userData = var;
}

UINT CChkCpaTask::getUserData()
{
	return _userData;
}