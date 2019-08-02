#include "stdafx.h"
#include "LoadDeviceImageThread.h"
#include "Module/System/SystemManager.h"

LoadDeviceImageThread::LoadDeviceImageThread( string threadID, wstring deviceID, bool bRefresh )
						: DeviceBaseThread(threadID, deviceID)
{
    _autoDelete = true;
	_bRefresh = bRefresh;
}

LoadDeviceImageThread::~LoadDeviceImageThread()
{
}

unsigned long LoadDeviceImageThread::Main()
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if (!pDevice)
		return false;

	RETURN_VALUE_IF(CheckExit(), 0);

	int minSize = 5000;
	CGetDeviceImageInfo info(pDevice);
	wstring strFile = CONFIG(_strDevicePhotoPath);
	strFile += pDevice->GetDevInfo()->GetDevModelName();
	strFile += L"_" + pDevice->GetDevInfo()->_ios_deviceColor + L".png";

	if (_bRefresh)
	{
		CFileOperation::DeleteFile(strFile);
	}
	
	RETURN_VALUE_IF(CheckExit(), 0);

	CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(strFile));
	bool bRec = false;
	bool bShow = false;
	if (CFileOperation::GetFileSizeW(strFile) < 100)
	{
		bRec = info.GetDeviceImageInfo();

		RETURN_VALUE_IF(CheckExit(), 0);

		if (bRec && info._url.length() > 0)
		{
			if (CFileOperation::GetFileSizeW(strFile) < minSize)
			{
				CNetOperation::DownloadFile(CCodeOperation::UnicodeToGB2312(info._url), strFile);

				RETURN_VALUE_IF(CheckExit(), 0);

				if (CFileOperation::GetFileSizeW(strFile) < minSize)
				{ //ÏÂÔØÊ§°ÜÖØÊÔ
					if (WaitForSingleObject(_hExitEvent, 500) != WAIT_TIMEOUT)
						return false;
					CNetOperation::DownloadFile(CCodeOperation::UnicodeToGB2312(info._url), strFile);
				}
			}
		}
	}

	RETURN_VALUE_IF(CheckExit(), 0);

	ResponseDeviceImageMsg msg;
	msg.deviceID = this->_deviceID;
	msg.imagePath = strFile;
	msg.SendToUI();
	return 0;
}