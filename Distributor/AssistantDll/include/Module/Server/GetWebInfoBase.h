#pragma once

namespace ND91Assistant
{
    class DeviceData;

	// 通过WEB交互获取各种包信息基类
	class CLASSINDLL_CLASS_DECL CGetWebInfoBase
	{
	public:
		CGetWebInfoBase();
		CGetWebInfoBase(DeviceData* pDevice);
		~CGetWebInfoBase(void);

		// 获取设备应用升级信息时调用
		string GetPhoneInfo();

		// Summary  : 获取本地应用升级信息时调用 
		// Returns  : string
		// Parameter: DEVICE_TYPE deviceType   软件类型(ios还是android)
		string GetSendInfo( int deviceType );

		wstring _strUrl;		// 安装包url
		wstring _strVersion;	// 当前最新版本号
		time_t  _updateTime;	// 版本更新时间
		int		_nFileSize;		// 安装包大小
		int		_nResult;		// 执行结果，0为正确，非0为错误
		int		_nNeedForce;    // 是否强制升级，1为强制	
		wstring _content;		// 内容描述

	protected:

		wstring GetDeviceMT();  // 获取设备平台类型
		wstring GetVersion();	// 获取手机守护版本
		wstring GetFwVersion(); // 获取设备固件版本

		bool ParseXml(string xmlData);	// 解析XML

		// WEB通信加密手机设备信息
		string GetPhoneData( string phoneVersion, string phoneSerial, string platForm );

		// 返回i=str&ks=index&key=key；  ks为加密key的索引，i为手机的信息，key为加密的key
		string EncodeInfo( string phoneData );
	protected:

		DeviceData* _pDevice;


	};

}

