#pragma once

#include "Module/ScreenShot/ScreenObserver.h"

namespace ND91Assistant
{
    // ��Ļ��ͼ��ʾ����
    class ScreenShotPlayer;
    class CLASSINDLL_CLASS_DECL ScreenShotManager
    {
    public:
	    ScreenShotManager(const DeviceData* pDevice);
	    virtual ~ScreenShotManager(void);

	    // Summary  : ���ý�����
	    // Returns	: void
	    // Parameter: ScreenObserver * pObserver
	    void SetObersever(ScreenObserver* pObserver);

	    // Summary  : ���浱ǰ��Ļͼ,��Ӧ����������ť����
	    // Returns	: void
	    wstring  SaveCurrentScreen();

	    // Summary  : �Զ�������Ļ��ȫ����ʾ
	    // Returns	: void
	    void  StartPlayScreen();

	    // Summary  : ֹͣ�Զ�������Ļ���˳�ȫ����ʾ
	    // Returns	: void
	    void  StopPlay();

	    // Summary  : ˢ��
	    // Returns	: void
	    void  FreshScreen();

	    // Summary  : �����Ļ��ͼ���ش洢·��
	    // Returns	: const wstring
	    const wstring  GetScreenSnapFolder();

	    // Summary  : ���ȫ����ʾ����
	    // Returns	: const ScreenShotPlayer*
	    ScreenShotPlayer* GetScreenShotPlayer(){return _pScreenPlayer;}

        // ����
        CommandExecuteResult CaptureScreen(wstring strSavePath);
    private:

	    ScreenShotPlayer*   _pScreenPlayer;

	    const DeviceData*   _pDevice;
    };

}
