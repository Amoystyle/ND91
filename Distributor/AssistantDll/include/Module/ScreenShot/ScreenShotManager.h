#pragma once

#include "Module/ScreenShot/ScreenObserver.h"

namespace ND91Assistant
{
    // 屏幕截图演示管理
    class ScreenShotPlayer;
    class CLASSINDLL_CLASS_DECL ScreenShotManager
    {
    public:
	    ScreenShotManager(const DeviceData* pDevice);
	    virtual ~ScreenShotManager(void);

	    // Summary  : 设置接收者
	    // Returns	: void
	    // Parameter: ScreenObserver * pObserver
	    void SetObersever(ScreenObserver* pObserver);

	    // Summary  : 保存当前屏幕图,对应“截屏“按钮功能
	    // Returns	: void
	    wstring  SaveCurrentScreen();

	    // Summary  : 自动更新屏幕或全屏演示
	    // Returns	: void
	    void  StartPlayScreen();

	    // Summary  : 停止自动更新屏幕或退出全屏演示
	    // Returns	: void
	    void  StopPlay();

	    // Summary  : 刷新
	    // Returns	: void
	    void  FreshScreen();

	    // Summary  : 获得屏幕截图本地存储路径
	    // Returns	: const wstring
	    const wstring  GetScreenSnapFolder();

	    // Summary  : 获得全屏演示对象
	    // Returns	: const ScreenShotPlayer*
	    ScreenShotPlayer* GetScreenShotPlayer(){return _pScreenPlayer;}

        // 截屏
        CommandExecuteResult CaptureScreen(wstring strSavePath);
    private:

	    ScreenShotPlayer*   _pScreenPlayer;

	    const DeviceData*   _pDevice;
    };

}
