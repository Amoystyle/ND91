#pragma once
#include "UI/ProgressRecvMsg.h"
#include "common/FileManagerCommonFunc.h"


enum CommonProgressType
{
	BACKUP_OR_RESTORE,
	FILEMANAGER,
};

class BackUpAndRestoreObserver 
	: public ProgressObserver
	, public BaseHandler<ProgressShowExistReturnMsg>
	, public BaseHandler<ProgressBtnMsg>
{
public:
	BackUpAndRestoreObserver(){};

	// ���ս�����ʾ��������ʾ������ݣ�����������֧�֣��ɽ���ʵ��
	virtual void GetPrompt(MESSAGE_CODE msgCode)
	{
		if (_pegresstype == BACKUP_OR_RESTORE)
		{
			if ( msgCode == MESSAGE_DOWNLOAD )
				msgCode = MESSAGE_BACKUP;
		}
		else if (_pegresstype == FILEMANAGER)
		{
			_nLastMsgCode   = msgCode;
			_strMsg         = FileManagerCommonFunc::GetPrompt(msgCode);
		}
	}

	int GetPer( __int64 nPos, __int64 nRange )
	{
		if (nPos > 0 && nRange > 0)
		{
			while (nPos > 10240 && nRange > 10240)
			{
				nPos = nPos >> 1;
				nRange = nRange >> 1;
			}
			return (int)(nPos * 100 / nRange);
		} 
		return 0;
	}

	wstring  SetSubProgressInfo( wstring strInfo )
	{
		if (strInfo.find(L"Contact") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"Contact", CommonUIFunc::LoadString(_T("ContactPerson_text")));
		if (strInfo.find(L"Picture") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"Picture", CommonUIFunc::LoadString(_T("Image_text")));
		if (strInfo.find(L"Music") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"Music", CommonUIFunc::LoadString(_T("Music_text")));
		if (strInfo.find(L"SMS") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"SMS", CommonUIFunc::LoadString(_T("Message_text")));
		if (strInfo.find(L"Ring") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"Ring", CommonUIFunc::LoadString(_T("DiyBell_text")));
		if (strInfo.find(L"Callhistory") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"Callhistory", CommonUIFunc::LoadString(_T("CallRecord_text")));
		if (strInfo.find(L"Theme") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"Theme", CommonUIFunc::LoadString(_T("Theme_text")));
		if (strInfo.find(L"Application") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"Application", CommonUIFunc::LoadString(_T("App_text")));
		if (strInfo.find(L"EBook") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"EBook", CommonUIFunc::LoadString(_T("E_Book")));
		if (strInfo.find(L"Note") != wstring::npos)
			strInfo = CStrOperation::ReplaceW(strInfo, L"Note", CommonUIFunc::LoadString(_T("Memorandum_text")));

		return strInfo;
	}

	void GetBackupOrRestoreProgress(ProgressData pData) 
	{
		if (pData._allTaskSum == 0)
			pData._allTaskSum = pData._currentTaskSum;

		wchar_t msg[512] ={0};
		ProgressMsg progressmsg;
		progressmsg.nRequestID = _RequestID;
		progressmsg.wndHide = false;
		wsprintf(msg,_T("%s (%I64d/%I64d)"),CommonUIFunc::LoadString(_T("Message_Backup_text")),min(pData._taskNumberCompleted+1, pData._taskNumberSum),pData._taskNumberSum);

		progressmsg.mainText = msg;
		progressmsg.mainPos = GetPer(pData._allTaskProgresss, pData._allTaskSum);
		
		//��׿Ϊ\  iosΪ/
		if (pData._currentTaskName.find(_T("/")) != -1)
		{	//������
			int flag = pData._currentTaskName.find_last_of(_T("/"));
			wsprintf(msg,_T("%s (%s)"),CommonUIFunc::LoadString(_T("E_Book_text")),
				pData._currentTaskName.substr(flag+1,pData._currentTaskName.length()).c_str());

		}
		else if (pData._currentTaskName.find(_T("\\")) != -1)
		{	//������
			int flag = pData._currentTaskName.find_last_of(_T("\\"));
			wsprintf(msg,_T("%s (%s)"),CommonUIFunc::LoadString(_T("E_Book_text")),
				pData._currentTaskName.substr(flag+1,pData._currentTaskName.length()).c_str());

		}
		else if (pData._currentTaskName == _T("Contact") && platform == Ios )
		{	//IOS��ϵ�˻�ԭ��ʾ�����⴦��
			if (pData._currentTaskProgress == 100)		//�������
				wsprintf(msg,_T("%s"),CommonUIFunc::LoadString(_T("Synchronization")));
			else
				wsprintf(msg,_T("%s (%I64d%%)"),pData._currentTaskName.c_str(),pData._currentTaskProgress );
		}
		else if (pData._currentTaskName == _T("Picture") && platform == Ios )
		{//IOSͼƬ��ʾ����ʹ�ðٷֱ�
			wsprintf(msg,_T("%s (%I64d%%)"),CommonUIFunc::LoadString(_T("Image_text")),pData._currentTaskProgress );
		}
		else if (pData._currentTaskName == _T("Contact") && platform == Android )
		{	//��׿��ϵ�˻�ԭ��ʾ�����⴦��
			if (pData._currentTaskProgress == pData._currentTaskSum
				&& pData._currentTaskSum != 0)		//�������
				wsprintf(msg,_T("%s"),CommonUIFunc::LoadString(_T("Synchronization")));
			else
				wsprintf(msg,_T("%s (%I64d/%I64d)"),pData._currentTaskName.c_str(),pData._currentTaskProgress ,pData._currentTaskSum );
		}
		else
		{
			wsprintf(msg,_T("%s (%I64d/%I64d)"),pData._currentTaskName.c_str(),pData._currentTaskProgress ,pData._currentTaskSum );
		}

		progressmsg.subText = SetSubProgressInfo(msg);

		progressmsg.subPos = GetPer(pData._currentTaskProgress, pData._currentTaskSum);

		progressmsg.SendToUI();
	}


	void GetFileManagerProgress(ProgressData pData) 
	{
		ProgressMsg progressmsg;
		progressmsg.nRequestID = _RequestID;
		progressmsg.wndHide = false;
		if ( 0 == pData._allTaskProgresss && 0 == pData._taskNumberSum )
		{
			
			progressmsg.mainText = _strMsg;
			progressmsg.subText = _T("");
		}
		else if (_nLastMsgCode == MESSAGE_UPLOAD || _nLastMsgCode == MESSAGE_DOWNLOAD)
		{ // �򻯴�С��ʽ
			if (pData._allTaskSum == 0)
				pData._allTaskSum = pData._currentTaskSum;

			wostringstream woss;
			woss << _strMsg ;
			if ( pData._allTaskSum )
			{
				woss << L" ( " 
					<< CStrOperation::FormatFileSize(pData._allTaskProgresss) 
					<< L" / " 
					<< CStrOperation::FormatFileSize(pData._allTaskSum) 
					<< L") ";
			}
			progressmsg.mainText = woss.str();
			

			if ( pData._allTaskSum )
				progressmsg.mainPos = GetPer(pData._allTaskProgresss, pData._allTaskSum);


			wostringstream woss2;
			woss2 << pData._currentTaskName;
			if ( pData._currentTaskSum )
			{
				woss2 << L" (" 
					<< CStrOperation::FormatFileSize(pData._currentTaskProgress) 
					<< L" / " 
					<< CStrOperation::FormatFileSize(pData._currentTaskSum) 
					<< L") ";
			}
			progressmsg.subText = woss2.str();
			progressmsg.subPos = GetPer(pData._currentTaskProgress, pData._currentTaskSum);
			
		}
		else
		{
			// ʵ�ʴ�С��ʽ
			if (pData._allTaskSum == 0)
				pData._allTaskSum = pData._currentTaskSum;

			wostringstream woss;
			woss << _strMsg 
				<< L"( "
				<< min(pData._taskNumberCompleted+1, pData._taskNumberSum)
				<< L" / "
				<< pData._taskNumberSum
				<< L")";
			progressmsg.mainText = woss.str();
			progressmsg.mainPos = GetPer(pData._allTaskProgresss, pData._allTaskSum);
			

			wostringstream woss2;
			woss2 << pData._currentTaskName ;
			progressmsg.subText = woss2.str();
			progressmsg.subPos = GetPer(pData._currentTaskProgress, pData._currentTaskSum);    
		}

		progressmsg.SendToUI();
	}

	// ���ս�������
	virtual void GetProgress(ProgressData pData) 
	{
		if (_pegresstype == BACKUP_OR_RESTORE)
		{
			GetBackupOrRestoreProgress(pData);
		}
		else if (_pegresstype == FILEMANAGER)
		{
			GetFileManagerProgress(pData);
		}
	}

	// ���ս��������Ϣ
	virtual void Complete() {} 
	// Summary  : ѯ���û��ļ�/Ŀ¼�Ѵ���ʱ��δ���
	// Returns  : ND91Assistant::EXIST_ACTION �û���ѡ��
	// Parameter: std::wstring name �ļ�/Ŀ¼��
	virtual EXIST_ACTION GetExistAction(std::wstring name) 
	{ 
		if (_pegresstype == FILEMANAGER)
		{
			_event = CreateEvent(NULL, false, false, NULL);	
			ProgressShowExistMsg msg;
			msg.name = name;
			msg.nRequestID = _RequestID;
			msg.SendToUI();
			WaitForSingleObject(_event, INFINITE);
			CloseHandle(_event);
			return _retMsg;
		}
		else
			return OVERWRITE_ALL; 
		
	}

	virtual bool Handle(const ProgressShowExistReturnMsg* pMsg)
	{
		if (_RequestID == pMsg->nRequestID)
		{
			_retMsg = (EXIST_ACTION)pMsg->return_value;
			SetEvent(_event);
		}
		return true;
	}

	virtual bool Handle(const ProgressBtnMsg* pMsg)
	{
		if (_RequestID == pMsg->nRequestID)
		{
			if (pMsg->cancelBtn)
				this->Cancel();
			else if (pMsg->continueBtn)
				this->Skip();
		}
		return true;
	}

	HANDLE      _event; 
	EXIST_ACTION _retMsg;
	MESSAGE_CODE _nLastMsgCode;     // �������Ϣ
	wstring      _strMsg;           // ��ʾ��Ϣ
	int platform;
	int _RequestID;
	CommonProgressType _pegresstype;
};