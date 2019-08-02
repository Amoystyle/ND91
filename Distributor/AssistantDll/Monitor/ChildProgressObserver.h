#pragma once
#include "Monitor/ProgressObserver.h"
namespace ND91Assistant
{
    // �����ӽ������Ĵ�����������������������
    class ChildProgressObserver : public ProgressObserver
    {
    public:
        // pParentSubject : ��������
        // nMax ���ν�����Ҫ���ӵ����ֵ
        // defaultAction �ļ�����ʱObserver������ʱĬ�ϵĴ���
        ChildProgressObserver( void* pParentSubject = NULL, __int64 nMax = 0, EXIST_ACTION defaultAction = OVERWRITE); 
        virtual ~ChildProgressObserver() {};

        virtual void GetPrompt(MESSAGE_CODE msgCode);
        virtual void GetProgress(ProgressData data);
        virtual void Complete() {};

        virtual EXIST_ACTION GetExistAction(std::wstring name);

        virtual bool IsCancel();
    private:
        __int64 _nPos;
        __int64 _nLastPos;
        __int64 _nMax;
        void* _pSubject;
        EXIST_ACTION _defaultAction;
    };
}