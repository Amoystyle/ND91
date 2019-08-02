#pragma once
#include "Base/BaseCSLock.h"
#include "Base/SingletonDefine.h"

#pragma warning (disable:4251)

class IMessage;
class IHandler;

// 消息中心
class SINGLETON_API MessageCenter : private BaseCSLock
{
    friend class Singleton;
public:
    // 对指定消息，添加接收者
    void Attach(IHandler* pHandler);

    // 对指定消息，移除接收者
    void Detach(IHandler* pHandler);

    // 广播消息（同步）返回是否有接收者
    bool Send(const IMessage* pMsg);

    // 记录消息
    inline void RecordMessage( const IMessage* pMsg ) const;
    inline void RecordMessage( const IMessage* pMsg, bool begin ) const;
    inline void RecordHandle( IHandler* p, const IMessage* pMsg ) const;

    // 通过Windows消息机制广播消息（异步）
    void SendToUI(HWND hWnd, const IMessage* pMsg);

    // 通过消息获取数据，直接在消息IMessage内返回结果
    void GetData(IMessage* pMsg);

    // 属性：是否记录消息
    bool GetRecordMessage() const { return _recordMessage; }
    void SetRecordMessage(bool val) { _recordMessage = val; }

    // 设置组消息的接收者
    void GroupMsgHandler(IMessage* pMsg, IHandler**& _receiverArray, unsigned int& _arrayLength );

    // 组消息开始（同步）
    void Begin(const IMessage* pMsg);

    // 组消息结束（同步）
    void End(const IMessage* pMsg);

private:
    MessageCenter();
    virtual ~MessageCenter();

private:
    typedef list<IHandler*>     HANDLER_LIST;    // 所有Handler的列表

    map<string, HANDLER_LIST*>  _handlers;       // 所有消息接收者

    CRITICAL_SECTION            _msgLock;        // _msgReceivers的锁

    bool _recordMessage;    // 是否记录消息（总开关，每个消息有自己开关）
};
