#pragma once

#include "Monitor/ProgressObserver.h"
using namespace std;
namespace ND91Assistant
{
	// App进度提示转换类
	class AppProgressObserver : public ProgressObserver
	{
	public:
		AppProgressObserver(ProgressSubject* pSubject)
		{
			_nLastAllTaskProgress = 0;
			_pSubject = pSubject;
		}
		virtual ~AppProgressObserver() {}

		// 接收进度提示：具体提示语句内容（包括多语种支持）由界面实现
		virtual void GetPrompt(MESSAGE_CODE msgCode) {};

		// 接收进度数据
		virtual void GetProgress(ProgressData data)		
		{
			if (data._allTaskSum > 0)
			{
				_pSubject->AddTaskProgress( (data._allTaskProgresss - _nLastAllTaskProgress) * 100 / data._allTaskSum );
				_nLastAllTaskProgress = data._allTaskProgresss;
				_pSubject->SendProgress();
			}
		};

		// 接收进度完成消息
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