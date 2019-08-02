#pragma once

#include "../Base/BaseThread.h"
#include "../Base/BaseCSLock.h"

// �̹߳����࣬����
// ���̰߳�ȫ��true
class SINGLETON_API ThreadManager : public BaseCSLock
{
    friend class Singleton;
public:

    // �����߳�����
    void SetThreadID(int nID);

    // ��ȡ�߳�����
    int GetThreadID() const;

    // �����߳�
    void InsertThread(BaseThread* pThread, string id);

    // ȡ�߳�
    BaseThread* GetThread(string id);
    BaseThread* GetThread(int nGUID);

    // �Ƴ��߳�
    void RemoveThread(BaseThread* pThread);

    // �Ƴ��߳�
    void RemoveThread(string id);

    // ��ȡGUID
    int GetGUID();

    // �˳��߳�
    void ExitThread(string id);
    void ExitThread(int nGUID);

    // �˳����в����豸����ΪdeviceID���߳�
    void ExitThreads(wstring deviceID);
private:
    ThreadManager();    // private��ֻ����Singleton�д���
    ~ThreadManager();

    // key: �߳�ID
    // value���߳���ָ��
    typedef map<string, BaseThread*> THREAD_MAP;

    THREAD_MAP _threads;        // �߳��б�
    int _nThreadID;             // �߳�����
    int _nGUID;                 // ȫ������
};