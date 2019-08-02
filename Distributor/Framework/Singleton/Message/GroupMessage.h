/********************************************************************
	created:	2012/12/06 11:54            author:		邱畅
	
	purpose:	组消息类
*********************************************************************/
#pragma once
#include "Message.h"

/************************************************************************/
/* 组消息
/*
/* 使用注意：
/*      每次创建组消息，都是一个新的组
/*      只会发送给创建消息时，已登记接收消息的Handler对象
/*      赋值构造和赋值构造有开销，请尽量减少
/*
/* 实现原理：
/*      初始化时记录所有接收者，发送时检查是否已记录的接收者
/*      用这种方式避免一个迟来的接收者仅接收到后半部分的消息
/************************************************************************/
template<class M>
class GroupMessage : public BaseMessage<M>
{
public:
	GroupMessage() : BaseMessage<M>() { _handlerArray = 0; _arrayLength = 0;  }
    virtual ~GroupMessage() { Release(); }
    // 拷贝构造
    GroupMessage(const GroupMessage& rhs)
    {
        _handlerArray = 0;
        _arrayLength = 0;
        Copy(rhs);
    }

    // 赋值构造
    GroupMessage& operator = (const GroupMessage& rhs)
    {
        if ( this == &rhs ) return *this;
        Copy(rhs);

        return *this;
    }

    // 通知 Handler，组消息开始
    void Begin()
    {
        Init();
        LPARAM p = (LPARAM)new M(*((M*)this));
        assert ( p );
        if ( !p ) return;
        SendMessage(Singleton::GetMainWnd(), WM_GROUP_BEGIN, NULL, p);
    }

    // 通知 Handler，组消息结束
    void End()
    {
        LPARAM p = (LPARAM)new M(*((M*)this));
        assert ( p );
        if ( !p ) return;
        SendMessage(Singleton::GetMainWnd(), WM_GROUP_END, NULL, p);
    }

private:

    // 发送：发送前检查
    virtual bool SendTo(IHandler* pHandler) const
    {
        if ( pHandler && ! Exist(pHandler) ) return false;

        BaseHandler<M>* p = (BaseHandler<M>*)pHandler;
        return p->Handle((const M*) this);
    }

    // 初始化（构造函数中调用）
    void Init()
    {
        _handlerArray = 0;
        _arrayLength = 0;

        // 保存当前的消息接收者
        Singleton::GetMessageCenter()->GroupMsgHandler(this, _handlerArray, _arrayLength);
    }

    // 释放内存
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

    // 复制
    void Copy(const GroupMessage& rhs)
    {
        Release();
        this->_arrayLength = rhs._arrayLength;
        this->_handlerArray = new IHandler*[this->_arrayLength];
        memcpy(this->_handlerArray, rhs._handlerArray, 
            sizeof(IHandler*) * this->_arrayLength);

        this->RecordMessage(rhs.RecordMessage());
    }

    // 判断是否本消息组的接收者
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
    IHandler**      _handlerArray;  // 消息接收者数组
    unsigned int    _arrayLength;   // 消息接收者数组指针
};

