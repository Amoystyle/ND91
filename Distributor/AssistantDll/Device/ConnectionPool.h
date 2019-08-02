///////////////////////////////////////////////////////////
//  ConnectionPool.h
//  Implementation of the Class ConnectionPool
//  Created on:      01-六月-2011 9:42:51
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_B7CD0339_E0A3_4b8c_918D_96B3D2599477__INCLUDED_)
#define EA_B7CD0339_E0A3_4b8c_918D_96B3D2599477__INCLUDED_

#include <string>
using namespace std;

// TODO: 确认是否放到OPTION
#define	CONNECT_OFF_TIME			10000	//连接超时时间
#define	DEFAULT_CONNECT_MIN_NUM		1		//默认最小连接数
#define	DEFAULT_CONNECT_MAX_NUM		20		//默认最大连接数

//连接池管理线程
#ifdef WIN32
    ::DWORD WINAPI ConnectionPoolRun(LPVOID lpParameter);
#else
	void* ConnectionPoolRun (void* lpParameter);
#endif

#include "Common/BaseSocket.h"
namespace ND91Assistant
{

	class CBaseSocket;
    class DeviceInfo;

    // 连接池：每个设备需要一个连接池，应该挂接在DeviceData上
    // 目前暂时不做连接控制，取连接就new，返回连接就delete
    // 目前Android守护显示，至少需要保持一个连接
	class ConnectionPool
    {
    private:
        // 连接池管理的连接
        class IOConnection
        {
        public:
            IOConnection(int nID, string host, UINT nPort, bool bNoDelay=true)
			{
				InitializeCriticalSection(&_csLock);
                _id = nID;
                _bUsed = false; 
                _host = host;
                _nPort = nPort;
                _bNoDelay = bNoDelay;
                _pBaseSocket = NULL;
                CreateSocket();
            };
            ~IOConnection()
            {          
				CloseSocket();

				DeleteCriticalSection(&_csLock);
                SAFE_DELETE(_pBaseSocket);
            };
            // 获取连接ID
            int GetID() { return _id; };
            // 判断连接是否正在使用
            bool IsUsed() 
            {
                // 检查的时候如果发现连接已经不可用，则马上重建
                if (!_pBaseSocket->IsSocketAvailable())
                {
                    CreateSocket(); // 重建Socket
                    return false;
                }
                return _bUsed; 
            };
            // 设置连接是否正在使用
            void SetUsed( bool bUsed ) 
            { 
                _bUsed = bUsed; 
                if ( !bUsed ) //连接还回的时候记录时间
                    _nLastUseTime = ::GetTickCount();
            };            
             //该连接空闲时间
            DWORD GetFreeTime()
            {
                if (!_bUsed)
                    return ::GetTickCount() - _nLastUseTime;
                return 0;
            };
            // 管理连接
            void CloseSocket()
            {
				SetCSLock(_csLock)

                _pBaseSocket->close();
				SAFE_DELETE(_pBaseSocket);
            };
            // 建立连接
            bool CreateSocket()
			{
				SetCSLock(_csLock)

                if (_pBaseSocket) _pBaseSocket->close();
                _pBaseSocket = new CBaseSocket();
               return _pBaseSocket->create(_host.c_str(), _nPort, _bNoDelay);
            }        
            CBaseSocket* GetSocket() { return _pBaseSocket; };
        private:
            DWORD _nLastUseTime;	//该连接最后使用时间
            int _id;                    // 索引
            bool _bUsed;                // 连接状态
            string _host;
            short _nPort;
            bool _bNoDelay;
            CBaseSocket* _pBaseSocket;  // Socket

			CRITICAL_SECTION _csLock;
        };

    public:
        ConnectionPool(const DeviceInfo* pDeviceInfo);
        virtual ~ConnectionPool();

        void ReleaseAll();
	private:
        string _strHost;
        short _nPort;
		vector<IOConnection*> _ConnectionList;

		int	_nMinThreadNumber;	//开辟的最小连接数
		int _nMaxThreadNumber;	//开辟的最大连接数
		int	_nThreadNumber;		//当前开辟的连接数
        int _nCurID;            //当前分配到的ID
        int _nLastFindIndex;    

		HANDLE _hThread;        //开辟的管理线程句柄
		bool _bThreadQuitSign;  //线程退出信号

		CRITICAL_SECTION _cs;//访问临界值
    private:
        // 连接池线程
#ifdef WIN32
        static DWORD WINAPI ConnectionPoolRun(void* lpParameter);
#endif
		//创建一个连接
        ConnectionPool::IOConnection* CreateConnetion();
		bool SetQuitSign() { return _bThreadQuitSign; }
		// 检查超时
		void CheckOffTime();
        // 寻找可用的连接
        ConnectionPool::IOConnection* FindAvail();
        // 是否可以继续创建连接
        bool IsCanCreate();
        // 获取当前连接数
        int GetConnectCount();
        // 设置为使用中
        void SetConnectionUsed(int nID, bool bUsed);
        // 查找连接
        ConnectionPool::IOConnection* FindConnection(int nID);
    public:
		/************************************************************************/
		/*初始化连接池
		strHost 目标IP
		nPort   目标端口
		nConnectType 连接类型 0USB 1WIFI
		nMinThread 保持的最少连接数
		nMaxThread 可开辟的最大连接数
		*/
		/************************************************************************/
		void init(int nMinThread=DEFAULT_CONNECT_MIN_NUM, int nMaxThread=DEFAULT_CONNECT_MAX_NUM);

		//取连接
        int Get();
		// 还连接
        void Putback(int nID);
        // 重建连接
        bool ResetConnect(int nID);
        // 发送数据
        bool SendData(int nID, const char* szData, UINT nLen);
        // 接收数据
        string ReceiveData(int nID, unsigned int size = 0);
        // 连接状态
        int GetConnectState(int nID);
        // 检查连接是否有效
        bool IsConnectionAvailable(int nID);
    };
}

#endif // !defined(EA_B7CD0339_E0A3_4b8c_918D_96B3D2599477__INCLUDED_)
