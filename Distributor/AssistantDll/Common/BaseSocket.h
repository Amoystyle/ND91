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

		//����IOS����ͨ�ţ�socket��ITUNES���������ṩ
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
	    ����һ��Socket,������
	    szHost : Ŀ���ַ
	    nPort : Ŀ��˿�
	    bNoDelay : �Ƿ�ʱ����
	    */
	    bool create(const char* szHost, UINT nPort, bool bNoDelay=true,int DelayTime=30000);
	    //set NoDelay
	    bool setopt(int DelayTime);
	    //��������
	    bool sendData(const char* szData, UINT nLen=0);
	    /*
	    ��������
	    ppData Ҫ��ŵ�ָ���ַ
	    nLen �ɷŵĿռ��С��0��ʾ��Ҫ���ڲ�����ռ�
	    return ���յĴ�С
	    */
	    size_t receiveData(char** ppData, UINT &nLen, int nRetryTime=SOCKET_READ_TIME);
        //�������� nSize=0 ��ʾ�����������ݣ� nSize>0��ʾ����nSize���ֽ�
		//nRetryTime ���Դ���
	    string receiveData(UINT nSize=0, int nRetryTime=3);
	    //string receiveData();
	    //ֻ����nReceiveSize���ֽڵ����ݣ��������pData��
	    bool receiveDataLength(char* pData, UINT nReceiveSize, int nRetryTime=SOCKET_READ_TIME);	
	    //�ر�Socket����
	    bool close();
	    //Socket�Ƿ����
	    bool isValid();
	    //��ȡSocket״̬
	    int	 getState();
        //Socket�Ƿ����
        bool IsSocketAvailable();
	    //��ȡ������Ϣ - temp
	    int getLastError();
		//���socket���ػ�ͨѶ�Ƿ��д���
		int GetSockLastError();
        //����Զ�̶˿ں󣬻��ڱ��ز���һ����Ӧ�Ķ˿ڣ���������������˿ڣ���Ҫ��Connect���Ӻ��ȡ
        UINT getLocalPort();
        void SetTimeout(int nTimeout) {_nTimeout = nTimeout;};
		//�ж��Ƿ����յ����ݣ������̳߳ػ��յ�ʱ���ж��Ƿ�Ҫ����
		bool  HasBuffData();
    public:
	    static short getActiveSocketPort();
	    static bool isPortUsed(const UINT nPort);


    /**** ����Ϊ���ӳ���� **************************************/
    private:
		DWORD _nLastUseTime;	//���������ʹ��ʱ��
		bool _bNeedCloseSocket;
        int _nTimeout;
        int _nLastSocketError;
    public:
        //�������ӳ�
        void setConnectionPool(ConnectionPool* pPool);
        //�����ӷŻ����ӳ�
        void putbackConnectionPool();
		//GetFreeTime
		int GetFreeTime();
    
        char* GetHost();
        int GetPort();
        void Release();
    };
}
