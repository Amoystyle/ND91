#include "stdafx.h"
#include "MobileImageMounterClient.h"

#include "Device/CFConverter.h"
#include "Module/Common/Synchronizer/DeviceLink.h"
#include "Device/IosUsbConnection.h"

MobileImageMounterClient::MobileImageMounterClient(IosUsbConnection* device)
:ServiceClientBase(device)
{
	_isServiceRunning = false;
	_serviceName = "com.apple.mobile.mobile_image_mounter";
}

MobileImageMounterClient::~MobileImageMounterClient(void)
{
}

bool ND91Assistant::MobileImageMounterClient::StartMobileImageMouterService()
{
	if(!_isServiceRunning)
	{
		_isServiceRunning = StartService();
	}
	return _isServiceRunning;
}

bool ND91Assistant::MobileImageMounterClient::MountImage( string imagePathOnDevice, string imageSignature)
{
	if(StartMobileImageMouterService())
	{
		map<string, void*> msgDict;
		msgDict["Command"]=CCFConverter::StringtoCFString("MountImage");
		msgDict["ImagePath"]=CCFConverter::StringtoCFString(imagePathOnDevice);
		msgDict["ImageSignature"]=CCFConverter::NDCFDataCreate(NULL,(unsigned char*)(imageSignature.c_str()),imageSignature.length());
		msgDict["ImageType"]= CCFConverter::Int32ToCFInt32(0);

		if(SendMessageToService(msgDict))
		{
			map<string, string> resultmap;
			ReadMessageFromService(resultmap);
			if(resultmap.find("Status")!=resultmap.end() && resultmap["Status"]=="Complete")
				return true;
		}
	}
	return false;
}