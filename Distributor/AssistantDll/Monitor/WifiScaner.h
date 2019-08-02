#ifdef FOLLOWING_IS_IGNORE_CODE  // 局域网扫描功能已被废弃
#pragma once

#include <windows.h>
#include <string>
#include <vector>
#pragma  comment(lib, "Ws2_32.lib") 
using namespace std;
namespace ND91Assistant
{
    // 用于检测局域网中的可连接的守护，当前需要定点发送UDP包激活，才能收到守护发送的UDP包
    // 设置广播的地址 和 定点发送的地址 尚未实现
    //  !!! 该方法已被废弃 !!! 停止更新！
    class WifiScaner
    {
#define WIFI_UDP_FROM_PC            "91 PC SUITE"
#define WIFI_UDP_FROM_DEMON_HEAD    "91 DEAMON|"
#define WIFI_UDP_PORT               46923
    public:
        struct WifiDevice{
            string  ip;         //ip
            int     port;       //端口
            string  mac;        //Mac地址
            string  imei;       //imei
            string  model;      //手机型号
            //int     nDetectTime;//检测到的时间
        };

        WifiScaner();
        ~WifiScaner();

        //启动检测线程，包含发送线程 和 接收线程
        virtual void StartThread();

        //停止检测线程
        virtual void StopThread();

        vector<WifiDevice*> getDeviceList(){return _DeviceList;};
    private:
        //开启一个广播UDP数据包的线程，往所有局域网段广播UDP数据
        static DWORD WINAPI SendData(LPVOID lpParameter);

        //开启一个接收UDP数据包的线程，接收守护广播出来的UDP数据包
        static DWORD WINAPI ReceData(LPVOID lpParameter);

        //将检测到的设备放进列表
        void AddNewDevice(string ip, string imei, string mac, string model);

        //清空检测到的设备列表
        void clearDevice();


        bool    getSendThreadQuitSign(){return _bSendThreadQuitSign;};
        bool    getReceThreadQuitSign(){return _bReceThreadQuitSign;};
    private:
        vector<WifiDevice*> _DeviceList; //记录当前检测到的设备列表
        
        //以下为线程相关
        bool    _bSendThreadQuitSign;       //关闭线程标志
        bool    _bReceThreadQuitSign;       //关闭线程标志
        DWORD	_nSendThreadId;				//开启的线程ID
        DWORD   _nReceThreadId;             //开启的线程ID
        HANDLE  _hSendThread;               //开启的线程
        HANDLE  _hReceThread;

    };
}

#endif