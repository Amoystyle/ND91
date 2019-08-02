#pragma once

#include "Module/Application/IosIcon/IosIconSet.h"

namespace ND91Assistant
{
    // itunes跟踪到的支持监听 __UIKitDesktopImageChangedNotification 这个事件
	// IOS设备的屏幕操作类, 实现与com.apple.springboardservices服务通讯
	// 主要用于图标(位置),壁纸等信息获取设置 (只支持3.X以上)
	class IosIconSet;
	class CLASSINDLL_CLASS_DECL IosScreenOperation
	{
	public:
		//************************************
		// Method:    IosScreenOperation 构造函数
		// FullName:  IosScreenOperation::IosScreenOperation
		// Access:    public 
		// Returns:   
		// Qualifier:
		// Parameter: void * pDevPtr ：等于_ios_devptr
		// Parameter: wstring strProductVersion ：等于_deviceProductVersion
		// Parameter: bool biPad ：等于IsPad()
		//************************************
		IosScreenOperation(void* pDevPtr, wstring strProductVersion, bool biPad);
		~IosScreenOperation(void);

		// 获取图标状态信息
		IosIconSet* GetIconState();

		// 设置新的图标状态
		bool SetIconState();	

		// 释放图标状态信息
		void FreeIconSet();

        // 增加对未激活设备的支持
        bool AllowUnactivated() { return false; }

	private:	
		// 获取设备屏幕方向
		eInterfaceOrientation GetInterfaceOrientation();

		// 获取主屏幕壁纸(固件4.0开始才有这个功能,3GS机器开始,3G需要手动开启)
		bool GetHomeScreenWallpaper(wstring strSavePath);

		bool SendMessageToService(void* pSocket, map<string, string> mapMsg);
		IosIconSet* ReadMssageFromService(void* pSocket);

		void InitIconImageFile(IosIcon* pIcon);
		wstring GetIconImageData(string strBundleID);

		void StartSpringBoardService();
		void CloseSpringBoardService();

	private:
		void*                   _pSpringBoardServiceHandle; // 屏幕操作的服务句柄
		void*                   _pDevPtr;                   // 设备句柄
		wstring                 _strProductVersion;         // 设备的版本号
		int	                    _nVer;                      // 设备版本号的数字表示
		bool                    _biPad;                     // 是否为IPAD
		eInterfaceOrientation   _eInterfaceOrientationType; // 横竖屏标识
		IosIconSet*             _pIosIconSet;               // 设备屏幕的数据类指针
        CRITICAL_SECTION        _cs;                        // 访问临界值
	};

}
