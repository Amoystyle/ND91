#include "stdafx.h"
#include "GetSDPath_Android.h"
#include "AblFileOperation.h"

GetSDPath_Android::GetSDPath_Android( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	_reqeustSDPathMsg= NULL;
}

GetSDPath_Android::~GetSDPath_Android()
{
	SAFE_DELETE(_reqeustSDPathMsg);
}

bool GetSDPath_Android::Action(DeviceData* pDevice)
{
    WAIT_DEVICE_ACTIVE_NORMAL;
	RETURN_FALSE_IF(!_reqeustSDPathMsg);

	ResponseSDPathMsg msg;
	msg.nUserData = _reqeustSDPathMsg->nUserData;
	msg.nRequestID = _nGUID;
    try
    {
			if(pDevice->GetdeviceType() == Android)
			{
				SDCardState state = pDevice->AndroidGetSdCardState();
				if(state == MOUNTED 
					|| state == MOUNTED_READ_ONLY
					|| state == SHARED)
				{
					tstring sdpath = pDevice->AndroidGetSdcardPath();
					if(!sdpath.empty())
						msg.lst_sdpath.push_back(sdpath);
					sdpath = pDevice->GetAndroidExtCardPath();
					if(!sdpath.empty())
						msg.lst_sdpath.push_back(sdpath);
				}
			}
    }
    catch(AssistantException e)
    {
    }
    catch (...)
    {
    }    
	msg.SendToUI();
    return true;
}
