#pragma once

#include "Monitor/ProgressObserver.h"
using namespace std;
namespace ND91Assistant
{
	// App������ʾת����
	class AppProgressObserver : public ProgressObserver
	{
	public:
		AppProgressObserver(ProgressSubject* pSubject)
		{
			_nLastAllTaskProgress = 0;
			_pSubject = pSubject;
		}
		virtual ~AppProgressObserver() {}

		// ���ս�����ʾ��������ʾ������ݣ�����������֧�֣��ɽ���ʵ��
		virtual void GetPrompt(MESSAGE_CODE msgCode) {};

		// ���ս�������
		virtual void GetProgress(ProgressData data)		
		{
			if (data._allTaskSum > 0)
			{
				_pSubject->AddTaskProgress( (data._allTaskProgresss - _nLastAllTaskProgress) * 100 / data._allTaskSum );
				_nLastAllTaskProgress = data._allTaskProgresss;
				_pSubject->SendProgress();
			}
		};

		// ���ս��������Ϣ
		virtual void Complete()
		{
			_pSubject->IncreaseTaskNumberComplete();
			_pSubject->SendProgress();
		};

		virtual EXIST_ACTION GetExistAction(std::wstring name)
		{
			return NONE;
		};
	private:
		__int64	_nLastAllTaskProgress;
		ProgressSubject* _pSubject;
	};
}