#pragma once

#include "AssistantDll.h"
#include "ISubject.h"
#include "ProgressObserver.h"


#include <string>

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL ProgressSubject : public ISubject
    {
        friend class AdbUploadFileCommand;
        friend class AdbDownloadFileCommand;
        friend class IosWiFiFileSystem;
        friend class AFCFileSysemBase;
        friend class IosMessageDB;
        friend class IosCallDataDB;
        friend class IosNotesDB;
        //friend class IosContactDB;
        friend class IosIPAOperationWithItunes;
        friend class IosPXLAppOperation;
        friend class IosIPAOperationWithDaemon;
        friend class IosDebAppOperation;
        friend class IIosAppOperation;
        friend class IPAPackageHelper;
    public:
        virtual ~ProgressSubject() {}

        // 发送进度提示：具体提示语句内容（包括多语种支持）由界面实现
        virtual void SendPrompt(MESSAGE_CODE msgCode);

        // 发送进度数据，并保存
        virtual void SendProgress();

        // 完成
        virtual void Complete();

 		// 取得上次发送的进度数据
 		const ProgressData* GetLastProgress();

        // 清除ProgressData的数据
        void ResetProgressData();

#pragma region 设置任务进度: 由具体执行的Command使用
        
        // 同时增加：当前任务进度、总体进度
        virtual void AddTaskProgress(__int64 value);
        
        // 设置当前任务总量：同时将当前任务进度清零
        void SetCurrentTaskSum(__int64 value);

        // 设置当前任务名称
        void SetCurrentTaskName(std::wstring name);

#pragma endregion

#pragma region 设置总体进度：由循环调用Command的方法使用

        // 增加所有任务进度：仅在当前任务被取消时使用
        void AddAllTaskProgress(__int64 value);

        // 设置所有任务总量：同时将所有任务进度清零
        void SetAllTaskSum(__int64 value);

        // 增加已完成的任务数，默认已完成任务数加1
        void IncreaseTaskNumberComplete(__int64 valua = 1);

        // 设置任务总数：同时将已完成任务数清零
        void SetTaskNumberSum(__int64 value);

        // 设置当前任务进度完成
        void SetCurrentTaskComplete();

#pragma endregion

    protected:

		ProgressData _data;
    };

    // 空Subject实例（NullObject、Singleton）
	class NullProgressSubject : public ProgressSubject
	{
	public:
        NullProgressSubject() {}
        virtual ~NullProgressSubject() {}

		virtual void SendPrompt(MESSAGE_CODE msgCode) {}
		virtual void SendProgress(const ProgressData* pData) {}
		virtual void Complete() {}

		static NullProgressSubject& Instance() { return _instance; }

	private:

		static NullProgressSubject _instance;
	};

    // 子进度条，子进度条的全局进度将转化为原进度条的子进度
    class ChildProgressSubject : public ProgressSubject
    {
    public:
        // pParentSubject : 父进度条
        ChildProgressSubject( ProgressSubject* pParentSubject, int64_t nMax = 0) 
        { 
            _pSubject = pParentSubject; 
            if (pParentSubject && nMax == 0)
                _nMax = pParentSubject->GetLastProgress()->_currentTaskSum; 
            else
                _nMax = nMax;
            _nPos = 0; 
            _nLastPos = 0; 
        };
        virtual ~ChildProgressSubject() {}

        virtual void SendPrompt(MESSAGE_CODE msgCode) 
        { 
            if (_pSubject) 
                _pSubject->SendPrompt(msgCode); 
        }
        virtual void SendProgress() 
        {
            // 转换进度条
            if (_pSubject && _data._allTaskSum > 0)
            {
                _nPos = _data._currentTaskProgress * _nMax / _data._allTaskSum;
                if (_nPos > _nLastPos)
                {
                    _pSubject->AddTaskProgress(_nPos - _nLastPos);
                    _pSubject->SendProgress();
                    _nLastPos = _nPos;
                }
            }
        }
        virtual void Complete() {  }

    private:
        __int64 _nPos;
        __int64 _nLastPos;
        __int64 _nMax;
        ProgressSubject* _pSubject;
    };


	class BackupRestoreProgressSubject : public ProgressSubject
	{
	public:
		void AddTaskProgress( __int64 value )
		{
			if (_data._currentTaskSum == 0)
				return;

			_data._currentTaskProgress += value;   
			_data._allTaskProgresss = _data._taskNumberCompleted * 100
				+ 100 * _data._currentTaskProgress /  _data._currentTaskSum;

			if (_data._currentTaskProgress > _data._currentTaskSum)
				_data._currentTaskProgress = _data._currentTaskSum;

			if (_data._allTaskProgresss > _data._allTaskSum)
				_data._allTaskProgresss = _data._allTaskSum;
		}
	};
}
