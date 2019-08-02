#pragma once
#include "AssistantDll.h"
#include <string>
using namespace std;
namespace ND91Assistant
{
    // 一个硬件的信息类
    class CLASSINDLL_CLASS_DECL HardwareInfo
    {
    friend class DriverManager;

    public:
        //HardwareInfo(HARDWARE_TYPE nType, HDEVINFO hDevinfo, SP_DEVINFO_DATA device);
		HardwareInfo(HARDWARE_TYPE nType);
        HardwareInfo(const HardwareInfo* pInfo);
        ~HardwareInfo();

    public:
        HARDWARE_TYPE _nType;

        wstring _strProviderName;    //制造厂商
        wstring _strDriverDesc;      //设备名
        wstring _strHardID;          //硬件序列号
        wstring _strCompareID;       //设备复合信息

		string	_strVID;
		string  _strPID;
		string	_strMI;
		string  _strKey;

		int		_vid;
		int		_pid;	

		unsigned long	_nState;			 //设备状态，0表示正常(不需要安装驱动) 否则表示需要安装驱动
		
    private:


    };
}