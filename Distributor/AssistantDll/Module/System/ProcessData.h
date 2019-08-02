#pragma once

#include "Module/BaseData.h"
#include <string>
using namespace std;
namespace ND91Assistant
{
    //用于解析手机返回的数据类，考虑到界面可能也需要用到，所以继承BaseData
    class ProcessData : public BaseData
    {
        friend class AdbPSCommand;
    public:
	    ProcessData();
	    virtual ~ProcessData();

        //进程的状态
	    enum AndroidProcessState 
	    {
		    Unknow, // 未知（默认）
		    D,      // 无法中断的休眠状态（通常 IO 的进程）
		    R,      // 正在运行可中在队列中可过行的
		    S,      // 处于休眠状态
		    T,      // 停止或被追踪
		    W,      // 进入内存交换（从内核2.6开始无效）
		    X,      // 死掉的进程
		    Z       // 僵尸进程
	    };

        virtual void Update();

	    string  _strUser;
	    int     _nPID;
	    int     _nPPID;
	    long    _nSize;
	    long    _nRss;
	    string  _strName;
	    AndroidProcessState _eState;



    private:
        bool ParseAndroidData(string strLine);
        
    };
}
