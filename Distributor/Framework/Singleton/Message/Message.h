/********************************************************************
	created:	2012/12/06 11:54            author:		邱畅
	
	purpose:	消息基类、消息处理基类
*********************************************************************/
#pragma once
#include <windows.h>
#include "MessageCenter.h"
#include <assert.h>
/************************************************************************/
/* 消息处理抽象类
/************************************************************************/
class IHandler
{
public:
    virtual string GetType() = 0;
};

/************************************************************************/
/* 消息抽象类
/************************************************************************/
class IMessage
{
public:
    IMessage() { _bRecordMessage = false; };
    IMessage(const IMessage& rhs) { _bRecordMessage = rhs._bRecordMessage; };
    IMessage& operator = (const IMessage& rhs) { if ( this == &rhs ) return *this; _bRecordMessage = rhs._bRecordMessage; return *this; }

    virtual ~IMessage() {}
    // Return bool 是否继续传递
    virtual bool SendTo(IHandler* pHandler) const = 0;
    virtual void PostToUI(HWND hWnd) const = 0;
    // Return bool 是否继续传递
    virtual bool SendAndGetReturn(IHandler* pHandler) = 0;
    virtual void Begin(IHandler* pHandler) const = 0;
    virtual void End(IHandler* pHandler) const = 0;

    // 消息Log开关
    bool RecordMessage() const { return _bRecordMessage; }
    void RecordMessage(bool val) { _bRecordMessage = val; }

    // 用于Log输出
    virtual wstring ToString() const { return L"Please override ToString() to record message data"; }

private:
    bool _bRecordMessage;    // 是否打开本消息Log
};

/************************************************************************/
/* 消息基类（模板参数：子类）
/************************************************************************/
template<class M>
class BaseMessage : public IMessage
{
    friend class MessageCenter;
public:
    virtual ~BaseMessage() {}

    // 快捷的调用方式：同步
    virtual bool Send() const
    {
        return Singleton::GetMessageCenter()->Send(this);
    }

    // 快捷的调用方式：异步
    virtual void PostToUI() const 
    {
        PostToUI(Singleton::GetMainWnd());        
    }

    // 快捷的调用方式：同步
    virtual void SendToUI() const 
    {
        SendToUI(Singleton::GetMainWnd());        
    }

    // 快捷的调用方式：同步并等待返回值
    virtual void SendAndGetReturn()
    {
        Singleton::GetMessageCenter()->GetData(this);
    }

private:
    virtual bool SendTo(IHandler* pHandler) const
    {
        BaseHandler<M>* p = dynamic_cast<BaseHandler<M>*>(pHandler);
        assert ( p );
        if ( ! p ) return false;

        Singleton::GetMessageCenter()->RecordHandle(p, this);
        return p->Handle((const M*) this);
    }


    virtual void PostToUI(HWND hWnd) const 
    {
        LPARAM p = (LPARAM)new M(*((M*)this));
        assert ( p );
        if ( !p ) return;

        ((M*)p)->RecordMessage(this->RecordMessage());
        PostMessage(hWnd, WM_SEND_TO_MAIN, NULL, p);
    }

    virtual void SendToUI(HWND hWnd) const 
    {
        LPARAM p = (LPARAM)new M(*((M*)this));
        assert ( p );
        if ( !p ) return;
        ((M*)p)->RecordMessage(this->RecordMessage());
        SendMessage(hWnd, WM_SEND_TO_MAIN, NULL, p);
    }

    // 发送消息
    virtual bool SendAndGetReturn(IHandler* pHandler)
    {
        BaseGetData<M>* p = dynamic_cast<BaseGetData<M>*>(pHandler);
        assert ( p );
        if ( ! p ) return false;

        Singleton::GetMessageCenter()->RecordHandle(p, this);
        return p->Handle((M*) this);
    }

    // 组消息的开始通知
    virtual void Begin(IHandler* pHandler) const
    {
        BaseHandler<M>* p = dynamic_cast<BaseHandler<M>*>(pHandler);
        assert ( p );
        if ( ! p ) return;
        return p->Begin((const M*)this);
    }

    // 组消息的结束通知
    virtual void End(IHandler* pHandler) const
    {
        BaseHandler<M>* p = dynamic_cast<BaseHandler<M>*>(pHandler);
        assert ( p );
        if ( ! p ) return;
        return p->End((const M*)this);
    }

public:
    int nRequestID;    //通用请求编号
    int nUserData;     //通用用户数据
    int nParam;        //通用int参数
    wstring strParam;  //通用wsring参数
};

/************************************************************************/
/* 消息处理基类（模板参数：要处理的消息）
/************************************************************************/
template<class M>
class BaseHandler : public IHandler
{
public:
    BaseHandler() { Singleton::GetMessageCenter()->Attach(this); }
    virtual ~BaseHandler() { Singleton::GetMessageCenter()->Detach(this); }
    virtual string GetType() { return typeid(M).raw_name(); }

    // 子类需要实现：消息的处理方法
    virtual bool Handle(const M* pMsg) = 0;

    // 子类按需实现：组消息的起始通知
    virtual void Begin(const M* pMsg) {}

    // 子类按需实现：组消息的结束通知
    virtual void End(const M* pMsg) {}
};

/************************************************************************/
/* 即时返回结果的消息处理基类（模板参数：要处理的消息）
/************************************************************************/
template<class M>
class BaseGetData : public BaseHandler<M>
{
public:
    BaseGetData() : BaseHandler<M>() {}

    // 需要实现非 const 参数的方法
    virtual bool Handle(M* pMsg) = 0;

private:
    // 不实现const参数的方法
    virtual bool Handle(const M* pMsg) 
    {
		assert(false); 
        return true; 
    }
};
