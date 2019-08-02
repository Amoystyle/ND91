#pragma once
#include "Monitor/ProgressObserver.h"
namespace ND91Assistant
{
    // 利用子进度条的处理来解决任务包含任务的情况
    class ChildProgressObserver : public ProgressObserver
    {
    public:
        // pParentSubject : 父进度条
        // nMax 本次进度条要增加的最大值
        // defaultAction 文件存在时Observer不存在时默认的处理
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