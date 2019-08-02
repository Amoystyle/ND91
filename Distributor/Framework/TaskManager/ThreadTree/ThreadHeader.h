/***************************************************************************//**
 *  @file 
 *  @brief [线程控制器的相关结构定义与宏声明头文件]
 * 
 *  <b>文件名</b>      : ThreadHeader.h
 *  @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发组
 *  @n@n<b>作  者</b>  : alzq
 *  @n@n<b>创建时间</b>: 09-2-6 9:56:00
 *  @n@n<b>文件描述</b>: 
 *  @version  版本        修改者        时间        描述@n
 *  @n        [版本号]    alzq          2009-02-06  [描述]
 * 
*******************************************************************************/
#ifndef __THREAD_HEADER__
#define __THREAD_HEADER__

#include <list>

using namespace std;

//信号类型----------锁，信号量
#define SINGLE_EVENT						0x01
#define SINGLE_MUTEX						0x02

//线程等待状态
#define WAIT_INFINTE						0x01
#define WAIT_TIME_LIMIT						0x02

//释放锁的释放结果
#define RELEASE_MUTEX_ERROR					0x01
#define RELEASE_MUTEX_FAIL					0x02
#define RELEASE_MUTEX_SUC					0x04

//操作信号量的释放结果
#define SET_EVENT_ERROR						0x01
#define SET_EVENT_FAIL						0x02
#define SET_EVENT_SUC						0x04

//等待信号或锁时的返回结果意义
#define WAIT_SINGLE_ERROR					0x01		//等待的信号指针错误
#define WAIT_SINGLE_TIMEOUT					0x02		//等待超出时间限制
#define WAIT_SINGLE_SUC						0x04		//等待成功
#define WAIT_SINGLE_SUC_THREAD_EXIT			0x08		//等待成功，但是获取锁权限的线程退出并未释放锁，此结果只存在等待单个锁的调用函数返回

//线程状态
#define THREAD_STATE_ERROR					0x01		//等待状态
#define THREAD_STATE_WAIT					0x02		//等待状态
#define THREAD_STATE_TIMEOUT				0x04		//最后一个等待结果为超时
#define THREAD_STATE_SUC					0x08		//最后一个等待结果为获取成功
#define THREAD_STATE_SUC_THREAD_EXIT		0x10		//最后一个等待结果为获取成功，但是获取锁权限的线程退出并未释放锁，此结果只存在等待单个锁的调用函数返回
#define THREAD_STATE_CLOSE					0x20		//关闭线程

//等待的锁链表里的锁状态
#define WAIT_STATE_WAIT						0x01		//还在等待
#define WAIT_STATE_SUC						0x02		//已经获取成功

//安全释放指针宏
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if (NULL != p) {delete p;p = NULL;}}
#endif
#define SAFE_DELETE_CHARS(p) {if (NULL != p) {delete [] p;p = NULL;}}

//线程控制对象信息存储结构体
struct THREADTARGET
{
	BOOL isMainThread;			//是否主线程
	HANDLE controlEvent;		//用以阻塞线程的对象
	BOOL isWinThread;			//是否窗口线程
	HANDLE handle;				//线程句柄
	LPVOID winThread;			//窗口线程的指针
	DWORD threadId;				//线程ID用来进行线程的识别

	THREADTARGET()
	{
		isMainThread = FALSE;			//是否主线程
		isWinThread = FALSE;
		handle = NULL;
		winThread = NULL;
		threadId = NULL;

		controlEvent = CreateEvent(NULL, TRUE, FALSE, NULL);		//用以阻塞线程的对象
	}
	~THREADTARGET()
	{
		if(NULL != controlEvent)
		{
			CloseHandle(controlEvent);
			controlEvent = NULL;
		}
	}
};

//用户通过控制类访问线程时，可能需要查询的信息存储结构体
struct THREADTOUSERINFO
{
	BOOL threadEnable;			//线程当前是否有效

	THREADTOUSERINFO()
	{
		threadEnable = FALSE;
	}
};

//树型结构中存储锁或信号量的节点类
struct SINGLE
{
public:
protected:
	LPVOID lpSingle;			//存储相关节点的地址变量
	DWORD singleID;				//在树型结构中的位置表示变量
	UINT singleType;			//表示是锁还是信号量的变量
	bool logEnable;				//是否需要进行LOG输出

public:
	SINGLE()
	{
		singleType = NULL;
		lpSingle = NULL;
		singleID = NULL;
		logEnable = false;
	}
	SINGLE(UINT type, LPVOID lp, DWORD id)
	{
		singleType = type;
		lpSingle = lp;
		singleID = id;
	}
	UINT GetType()
	{
		return singleType;
	}
	UINT GetId()
	{
		return singleID;
	}
	LPVOID GetSingle()
	{
		return lpSingle;
	}
	void SetLogEnable(bool enable)
	{
		logEnable = enable;
	}
	bool GetLogEnable()
	{
		return logEnable;
	}
};

//用户等待的锁或信号量表示变量，以及获取状态
struct WAITSTATE
{
	SINGLE * singleTarget;			//等待的锁或信号量的标志变量
	UINT state;						//获取状态（锁），等待状态（信号量）

	WAITSTATE()
	{
		singleTarget = NULL;
		state = NULL;
	}
	WAITSTATE(SINGLE * single, UINT st)
	{
		singleTarget = single;
		state = st;
	}
};

typedef		TCHAR								CTCHAR;

typedef		list<SINGLE *>						SINGLELIST,EventList,MutexList;
typedef		list<WAITSTATE *>					SINGLEWAITLIST;
typedef		list<LPVOID>						THREADLIST;

#endif
