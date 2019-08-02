#pragma once

#include <string>
using namespace std;

namespace ND91Assistant
{
    class ConnectionPool;

    enum SOCKET_STATE{
	    SOCKET_STATE_NONE = 0,
	    SOCKET_STATE_CONNECT,
	    SOCKET_STATE_CREATE,
	    SOCKET_STATE_SEND_ING,
	    SOCKET_STATE_SEND_ED,
	    SOCKET_STATE_RECEIVE_ING,
	    SOCKET_STATE_RECEIVE_ED,
	    SOCKET_STATE_CLOSE,
	    SOCKET_STATE_ERROR
    };
    #define SOCKET_TIME_OUT		10
    #define SOCKET_READ_TIME	10
    #define SOCKET_WRITE_TIME	2
    #define RECEIVE_BUFFER_SIZE	10240

    class CBaseSocket
    {
    public:
	    CBaseSocket(void);
	    ~CBaseSocket(void);

		//用于IOS服务通信，socket由ITUNES函数创建提供
		CBaseSocket(SOCKET hSocket);
    private:
	    char m_pHost[256];
	    int m_nPort;
	    SOCKET m_hSocket;
	    bool m_bCreate;
	    char m_pRecBuffer[RECEIVE_BUFFER_SIZE];
	    UINT m_nBuffer;
	    LPSTR lpBuffer;    
		int _timerOut;

    public:
	    int m_nState;
    public:
	    /*
	    创建一个Socket,并连接
	    szHost : 目标地址
	    nPort : 目标端口
	    bNoDelay : 是否超时返回
	    */
	    bool create(const char* szHost, UINT nPort, bool bNoDelay=true,int DelayTime=30000);
	    //set NoDelay
	    bool setopt(int DelayTime);
	    //发送数据
	    bool sendData(const char* szData, UINT nLen=0);
	    /*
	    接收数据
	    ppData 要存放的指针地址
	    nLen 可放的空间大小，0表示需要在内部申请空间
	    return 接收的大小
	    */
	    size_t receiveData(char** ppData, UINT &nLen, int nRetryTime=SOCKET_READ_TIME);
        //接收数据 nSize=0 表示接收所有数据， nSize>0表示接收nSize个字节
		//nRetryTime 重试次数
	    string receiveData(UINT nSize=0, int nRetryTime=3);
	    //string receiveData();
	    //只接收nReceiveSize个字节的数据，并存放在pData中
	    bool receiveDataLength(char* pData, UINT nReceiveSize, int nRetryTime=SOCKET_READ_TIME);	
	    //关闭Socket连接
	    bool close();
	    //Socket是否可用
	    bool isValid();
	    //获取Socket状态
	    int	 getState();
        //Socket是否可用
        bool IsSocketAvailable();
	    //获取错误信息 - temp
	    int getLastError();
		//获得socket与守护通讯是否有错误
		int GetSockLastError();
        //连接远程端口后，会在本地产生一个对应的端口，本函数返回这个端口，需要在Connect连接后获取
        UINT getLocalPort();
        void SetTimeout(int nTimeout) {_nTimeout = nTimeout;};
		//判断是否能收到数据，用于线程池回收的时候判断是否要丢弃
		bool  HasBuffData();
    public:
	    static short getActiveSocketPort();
	    static bool isPortUsed(const UINT nPort);


    /**** 以下为连接池相关 **************************************/
    private:
		DWORD _nLastUseTime;	//该连接最后使用时间
		bool _bNeedCloseSocket;
        int _nTimeout;
        int _nLastSocketError;
    public:
        //设置连接池
        void setConnectionPool(ConnectionPool* pPool);
        //将连接放回连接池
        void putbackConnectionPool();
		//GetFreeTime
		int GetFreeTime();
    
        char* GetHost();
        int GetPort();
        void Release();
    };
}
