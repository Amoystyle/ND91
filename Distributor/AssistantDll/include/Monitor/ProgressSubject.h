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

        // ���ͽ�����ʾ��������ʾ������ݣ�����������֧�֣��ɽ���ʵ��
        virtual void SendPrompt(MESSAGE_CODE msgCode);

        // ���ͽ������ݣ�������
        virtual void SendProgress();

        // ���
        virtual void Complete();

 		// ȡ���ϴη��͵Ľ�������
 		const ProgressData* GetLastProgress();

        // ���ProgressData������
        void ResetProgressData();

#pragma region �����������: �ɾ���ִ�е�Commandʹ��
        
        // ͬʱ���ӣ���ǰ������ȡ��������
        virtual void AddTaskProgress(__int64 value);
        
        // ���õ�ǰ����������ͬʱ����ǰ�����������
        void SetCurrentTaskSum(__int64 value);

        // ���õ�ǰ��������
        void SetCurrentTaskName(std::wstring name);

#pragma endregion

#pragma region ����������ȣ���ѭ������Command�ķ���ʹ��

        // ��������������ȣ����ڵ�ǰ����ȡ��ʱʹ��
        void AddAllTaskProgress(__int64 value);

        // ������������������ͬʱ�����������������
        void SetAllTaskSum(__int64 value);

        // ��������ɵ���������Ĭ���������������1
        void IncreaseTaskNumberComplete(__int64 valua = 1);

        // ��������������ͬʱ�����������������
        void SetTaskNumberSum(__int64 value);

        // ���õ�ǰ����������
        void SetCurrentTaskComplete();

#pragma endregion

    protected:

		ProgressData _data;
    };

    // ��Subjectʵ����NullObject��Singleton��
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

    // �ӽ��������ӽ�������ȫ�ֽ��Ƚ�ת��Ϊԭ���������ӽ���
    class ChildProgressSubject : public ProgressSubject
    {
    public:
        // pParentSubject : ��������
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
            // ת��������
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
