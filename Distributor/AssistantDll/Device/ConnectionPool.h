///////////////////////////////////////////////////////////
//  ConnectionPool.h
//  Implementation of the Class ConnectionPool
//  Created on:      01-����-2011 9:42:51
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_B7CD0339_E0A3_4b8c_918D_96B3D2599477__INCLUDED_)
#define EA_B7CD0339_E0A3_4b8c_918D_96B3D2599477__INCLUDED_

#include <string>
using namespace std;

// TODO: ȷ���Ƿ�ŵ�OPTION
#define	CONNECT_OFF_TIME			10000	//���ӳ�ʱʱ��
#define	DEFAULT_CONNECT_MIN_NUM		1		//Ĭ����С������
#define	DEFAULT_CONNECT_MAX_NUM		20		//Ĭ�����������

//���ӳع����߳�
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

    // ���ӳأ�ÿ���豸��Ҫһ�����ӳأ�Ӧ�ùҽ���DeviceData��
    // Ŀǰ��ʱ�������ӿ��ƣ�ȡ���Ӿ�new���������Ӿ�delete
    // ĿǰAndroid�ػ���ʾ��������Ҫ����һ������
	class ConnectionPool
    {
    private:
        // ���ӳع��������
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
            // ��ȡ����ID
            int GetID() { return _id; };
            // �ж������Ƿ�����ʹ��
            bool IsUsed() 
            {
                // ����ʱ��������������Ѿ������ã��������ؽ�
                if (!_pBaseSocket->IsSocketAvailable())
                {
                    CreateSocket(); // �ؽ�Socket
                    return false;
                }
                return _bUsed; 
            };
            // ���������Ƿ�����ʹ��
            void SetUsed( bool bUsed ) 
            { 
                _bUsed = bUsed; 
                if ( !bUsed ) //���ӻ��ص�ʱ���¼ʱ��
                    _nLastUseTime = ::GetTickCount();
            };            
             //�����ӿ���ʱ��
            DWORD GetFreeTime()
            {
                if (!_bUsed)
                    return ::GetTickCount() - _nLastUseTime;
                return 0;
            };
            // ��������
            void CloseSocket()
            {
				SetCSLock(_csLock)

                _pBaseSocket->close();
				SAFE_DELETE(_pBaseSocket);
            };
            // ��������
            bool CreateSocket()
			{
				SetCSLock(_csLock)

                if (_pBaseSocket) _pBaseSocket->close();
                _pBaseSocket = new CBaseSocket();
               return _pBaseSocket->create(_host.c_str(), _nPort, _bNoDelay);
            }        
            CBaseSocket* GetSocket() { return _pBaseSocket; };
        private:
            DWORD _nLastUseTime;	//���������ʹ��ʱ��
            int _id;                    // ����
            bool _bUsed;                // ����״̬
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

		int	_nMinThreadNumber;	//���ٵ���С������
		int _nMaxThreadNumber;	//���ٵ����������
		int	_nThreadNumber;		//��ǰ���ٵ�������
        int _nCurID;            //��ǰ���䵽��ID
        int _nLastFindIndex;    

		HANDLE _hThread;        //���ٵĹ����߳̾��
		bool _bThreadQuitSign;  //�߳��˳��ź�

		CRITICAL_SECTION _cs;//�����ٽ�ֵ
    private:
        // ���ӳ��߳�
#ifdef WIN32
        static DWORD WINAPI ConnectionPoolRun(void* lpParameter);
#endif
		//����һ������
        ConnectionPool::IOConnection* CreateConnetion();
		bool SetQuitSign() { return _bThreadQuitSign; }
		// ��鳬ʱ
		void CheckOffTime();
        // Ѱ�ҿ��õ�����
        ConnectionPool::IOConnection* FindAvail();
        // �Ƿ���Լ�����������
        bool IsCanCreate();
        // ��ȡ��ǰ������
        int GetConnectCount();
        // ����Ϊʹ����
        void SetConnectionUsed(int nID, bool bUsed);
        // ��������
        ConnectionPool::IOConnection* FindConnection(int nID);
    public:
		/************************************************************************/
		/*��ʼ�����ӳ�
		strHost Ŀ��IP
		nPort   Ŀ��˿�
		nConnectType �������� 0USB 1WIFI
		nMinThread ���ֵ�����������
		nMaxThread �ɿ��ٵ����������
		*/
		/************************************************************************/
		void init(int nMinThread=DEFAULT_CONNECT_MIN_NUM, int nMaxThread=DEFAULT_CONNECT_MAX_NUM);

		//ȡ����
        int Get();
		// ������
        void Putback(int nID);
        // �ؽ�����
        bool ResetConnect(int nID);
        // ��������
        bool SendData(int nID, const char* szData, UINT nLen);
        // ��������
        string ReceiveData(int nID, unsigned int size = 0);
        // ����״̬
        int GetConnectState(int nID);
        // ��������Ƿ���Ч
        bool IsConnectionAvailable(int nID);
    };
}

#endif // !defined(EA_B7CD0339_E0A3_4b8c_918D_96B3D2599477__INCLUDED_)
