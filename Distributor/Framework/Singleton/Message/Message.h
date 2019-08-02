/********************************************************************
	created:	2012/12/06 11:54            author:		��
	
	purpose:	��Ϣ���ࡢ��Ϣ�������
*********************************************************************/
#pragma once
#include <windows.h>
#include "MessageCenter.h"
#include <assert.h>
/************************************************************************/
/* ��Ϣ���������
/************************************************************************/
class IHandler
{
public:
    virtual string GetType() = 0;
};

/************************************************************************/
/* ��Ϣ������
/************************************************************************/
class IMessage
{
public:
    IMessage() { _bRecordMessage = false; };
    IMessage(const IMessage& rhs) { _bRecordMessage = rhs._bRecordMessage; };
    IMessage& operator = (const IMessage& rhs) { if ( this == &rhs ) return *this; _bRecordMessage = rhs._bRecordMessage; return *this; }

    virtual ~IMessage() {}
    // Return bool �Ƿ��������
    virtual bool SendTo(IHandler* pHandler) const = 0;
    virtual void PostToUI(HWND hWnd) const = 0;
    // Return bool �Ƿ��������
    virtual bool SendAndGetReturn(IHandler* pHandler) = 0;
    virtual void Begin(IHandler* pHandler) const = 0;
    virtual void End(IHandler* pHandler) const = 0;

    // ��ϢLog����
    bool RecordMessage() const { return _bRecordMessage; }
    void RecordMessage(bool val) { _bRecordMessage = val; }

    // ����Log���
    virtual wstring ToString() const { return L"Please override ToString() to record message data"; }

private:
    bool _bRecordMessage;    // �Ƿ�򿪱���ϢLog
};

/************************************************************************/
/* ��Ϣ���ࣨģ����������ࣩ
/************************************************************************/
template<class M>
class BaseMessage : public IMessage
{
    friend class MessageCenter;
public:
    virtual ~BaseMessage() {}

    // ��ݵĵ��÷�ʽ��ͬ��
    virtual bool Send() const
    {
        return Singleton::GetMessageCenter()->Send(this);
    }

    // ��ݵĵ��÷�ʽ���첽
    virtual void PostToUI() const 
    {
        PostToUI(Singleton::GetMainWnd());        
    }

    // ��ݵĵ��÷�ʽ��ͬ��
    virtual void SendToUI() const 
    {
        SendToUI(Singleton::GetMainWnd());        
    }

    // ��ݵĵ��÷�ʽ��ͬ�����ȴ�����ֵ
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

    // ������Ϣ
    virtual bool SendAndGetReturn(IHandler* pHandler)
    {
        BaseGetData<M>* p = dynamic_cast<BaseGetData<M>*>(pHandler);
        assert ( p );
        if ( ! p ) return false;

        Singleton::GetMessageCenter()->RecordHandle(p, this);
        return p->Handle((M*) this);
    }

    // ����Ϣ�Ŀ�ʼ֪ͨ
    virtual void Begin(IHandler* pHandler) const
    {
        BaseHandler<M>* p = dynamic_cast<BaseHandler<M>*>(pHandler);
        assert ( p );
        if ( ! p ) return;
        return p->Begin((const M*)this);
    }

    // ����Ϣ�Ľ���֪ͨ
    virtual void End(IHandler* pHandler) const
    {
        BaseHandler<M>* p = dynamic_cast<BaseHandler<M>*>(pHandler);
        assert ( p );
        if ( ! p ) return;
        return p->End((const M*)this);
    }

public:
    int nRequestID;    //ͨ��������
    int nUserData;     //ͨ���û�����
    int nParam;        //ͨ��int����
    wstring strParam;  //ͨ��wsring����
};

/************************************************************************/
/* ��Ϣ������ࣨģ�������Ҫ�������Ϣ��
/************************************************************************/
template<class M>
class BaseHandler : public IHandler
{
public:
    BaseHandler() { Singleton::GetMessageCenter()->Attach(this); }
    virtual ~BaseHandler() { Singleton::GetMessageCenter()->Detach(this); }
    virtual string GetType() { return typeid(M).raw_name(); }

    // ������Ҫʵ�֣���Ϣ�Ĵ�����
    virtual bool Handle(const M* pMsg) = 0;

    // ���ఴ��ʵ�֣�����Ϣ����ʼ֪ͨ
    virtual void Begin(const M* pMsg) {}

    // ���ఴ��ʵ�֣�����Ϣ�Ľ���֪ͨ
    virtual void End(const M* pMsg) {}
};

/************************************************************************/
/* ��ʱ���ؽ������Ϣ������ࣨģ�������Ҫ�������Ϣ��
/************************************************************************/
template<class M>
class BaseGetData : public BaseHandler<M>
{
public:
    BaseGetData() : BaseHandler<M>() {}

    // ��Ҫʵ�ַ� const �����ķ���
    virtual bool Handle(M* pMsg) = 0;

private:
    // ��ʵ��const�����ķ���
    virtual bool Handle(const M* pMsg) 
    {
		assert(false); 
        return true; 
    }
};
