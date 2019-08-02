#ifdef FOLLOWING_IS_IGNORE_CODE  // ������ɨ�蹦���ѱ�����
#pragma once

#include <windows.h>
#include <string>
#include <vector>
#pragma  comment(lib, "Ws2_32.lib") 
using namespace std;
namespace ND91Assistant
{
    // ���ڼ��������еĿ����ӵ��ػ�����ǰ��Ҫ���㷢��UDP����������յ��ػ����͵�UDP��
    // ���ù㲥�ĵ�ַ �� ���㷢�͵ĵ�ַ ��δʵ��
    //  !!! �÷����ѱ����� !!! ֹͣ���£�
    class WifiScaner
    {
#define WIFI_UDP_FROM_PC            "91 PC SUITE"
#define WIFI_UDP_FROM_DEMON_HEAD    "91 DEAMON|"
#define WIFI_UDP_PORT               46923
    public:
        struct WifiDevice{
            string  ip;         //ip
            int     port;       //�˿�
            string  mac;        //Mac��ַ
            string  imei;       //imei
            string  model;      //�ֻ��ͺ�
            //int     nDetectTime;//��⵽��ʱ��
        };

        WifiScaner();
        ~WifiScaner();

        //��������̣߳����������߳� �� �����߳�
        virtual void StartThread();

        //ֹͣ����߳�
        virtual void StopThread();

        vector<WifiDevice*> getDeviceList(){return _DeviceList;};
    private:
        //����һ���㲥UDP���ݰ����̣߳������о������ι㲥UDP����
        static DWORD WINAPI SendData(LPVOID lpParameter);

        //����һ������UDP���ݰ����̣߳������ػ��㲥������UDP���ݰ�
        static DWORD WINAPI ReceData(LPVOID lpParameter);

        //����⵽���豸�Ž��б�
        void AddNewDevice(string ip, string imei, string mac, string model);

        //��ռ�⵽���豸�б�
        void clearDevice();


        bool    getSendThreadQuitSign(){return _bSendThreadQuitSign;};
        bool    getReceThreadQuitSign(){return _bReceThreadQuitSign;};
    private:
        vector<WifiDevice*> _DeviceList; //��¼��ǰ��⵽���豸�б�
        
        //����Ϊ�߳����
        bool    _bSendThreadQuitSign;       //�ر��̱߳�־
        bool    _bReceThreadQuitSign;       //�ر��̱߳�־
        DWORD	_nSendThreadId;				//�������߳�ID
        DWORD   _nReceThreadId;             //�������߳�ID
        HANDLE  _hSendThread;               //�������߳�
        HANDLE  _hReceThread;

    };
}

#endif