#include "pch_module.h"

#ifdef MODULE_SCREENSHOT

#include "Module/ScreenShot/ScreenShotManager.h"
#include "Module/ScreenShot/ScreenShotPlayer.h"
#include "Module/ScreenShot/ScreenshotCommand.h"
#include "Module/ScreenShot/ScreenImageData.h"
#include "Common/Path.h"

ScreenShotManager::ScreenShotManager(const DeviceData* pDevice)
{
	_pDevice = pDevice;
	_pScreenPlayer = new ScreenShotPlayer(_pDevice);
}

void ND91Assistant::ScreenShotManager::SetObersever( ScreenObserver* pObserver )
{
	if (pObserver)
		_pScreenPlayer->Attach(pObserver);
}


ScreenShotManager::~ScreenShotManager(void)
{
	SAFE_DELETE(_pScreenPlayer);
}

wstring ND91Assistant::ScreenShotManager::SaveCurrentScreen()
{
	wstring savePath = L"";

	if(_pScreenPlayer->GetScreenShot())
	{
		//__time32_t now;
		//_time32(&now);
		time_t now;
		time(&now);

		CDealTime dtime(now);
		ostringstream oss;
		oss<<dtime.GetYear()/1000<<(dtime.GetYear()%1000)/100<<((dtime.GetYear()%1000)%100)/10<<dtime.GetYear()%1000%100%10
			<<"-"<<dtime.GetMonth()/10<<dtime.GetMonth()%10<<"-"<<dtime.GetDay()/10<<dtime.GetDay()%10<<"-"
			<<dtime.GetHour()/10<<dtime.GetHour()%10<<"-"<<dtime.GetMinute()/10<<dtime.GetMinute()%10<<"-"
			<<dtime.GetSecond()/10<<dtime.GetSecond()%10;

		savePath = _pDevice->GetPath()->GetLocalScreensnapPath()
            + CCodeOperation::GB2312ToUnicode(oss.str())+L".png";

		_pScreenPlayer->GetScreenImage()->SaveToFile(savePath, _pDevice);

	}
	return savePath;
}

void ND91Assistant::ScreenShotManager::StartPlayScreen()
{
	_pScreenPlayer->StartThread();
}

void ND91Assistant::ScreenShotManager::StopPlay()
{
	_pScreenPlayer->StopThread();
}

void ND91Assistant::ScreenShotManager::FreshScreen()
{
	_pScreenPlayer->GetScreenShot();
}

CommandExecuteResult ND91Assistant::ScreenShotManager::CaptureScreen( wstring strSavePath )
{
    ScreenshotCommand ssCmd(_pDevice, strSavePath);
    ssCmd.Execute();
    return ssCmd.Result();
}



#endif