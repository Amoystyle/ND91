// ������ɨ�蹦���ѱ�����

#include "stdafx.h"
#include "WifiScaner.h"

WifiScaner::WifiScaner(void)
{
    _bSendThreadQuitSign = false;
    _bReceThreadQuitSign = false;
    _hSendThread = NULL;
    _hReceThread = NULL;
}

WifiScaner::~WifiScaner(void)
{
    clearDevice();
}

DWORD WINAPI WifiScaner::SendData(LPVOID lpParameter)
{
    WifiScaner* pThread = (WifiScaner*)lpParameter;
    Sleep(1000);
    //����Socket
    SOCKET hSocket = socket( AF_INET, SOCK_DGRAM, 0);

    //����������Ϣ
    SOCKADDR_IN addrin;
    addrin.sin_family = AF_INET;
    // TODO: ��ȡ��ǰ������ IP
    addrin.sin_addr.s_addr = inet_addr("172.25.1.255");
    addrin.sin_port = htons(WIFI_UDP_PORT);

//    bool optval = true;
//    setsockopt(hSocket,SOL_SOCKET,SO_BROADCAST,(char FAR *)&optval,sizeof(optval));


    int nSend = 0;
    while (!pThread->getSendThreadQuitSign())
    {
        //�����η���
        addrin.sin_addr.s_addr = inet_addr("172.25.1.255");
        nSend = sendto(hSocket, 
            WIFI_UDP_FROM_PC, 
            sizeof(WIFI_UDP_FROM_PC)-1, 
            0, 
            (SOCKADDR*)&addrin, 
            sizeof(SOCKADDR));
        Sleep(500);
        //continue;
        //���¶��ض���IP��ַ����
        addrin.sin_addr.s_addr = inet_addr("172.25.1.224");
        nSend = sendto(hSocket, 
            WIFI_UDP_FROM_PC, 
            sizeof(WIFI_UDP_FROM_PC)-1, 
            0, 
            (SOCKADDR*)&addrin, 
            sizeof(SOCKADDR));

        Sleep(500);
    }
    closesocket(hSocket);
    return 0L;
}

DWORD WINAPI WifiScaner::ReceData(LPVOID lpParameter)
{
    WifiScaner* pThread = (WifiScaner*)lpParameter;

    //����Socket
    SOCKET hSocket = socket( AF_INET, SOCK_DGRAM, 0);

    //����������Ϣ
    SOCKADDR_IN addrin;
    addrin.sin_family = AF_INET;
    addrin.sin_addr.s_addr = htonl(INADDR_ANY);
    addrin.sin_port = htons(46923);

    bool optval = true;
    setsockopt(hSocket,SOL_SOCKET,SO_BROADCAST,(char FAR *)&optval,sizeof(optval));

    //���׽���
    bind(hSocket,(SOCKADDR*)&addrin,sizeof(SOCKADDR));

    //����UDP����
    SOCKADDR_IN addrClient;
    int len=sizeof(SOCKADDR);
    char receBuf[1024]={0};
    int  nRece = 0;
    while(!pThread->getReceThreadQuitSign())
    {  
        nRece = recvfrom(hSocket,
            receBuf,
            100,
            0,
            (SOCKADDR*)&addrClient,
            &len);
        if (nRece > 0)
        {
            if (memcmp(receBuf, WIFI_UDP_FROM_DEMON_HEAD, sizeof(WIFI_UDP_FROM_DEMON_HEAD)-1) == 0)
            {//��⵽�㲥��Wifi����
                vector<string> strList = CStrOperation::parseStrings(receBuf, nRece, '|');
                if (strList.size() == 4)
                {
                    //��⵽�豸
                    pThread->AddNewDevice(inet_ntoa(addrClient.sin_addr),
                        strList[1], strList[2], strList[3]);
                }
            }
        }
    }
    closesocket(hSocket);
    return 0L;
}

void WifiScaner::StartThread()
{
    if (_hSendThread && _hReceThread)
        return;

    _bSendThreadQuitSign = false;
    _bReceThreadQuitSign = false;
    _hSendThread = CreateThread(NULL, 0, WifiScaner::SendData,(LPVOID)this, 0, &_nSendThreadId);
    _hReceThread = CreateThread(NULL, 0, WifiScaner::ReceData,(LPVOID)this, 0, &_nReceThreadId);
}

void WifiScaner::StopThread()
{
    if (_hSendThread)
    {
        _bSendThreadQuitSign = true;
        WaitForSingleObject(_hSendThread,INFINITE);
        _hSendThread = NULL;
    }
    if (_hReceThread)
    {
        _bReceThreadQuitSign = true;
        WaitForSingleObject(_hReceThread,INFINITE);
        _hReceThread = NULL;
    }
}

void WifiScaner::AddNewDevice(string ip, string imei, string mac, string model)
{
    //�ж��ظ�
    vector<WifiDevice*>::iterator it;
    for (it=_DeviceList.begin(); it!= _DeviceList.end(); it++)
    {
        WifiDevice* pWifiDevice = *it;
        if (pWifiDevice->ip == ip)
        {
            return;
        }
    }

    WifiDevice* pNewWifiDevice = new WifiDevice();

    pNewWifiDevice->ip = ip;
    pNewWifiDevice->port = 5900;
    pNewWifiDevice->imei = imei;
    pNewWifiDevice->mac  = mac;
    pNewWifiDevice->model= model;

    _DeviceList.push_back(pNewWifiDevice);
}

void WifiScaner::clearDevice()
{
    vector<WifiDevice*>::iterator it;
    for (it=_DeviceList.begin(); it!= _DeviceList.end(); it++)
    {
        WifiDevice* pWifiDevice = *it;
        SAFE_DELETE(pWifiDevice);
    }
    _DeviceList.clear();
}

#endif