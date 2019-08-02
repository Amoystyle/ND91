#pragma once

#include "GetWebInfoBase.h"

namespace ND91Assistant
{
	//网络获取iphone设备信息的解析类
	class CLASSINDLL_CLASS_DECL CGetIPhoneSellInfo : public CGetWebInfoBase
	{
	public:
		CGetIPhoneSellInfo(DeviceData* pDevice);
		~CGetIPhoneSellInfo();

		//bool GetDeviceSellInfo();
        //bool LoadFrom91();

        // 获取设备保修相关信息
        bool GetWarrantInfo();

        wstring GetSn() { return _strSn; }                              // 获取设备序列号
        wstring GetPurchCountry() { return _strPurchCountry; }          // 获取发售国家
        wstring GetLastUnbrick() { return _strLastUnbrick; }            // 获取上次解锁
        wstring GetPHEndDate() { return _strPHEndDate; }                // 获取电话服务结束时间
        wstring GetHash() { return _strHash; }                          // 获取服务端存储apple获取信息hash值
        wstring GetColor() { return _strColor; }                        // 获取设备颜色
        wstring GetPurchaseDate() { return _strPurchaseDate; }          // 获取购买日期
        wstring GetCovEndDate() { return _strCovEndDate; }              // 获取保修日期

        int GetIsRegistered() { return _nIsRegistered; }                // 获取是否注册        -1=未知 0=No 1=Yes
        int GetActivationStatus() { return _nActivationStatus; }        // 获取是否激活        -1=未知 0=No 1=Yes
        int GetPHHasCoverage() { return _nPHHasCoverage; }              // 获取是否有电话服务  -1=未知 0=No 1=Yes
        int GetHWHasCoverage() { return _nHWHasCoverage; }              // 获取是否硬件保修    -1=未知 0=No 1=Yes

        bool GetSuccess() const { return _bSuccess; }

    private:
        // 从91服务器获取保修相关信息
        int RequestWarrantyFrom91Server(string& retWarrantyStr);

        // 从苹果服务器上获取保修信息
        int RequestWarrantyFromAppleServer(string& retWarrantyStr);

        // 解析91服务器反馈的信息
        void Analytic91Html(string htmlStr);

        // 解析苹果服务器反馈的信息
        void AnalyticAppleHtml(string htmlStr);

        wstring _strSn;                 // 设备序列号
        wstring _strPurchCountry;       // 发售国家
        wstring _strLastUnbrick;        // 上次解锁
        wstring _strPHEndDate;          // 电话服务结束时间
        wstring _strHash;               // 服务端存储apple获取信息hash值
        wstring _strColor;              // 设备颜色
        wstring _strPurchaseDate;       // 购买日期
        wstring _strCovEndDate;         // 保修日期

        int _nIsRegistered;             // 是否注册        -1=未知 0=No 1=Yes
        int _nActivationStatus;         // 是否激活        -1=未知 0=No 1=Yes
        int _nPHHasCoverage;            // 是否有电话服务  -1=未知 0=No 1=Yes
        int _nHWHasCoverage;            // 是否硬件保修    -1=未知 0=No 1=Yes
        
        bool _bSuccess;				    // 是否获取数据成功
    };
}

