/********************************************************************
	created:	2012/12/06 11:54            author:		��
	
	purpose:	����Ϣ��
*********************************************************************/
#pragma once
#include "Message.h"

/************************************************************************/
/* ����Ϣ
/*
/* ʹ��ע�⣺
/*      ÿ�δ�������Ϣ������һ���µ���
/*      ֻ�ᷢ�͸�������Ϣʱ���ѵǼǽ�����Ϣ��Handler����
/*      ��ֵ����͸�ֵ�����п������뾡������
/*
/* ʵ��ԭ��
/*      ��ʼ��ʱ��¼���н����ߣ�����ʱ����Ƿ��Ѽ�¼�Ľ�����
/*      �����ַ�ʽ����һ�������Ľ����߽����յ���벿�ֵ���Ϣ
/************************************************************************/
template<class M>
class GroupMessage : public BaseMessage<M>
{
public:
	GroupMessage() : BaseMessage<M>() { _handlerArray = 0; _arrayLength = 0;  }
    virtual ~GroupMessage() { Release(); }
    // ��������
    GroupMessage(const GroupMessage& rhs)
    {
        _handlerArray = 0;
        _arrayLength = 0;
        Copy(rhs);
    }

    // ��ֵ����
    GroupMessage& operator = (const GroupMessage& rhs)
    {
        if ( this == &rhs ) return *this;
        Copy(rhs);

        return *this;
    }

    // ֪ͨ Handler������Ϣ��ʼ
    void Begin()
    {
        Init();
        LPARAM p = (LPARAM)new M(*((M*)this));
        assert ( p );
        if ( !p ) return;
        SendMessage(Singleton::GetMainWnd(), WM_GROUP_BEGIN, NULL, p);
    }

    // ֪ͨ Handler������Ϣ����
    void End()
    {
        LPARAM p = (LPARAM)new M(*((M*)this));
        assert ( p );
        if ( !p ) return;
        SendMessage(Singleton::GetMainWnd(), WM_GROUP_END, NULL, p);
    }

private:

    // ���ͣ�����ǰ���
    virtual bool SendTo(IHandler* pHandler) const
    {
        if ( pHandler && ! Exist(pHandler) ) return false;

        BaseHandler<M>* p = (BaseHandler<M>*)pHandler;
        return p->Handle((const M*) this);
    }

    // ��ʼ�������캯���е��ã�
    void Init()
    {
        _handlerArray = 0;
        _arrayLength = 0;

        // ���浱ǰ����Ϣ������
        Singleton::GetMessageCenter()->GroupMsgHandler(this, _handlerArray, _arrayLength);
    }

    // �ͷ��ڴ�
    void Release()
    {
        if (_handlerArray && _arrayLength)
        {
            if (_handlerArray)
            {
                delete[] _handlerArray;
                _handlerArray = 0;
            }
            _arrayLength = 0;
        }
    }

    // ����
    void Copy(const GroupMessage& rhs)
    {
        Release();
        this->_arrayLength = rhs._arrayLength;
        this->_handlerArray = new IHandler*[this->_arrayLength];
        memcpy(this->_handlerArray, rhs._handlerArray, 
            sizeof(IHandler*) * this->_arrayLength);

        this->RecordMessage(rhs.RecordMessage());
    }

    // �ж��Ƿ���Ϣ��Ľ�����
    bool Exist(IHandler* pHandler) const
    {
        for(unsigned int i = 0; i < _arrayLength; ++i)
        {
            if ( _handlerArray[i] == pHandler )
            {
                return true;
            }
        }
        return false;
    }

private:
    IHandler**      _handlerArray;  // ��Ϣ����������
    unsigned int    _arrayLength;   // ��Ϣ����������ָ��
};

