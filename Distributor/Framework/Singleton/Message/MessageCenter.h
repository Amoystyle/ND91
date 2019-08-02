#pragma once
#include "Base/BaseCSLock.h"
#include "Base/SingletonDefine.h"

#pragma warning (disable:4251)

class IMessage;
class IHandler;

// ��Ϣ����
class SINGLETON_API MessageCenter : private BaseCSLock
{
    friend class Singleton;
public:
    // ��ָ����Ϣ����ӽ�����
    void Attach(IHandler* pHandler);

    // ��ָ����Ϣ���Ƴ�������
    void Detach(IHandler* pHandler);

    // �㲥��Ϣ��ͬ���������Ƿ��н�����
    bool Send(const IMessage* pMsg);

    // ��¼��Ϣ
    inline void RecordMessage( const IMessage* pMsg ) const;
    inline void RecordMessage( const IMessage* pMsg, bool begin ) const;
    inline void RecordHandle( IHandler* p, const IMessage* pMsg ) const;

    // ͨ��Windows��Ϣ���ƹ㲥��Ϣ���첽��
    void SendToUI(HWND hWnd, const IMessage* pMsg);

    // ͨ����Ϣ��ȡ���ݣ�ֱ������ϢIMessage�ڷ��ؽ��
    void GetData(IMessage* pMsg);

    // ���ԣ��Ƿ��¼��Ϣ
    bool GetRecordMessage() const { return _recordMessage; }
    void SetRecordMessage(bool val) { _recordMessage = val; }

    // ��������Ϣ�Ľ�����
    void GroupMsgHandler(IMessage* pMsg, IHandler**& _receiverArray, unsigned int& _arrayLength );

    // ����Ϣ��ʼ��ͬ����
    void Begin(const IMessage* pMsg);

    // ����Ϣ������ͬ����
    void End(const IMessage* pMsg);

private:
    MessageCenter();
    virtual ~MessageCenter();

private:
    typedef list<IHandler*>     HANDLER_LIST;    // ����Handler���б�

    map<string, HANDLER_LIST*>  _handlers;       // ������Ϣ������

    CRITICAL_SECTION            _msgLock;        // _msgReceivers����

    bool _recordMessage;    // �Ƿ��¼��Ϣ���ܿ��أ�ÿ����Ϣ���Լ����أ�
};
