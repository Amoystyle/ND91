#pragma once

#include "../Base/BaseThread.h"
#include "../Base/BaseCSLock.h"

// 线程管理类，单例
// 多线程安全：true
class SINGLETON_API ThreadManager : public BaseCSLock
{
    friend class Singleton;
public:

    // 设置线程索引
    void SetThreadID(int nID);

    // 获取线程索引
    int GetThreadID() const;

    // 增加线程
    void InsertThread(BaseThread* pThread, string id);

    // 取线程
    BaseThread* GetThread(string id);
    BaseThread* GetThread(int nGUID);

    // 移除线程
    void RemoveThread(BaseThread* pThread);

    // 移除线程
    void RemoveThread(string id);

    // 获取GUID
    int GetGUID();

    // 退出线程
    void ExitThread(string id);
    void ExitThread(int nGUID);

    // 退出所有操作设备对象为deviceID的线程
    void ExitThreads(wstring deviceID);
private:
    ThreadManager();    // private，只能在Singleton中创建
    ~ThreadManager();

    // key: 线程ID
    // value：线程类指针
    typedef map<string, BaseThread*> THREAD_MAP;

    THREAD_MAP _threads;        // 线程列表
    int _nThreadID;             // 线程索引
    int _nGUID;                 // 全局索引
};