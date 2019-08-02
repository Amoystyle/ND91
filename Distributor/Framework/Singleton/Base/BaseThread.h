#pragma once

#define CHECK_EXIT  if (CheckExit()) return 0;
#define WAIT_EXIT(t)   if (WaitExit(t)) return 0;

// 线程基类，通过ThraedManager可以根据ID获得指针
class SINGLETON_API BaseThread
{
    friend class ThreadManager;
public:

    BaseThread(const string id=""); // id：线程id，不重复的非空字符串
    virtual ~BaseThread();

    // 设定是否在线程结束时删除本对象
    void SetAutoDelete(bool value);

    // 启动线程，返回成功或失败
    bool Run();

    // 停止线程
    void Exit();

	HANDLE          _hThread;       // 线程句柄

    int GetGUID() { return _nGUID; };
protected:
    wstring         _deviceID;      // 对应的设备id,为空表示无对应设备；设备断开时用来终止对应的线程
    int             _nGroup;        // 线程分组
    int             _nGUID;     // 线程标志

    
    string          _id;            // 线程ID

    
    // 下面是退出机制，子类可以使用二个方法，或直接使用变量

    // 检测退出标志
    virtual bool CheckExit();

    // 等待系统退出：用户可自己调用Wait函数
    // return : bool 是否要退出
    virtual bool WaitExit(DWORD dwMilliseconds=INFINITE);

    HANDLE      _hExitEvent;        // 线程退出事件  
    bool        _exit;              // 线程退出标志变量

    HANDLE      _hSystemExitEvent;  // 系统退出事件  
    const bool* _pSystemExit;       // 系统退出标志变量

    bool        _autoDelete;        // 线程结束时删除本对象
private:

    // 线程执行函数    
    static DWORD WINAPI ThreadMain( LPVOID lpParam ) ;

	void InitPrintThreadID();

	string strPrintThreadID;
    // 实际线程执行函数，子类实现
    virtual unsigned long Main() = 0;

};