#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "OMSAddContactPhotoCommand.h"
#include "Module/AndroidDefine.h"

#define OMS_CONTACT_PATH L"/sdcard/image/contact/"

OMSAddContactPhotoCommand::OMSAddContactPhotoCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = PHOTOS;
	_uri = L"";
}

OMSAddContactPhotoCommand::~OMSAddContactPhotoCommand(void)
{
}

void ND91Assistant::OMSAddContactPhotoCommand::Execute()
{
	_uri = L"";
	
	UploadFileCommand upfilecmd(_pDevice);
	upfilecmd.setfilename(_localpath, OMS_CONTACT_PATH+CFileOperation::GetFileName(_localpath));
	upfilecmd.Execute();
	if (upfilecmd.GetUploadResult() == Transfer_Success)
		DefaultExecute();
}

std::string ND91Assistant::OMSAddContactPhotoCommand::GetAndroidBodyData()
{
	wstring  remotePath = OMS_CONTACT_PATH+CFileOperation::GetFileName(_localpath);
	BinaryBufferWriter  bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteChar(5);
	bufwriter.WriteString(MyFileOperation::GetImageFileMime(_localpath));
	bufwriter.WriteString(CStrOperation::CreateHashCode("/sdcard/image/contact"));
	bufwriter.WriteString("/sdcard/image/contact");

	return bufwriter.GetBuffer();

}

void ND91Assistant::OMSAddContactPhotoCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead_Top16(&reader))
		return;

	reader.ReadStringFromUTF8(_uri);
    _pRetPicture._photoUri = _uri;
	_pRetPicture._photoId = (atoi(CCodeOperation::WstringToString(CFileOperation::GetFileName(_uri)).c_str()));
	_pRetPicture._photoPath = OMS_CONTACT_PATH+CFileOperation::GetFileName(_localpath);
}

#endif